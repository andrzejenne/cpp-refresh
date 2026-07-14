#include "forecastmodel.h"

using namespace Qt::StringLiterals;

int ForecastModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : int(m_days.size());
}

QVariant ForecastModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_days.size())
        return {};

    const DailyForecast &d = m_days.at(index.row());
    switch (role) {
    case DayRole:  return d.day;
    case HighRole: return d.high;
    case LowRole:  return d.low;
    case CodeRole: return d.code;
    default:       return {};
    }
}

QHash<int, QByteArray> ForecastModel::roleNames() const
{
    return {
        {DayRole, "day"_ba},
        {HighRole, "high"_ba},
        {LowRole, "low"_ba},
        {CodeRole, "code"_ba},
    };
}

void ForecastModel::setDays(const QList<DailyForecast> &days)
{
    beginResetModel();
    m_days = days;
    endResetModel();
}
