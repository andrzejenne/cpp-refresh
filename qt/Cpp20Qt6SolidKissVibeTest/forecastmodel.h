#pragma once

#include "dailyforecast.h"

#include <QAbstractListModel>
#include <QList>
#include <qqmlregistration.h>

// Holds the forecast rows and nothing else -> Single Responsibility. Exposes
// exactly the roles a list delegate needs (Interface Segregation): the view
// binds to day/high/low/code, not to a fat domain object.
class ForecastModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Owned and populated by WeatherController")

public:
    enum Role { DayRole = Qt::UserRole + 1, HighRole, LowRole, CodeRole };

    using QAbstractListModel::QAbstractListModel;

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setDays(const QList<DailyForecast> &days);

private:
    QList<DailyForecast> m_days;
};
