#pragma once

#include <QList>
#include <QNetworkAccessManager>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QtQml/qqmlregistration.h>

class QNetworkReply;

class QJsonObject;

// Backend exposed to QML as the `WeatherService` element (import App).
// QML calls fetch(lat, lon, startDate, endDate); on completion `dataChanged`
// fires and the chart reads the hourly temperatures via count()/temperatureAt().
//
// Temperatures are always fetched and stored in Celsius; the `unit` property
// converts on read, so switching C/F/K needs no new network request.
class WeatherService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString locationLabel READ locationLabel NOTIFY dataChanged)
    Q_PROPERTY(int count READ count NOTIFY dataChanged)
    Q_PROPERTY(double minTemp READ minTemp NOTIFY dataChanged)
    Q_PROPERTY(double maxTemp READ maxTemp NOTIFY dataChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(QString unitSuffix READ unitSuffix NOTIFY unitChanged)

public:
    explicit WeatherService(QObject* parent = nullptr);

    bool loading() const { return m_loading; }
    QString locationLabel() const { return m_locationLabel; }
    int count() const { return static_cast<int>(m_temps.size()); }
    double minTemp() const { return convert(m_minC); }
    double maxTemp() const { return convert(m_maxC); }

    QString unit() const { return m_unit; }
    void setUnit(const QString& unit);
    QString unitSuffix() const;

    // startDate/endDate are ISO "yyyy-MM-dd"; if either is empty a default
    // 7-day forecast is requested instead.
    Q_INVOKABLE void fetch(double latitude, double longitude,
                           const QString& startDate = QString(),
                           const QString& endDate = QString());

    // Accessors used by the QML chart to populate the series (unit-converted).
    Q_INVOKABLE double temperatureAt(int index) const;
    Q_INVOKABLE QString timeAt(int index) const;
    // Timestamp of sample `index` as msecs since epoch (x value for DateTimeAxis).
    Q_INVOKABLE double timeMsAt(int index) const;

signals:
    void loadingChanged();
    void dataChanged();
    void unitChanged();
    void errorOccurred(const QString& message);

private:
    void setLoading(bool value);
    double convert(double celsius) const;

    // Completion path for fetch(): the finished-lambda unpacks the reply and
    // calls handleReply(), which validates the response and then hands a good
    // payload to updateSeries() to populate the stored samples.
    void handleReply(const QByteArray& body, bool hadNetworkError,
                     const QString& networkErrorString, double latitude, double longitude);
    void updateSeries(const QJsonObject& root, double latitude, double longitude);

    QNetworkAccessManager m_net;
    // The currently in-flight request, if any. A new fetch() aborts it so a
    // stale reply can never clobber fresher data or flip `loading` early.
    QPointer<QNetworkReply> m_reply;
    bool m_loading = false;
    QString m_locationLabel;
    QString m_unit = QStringLiteral("C");
    QList<double> m_temps;   // stored in Celsius
    QList<QString> m_times;  // ISO timestamps, e.g. "2026-05-29T13:00"
    QList<double> m_timeMs;  // matching msecs since epoch
    double m_minC = 0.0;
    double m_maxC = 0.0;
};
