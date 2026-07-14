#pragma once

#include "forecastmodel.h"

#include <QObject>
#include <QString>
#include <qqmlregistration.h>

class WeatherService;

// The ViewModel: translates user intent (search) into service calls and exposes
// view-ready state. It owns the ForecastModel and depends only on the abstract
// WeatherService, received via setService() -> Dependency Inversion, and
// trivially testable with a fake service. Exposed to QML as an app-wide
// singleton.
class WeatherController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString city READ city NOTIFY dataChanged)
    Q_PROPERTY(double temperature READ temperature NOTIFY dataChanged)
    Q_PROPERTY(int weatherCode READ weatherCode NOTIFY dataChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorChanged)
    Q_PROPERTY(ForecastModel *forecast READ forecast CONSTANT)

public:
    explicit WeatherController(QObject *parent = nullptr);

    // Injection seam: the composition root wires in a concrete provider.
    void setService(WeatherService *service);

    QString city() const { return m_city; }
    double temperature() const { return m_temperature; }
    int weatherCode() const { return m_weatherCode; }
    bool busy() const { return m_busy; }
    QString errorString() const { return m_error; }
    ForecastModel *forecast() { return &m_forecast; }

    Q_INVOKABLE void search(const QString &city);

signals:
    void dataChanged();
    void busyChanged();
    void errorChanged();

private:
    void setBusy(bool busy);
    void setError(const QString &message);

    WeatherService *m_service = nullptr;
    ForecastModel m_forecast;
    QString m_city;
    double m_temperature = 0.0;
    int m_weatherCode = 0;
    bool m_busy = false;
    QString m_error;
};
