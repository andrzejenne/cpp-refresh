#pragma once

#include <QObject>
#include <QString>

// Plain value type for one day's forecast. Q_GADGET keeps it a lightweight
// value (no QObject overhead); it is carried inside signals and stored by the
// model, which exposes its fields to QML as roles.
struct DailyForecast
{
    Q_GADGET
    Q_PROPERTY(QString day MEMBER day)
    Q_PROPERTY(double high MEMBER high)
    Q_PROPERTY(double low MEMBER low)
    Q_PROPERTY(int code MEMBER code)

public:
    QString day;      // e.g. "Mon"
    double high = 0.0; // °C
    double low = 0.0;  // °C
    int code = 0;      // WMO weather code
};
