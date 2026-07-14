#pragma once

#include "weatherservice.h"

#include <QNetworkAccessManager>

// One concrete provider built on the free Open-Meteo API (no key required).
// Its single responsibility is talking to that provider: geocode the city,
// then fetch the forecast, then translate the JSON into domain types. It knows
// nothing about the UI or the controller.
class OpenMeteoService : public WeatherService
{
    Q_OBJECT

public:
    explicit OpenMeteoService(QObject *parent = nullptr);

    void fetch(const QString &city) override;

private:
    void requestForecast(const QString &name, double lat, double lon);

    QNetworkAccessManager m_net;
};
