#include "WeatherService.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

#include <algorithm>
#include <limits>

// Enabled with -v / --verbose (see main.cpp).
Q_LOGGING_CATEGORY(lcOM, "openmeteo")

WeatherService::WeatherService(QObject* parent) : QObject(parent) {}

void WeatherService::setLoading(bool value)
{
    if (m_loading == value)
        return;
    m_loading = value;
    emit loadingChanged();
}

double WeatherService::convert(double celsius) const
{
    if (m_unit == QLatin1String("F"))
        return celsius * 9.0 / 5.0 + 32.0;
    if (m_unit == QLatin1String("K"))
        return celsius + 273.15;
    return celsius; // Celsius
}

QString WeatherService::unitSuffix() const
{
    if (m_unit == QLatin1String("F"))
        return QStringLiteral("°F");
    if (m_unit == QLatin1String("K"))
        return QStringLiteral("K");
    return QStringLiteral("°C");
}

void WeatherService::setUnit(const QString& unit)
{
    const QString u = unit.trimmed().toUpper();
    if (u != QLatin1String("C") && u != QLatin1String("F") && u != QLatin1String("K"))
        return;
    if (u == m_unit)
        return;
    m_unit = u;
    emit unitChanged();
    emit dataChanged(); // converted values changed -> chart rebuilds
}

void WeatherService::fetch(double latitude, double longitude, const QString& startDate,
                           const QString& endDate)
{
    QUrl url(QStringLiteral("https://api.open-meteo.com/v1/forecast"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("latitude"), QString::number(latitude, 'f', 5));
    query.addQueryItem(QStringLiteral("longitude"), QString::number(longitude, 'f', 5));
    query.addQueryItem(QStringLiteral("hourly"), QStringLiteral("temperature_2m"));
    query.addQueryItem(QStringLiteral("timezone"), QStringLiteral("auto"));
    if (!startDate.isEmpty() && !endDate.isEmpty()) {
        query.addQueryItem(QStringLiteral("start_date"), startDate);
        query.addQueryItem(QStringLiteral("end_date"), endDate);
    } else {
        query.addQueryItem(QStringLiteral("forecast_days"), QStringLiteral("7"));
    }
    url.setQuery(query);

    qCDebug(lcOM) << "GET" << url.toString();

    // Cancel any request still in flight so its (now stale) reply cannot
    // overwrite this one's data or flip `loading` off prematurely. The aborted
    // reply still emits finished() with OperationCanceledError, which the
    // handler below ignores.
    if (m_reply)
        m_reply->abort();

    setLoading(true);

    QNetworkReply* reply = m_net.get(QNetworkRequest(url));
    m_reply = reply;

    // Thin completion handler: tear down the reply, then delegate the parsing
    // to handleReply(). latitude/longitude are captured so the label can be
    // built from the coordinates this request was made with.
    connect(reply, &QNetworkReply::finished, this, [this, reply, latitude, longitude]() {
        reply->deleteLater();

        // A reply we aborted because a newer fetch() superseded it: drop it
        // silently and leave `loading` to the request that replaced it.
        if (reply->error() == QNetworkReply::OperationCanceledError)
            return;

        if (m_reply == reply)
            m_reply = nullptr;
        setLoading(false);

        const QByteArray body = reply->readAll();
        const int httpStatus =
            reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qCDebug(lcOM) << "reply: httpStatus=" << httpStatus << "bytes=" << body.size()
                      << "netError=" << reply->error();

        handleReply(body, reply->error() != QNetworkReply::NoError, reply->errorString(),
                    latitude, longitude);
    });
}

void WeatherService::handleReply(const QByteArray& body, bool hadNetworkError,
                                 const QString& networkErrorString, double latitude,
                                 double longitude)
{
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        // Network failure with no parseable body.
        if (hadNetworkError)
            emit errorOccurred(networkErrorString);
        else
            emit errorOccurred(QStringLiteral("Could not parse Open-Meteo response"));
        return;
    }

    const QJsonObject obj = doc.object();
    // Open-Meteo reports problems as {"error": true, "reason": "..."}.
    if (obj.value(QStringLiteral("error")).toBool()) {
        emit errorOccurred(obj.value(QStringLiteral("reason"))
                               .toString(QStringLiteral("Open-Meteo request rejected")));
        return;
    }

    updateSeries(obj, latitude, longitude);
}

void WeatherService::updateSeries(const QJsonObject& root, double latitude, double longitude)
{
    const QJsonObject hourly = root.value(QStringLiteral("hourly")).toObject();
    const QJsonArray temps = hourly.value(QStringLiteral("temperature_2m")).toArray();
    const QJsonArray times = hourly.value(QStringLiteral("time")).toArray();

    m_temps.clear();
    m_times.clear();
    m_timeMs.clear();
    m_temps.reserve(temps.size());
    m_times.reserve(times.size());
    m_timeMs.reserve(times.size());

    double lo = std::numeric_limits<double>::max();
    double hi = std::numeric_limits<double>::lowest();
    for (const QJsonValue& v : temps) {
        const double t = v.toDouble();
        m_temps.append(t);
        lo = std::min(lo, t);
        hi = std::max(hi, t);
    }
    for (const QJsonValue& v : times) {
        const QString s = v.toString();
        m_times.append(s);
        // Values are local wall-clock (timezone=auto); seconds may be absent.
        QDateTime dt = QDateTime::fromString(s, QStringLiteral("yyyy-MM-dd'T'hh:mm"));
        if (!dt.isValid())
            dt = QDateTime::fromString(s, Qt::ISODate);
        m_timeMs.append(dt.isValid() ? static_cast<double>(dt.toMSecsSinceEpoch()) : 0.0);
    }

    if (m_temps.isEmpty()) {
        m_minC = 0.0;
        m_maxC = 0.0;
        emit errorOccurred(QStringLiteral("No temperature data returned for this range"));
        emit dataChanged();
        return;
    }

    m_minC = lo;
    m_maxC = hi;
    m_locationLabel =
        QStringLiteral("%1°, %2°").arg(latitude, 0, 'f', 4).arg(longitude, 0, 'f', 4);

    qCDebug(lcOM) << "parsed" << m_temps.size() << "points; minC=" << m_minC
                  << "maxC=" << m_maxC << "first=" << m_times.constFirst()
                  << "(ms" << m_timeMs.constFirst() << ") last=" << m_times.constLast();

    emit dataChanged();
}

double WeatherService::temperatureAt(int index) const
{
    if (index < 0 || index >= m_temps.size())
        return 0.0;
    return convert(m_temps.at(index));
}

QString WeatherService::timeAt(int index) const
{
    if (index < 0 || index >= m_times.size())
        return {};
    return m_times.at(index);
}

double WeatherService::timeMsAt(int index) const
{
    if (index < 0 || index >= m_timeMs.size())
        return 0.0;
    return m_timeMs.at(index);
}
