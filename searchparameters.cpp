#include "searchparameters.h"
#include <QString>

SearchParameters::SearchParameters(QString stationFrom, QString stationTo, QDate tripDate)
    :_stationFrom(stationFrom),_stationTo(stationTo),_tripDate(tripDate)
{

}

SearchParameters::SearchParameters(SearchParameters &sparams)
{
    _coachTypes = sparams._coachTypes;
    _trains = sparams._trains;
    _stationFrom = sparams._stationFrom;
    _stationTo = sparams._stationTo;
    _tripDate = sparams._tripDate;
}


QVector<QString>& SearchParameters::setTrains()
{
    return _trains;
}

const QVector<QString>& SearchParameters::getTrains() const
{
    return _trains;
}

QVector<QString>& SearchParameters::setCoachTypes()
{
    return _coachTypes;
}

const QVector<QString>& SearchParameters::getCoachTypes() const
{
    return _coachTypes;
}


void SearchParameters::setTripDate(QDate tripdate)
{
    _tripDate = tripdate;
}

QDate SearchParameters::getTripDate()
{
    return _tripDate;
}


void SearchParameters::setStations(QString stationFrom, QString stationTo)
{
    _stationFrom = stationFrom;
    _stationTo = stationTo;
}

void SearchParameters::setStationFrom(QString stationFrom)
{
    _stationFrom = stationFrom;
}

void SearchParameters::setStationTo(QString stationTo)
{
    _stationTo = stationTo;
}


QString SearchParameters::stationFrom()
{
    return _stationFrom;
}


QString SearchParameters::stationTo()
{
    return _stationTo;
}

void SearchParameters::setStationsName(QString stationFrom, QString stationTo)
{
    _stationFromName = stationFrom;
    _stationToName = stationTo;
}

QString SearchParameters::stationFromName()
{
    return _stationFromName;
}

QString SearchParameters::stationToName()
{
    return _stationToName;
}
