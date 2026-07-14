#include "weathercontroller.h"

#include "weatherservice.h"

using namespace Qt::StringLiterals;

WeatherController::WeatherController(QObject *parent)
    : QObject(parent)
{
}

void WeatherController::setService(WeatherService *service)
{
    if (m_service == service)
        return;

    if (m_service)
        m_service->disconnect(this);

    m_service = service;
    if (!m_service)
        return;

    connect(m_service, &WeatherService::ready, this,
            [this](const QString &name, double tempC, int code,
                   const QList<DailyForecast> &days) {
                m_city = name;
                m_temperature = tempC;
                m_weatherCode = code;
                m_forecast.setDays(days);
                setError({});
                setBusy(false);
                emit dataChanged();
            });

    connect(m_service, &WeatherService::failed, this,
            [this](const QString &message) {
                setError(message);
                setBusy(false);
            });
}

void WeatherController::search(const QString &city)
{
    const QString trimmed = city.trimmed();
    if (trimmed.isEmpty() || m_busy)
        return;

    if (!m_service) {
        setError(u"No weather service configured"_s);
        return;
    }

    setError({});
    setBusy(true);
    m_service->fetch(trimmed);
}

void WeatherController::setBusy(bool busy)
{
    if (m_busy == busy)
        return;
    m_busy = busy;
    emit busyChanged();
}

void WeatherController::setError(const QString &message)
{
    if (m_error == message)
        return;
    m_error = message;
    emit errorChanged();
}
