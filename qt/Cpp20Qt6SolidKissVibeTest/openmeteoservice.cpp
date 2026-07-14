#include "openmeteoservice.h"

#include <QDate>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

using namespace Qt::StringLiterals;

namespace {

QUrl geocodeUrl(const QString &city)
{
    QUrl url(u"https://geocoding-api.open-meteo.com/v1/search"_s);
    QUrlQuery q;
    q.addQueryItem(u"name"_s, city);
    q.addQueryItem(u"count"_s, u"1"_s);
    url.setQuery(q);
    return url;
}

QUrl forecastUrl(double lat, double lon)
{
    QUrl url(u"https://api.open-meteo.com/v1/forecast"_s);
    QUrlQuery q;
    q.addQueryItem(u"latitude"_s, QString::number(lat));
    q.addQueryItem(u"longitude"_s, QString::number(lon));
    q.addQueryItem(u"current"_s, u"temperature_2m,weather_code"_s);
    q.addQueryItem(u"daily"_s, u"weather_code,temperature_2m_max,temperature_2m_min"_s);
    q.addQueryItem(u"timezone"_s, u"auto"_s);
    q.addQueryItem(u"forecast_days"_s, u"5"_s);
    url.setQuery(q);
    return url;
}

QString shortDay(const QString &isoDate)
{
    const QDate d = QDate::fromString(isoDate, Qt::ISODate);
    return d.isValid() ? d.toString(u"ddd"_s) : isoDate;
}

} // namespace

OpenMeteoService::OpenMeteoService(QObject *parent)
    : WeatherService(parent)
{
}

void OpenMeteoService::fetch(const QString &city)
{
    QNetworkReply *reply = m_net.get(QNetworkRequest(geocodeUrl(city)));
    connect(reply, &QNetworkReply::finished, this, [this, reply, city] {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit failed(reply->errorString());
            return;
        }
        const auto doc = QJsonDocument::fromJson(reply->readAll());
        const auto results = doc.object().value(u"results"_s).toArray();
        if (results.isEmpty()) {
            emit failed(u"No match for \"%1\""_s.arg(city));
            return;
        }
        const auto place = results.first().toObject();
        requestForecast(place.value(u"name"_s).toString(),
                        place.value(u"latitude"_s).toDouble(),
                        place.value(u"longitude"_s).toDouble());
    });
}

void OpenMeteoService::requestForecast(const QString &name, double lat, double lon)
{
    QNetworkReply *reply = m_net.get(QNetworkRequest(forecastUrl(lat, lon)));
    connect(reply, &QNetworkReply::finished, this, [this, reply, name] {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit failed(reply->errorString());
            return;
        }

        const auto root = QJsonDocument::fromJson(reply->readAll()).object();
        const auto current = root.value(u"current"_s).toObject();
        const auto daily = root.value(u"daily"_s).toObject();

        const auto dates = daily.value(u"time"_s).toArray();
        const auto codes = daily.value(u"weather_code"_s).toArray();
        const auto highs = daily.value(u"temperature_2m_max"_s).toArray();
        const auto lows = daily.value(u"temperature_2m_min"_s).toArray();

        QList<DailyForecast> days;
        days.reserve(dates.size());
        for (qsizetype i = 0; i < dates.size(); ++i) {
            days.push_back(DailyForecast{
                .day = shortDay(dates.at(i).toString()),
                .high = highs.at(i).toDouble(),
                .low = lows.at(i).toDouble(),
                .code = codes.at(i).toInt(),
            });
        }

        emit ready(name,
                   current.value(u"temperature_2m"_s).toDouble(),
                   current.value(u"weather_code"_s).toInt(),
                   days);
    });
}
