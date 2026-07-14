#pragma once

#include "dailyforecast.h"

#include <QList>
#include <QObject>
#include <QString>
#include <qqmlregistration.h>

// The abstraction the rest of the app depends on. The controller knows only
// this interface, never a concrete provider -> Dependency Inversion. New
// providers (a cached one, a mock, a different API) are added as subclasses
// without touching the controller -> Open/Closed. Registered as an uncreatable
// QML type so QML can refer to the type without instantiating it.
class WeatherService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("WeatherService is an interface; use a concrete provider")

public:
    using QObject::QObject;
    ~WeatherService() override = default;

    // Single responsibility: resolve a place name and return its weather.
    virtual void fetch(const QString &city) = 0;

signals:
    // Emitted on success with the resolved location name, current conditions
    // and a short daily forecast.
    void ready(const QString &resolvedName,
               double currentTempC,
               int currentCode,
               const QList<DailyForecast> &days);

    // Emitted on any failure (network, not-found, parse) with a display message.
    void failed(const QString &message);
};
