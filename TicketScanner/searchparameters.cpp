#include "searchparameters.h"
#include <QString>

SearchParameters::SearchParameters(QString stationFrom, QString stationTo, QDate tripDate)
    :_stationFrom(stationFrom),_stationTo(stationTo),_tripDate(tripDate),_stationFromName(""),_stationToName("")
    ,_searchForAnyTrain(true)
{

}

SearchParameters::SearchParameters(SearchParameters &sparams)
{
    _coachTypes = sparams._coachTypes;
    _trains = sparams._trains;
    _stationFrom = sparams._stationFrom;
    _stationTo = sparams._stationTo;
    _tripDate = sparams._tripDate;
    _stationToName = sparams._stationToName;
    _stationFromName = sparams._stationFromName;
    _searchForAnyTrain = sparams._searchForAnyTrain;

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

bool SearchParameters::searchForAnyTrain()
{
    return _searchForAnyTrain;
}

void SearchParameters::setSearchForAnyTrain(bool anytrain)
{
    _searchForAnyTrain = anytrain;
}

bool SearchParameters::searchForAnyCoach()
{
    return _searchForAnyCoach;
}

void SearchParameters::setSearchForAnyCoach(bool anywagon)
{
    _searchForAnyCoach = anywagon;
}

void SearchParameters::setFirstName(QString name)
{
    _firstName = name;
}

QString SearchParameters::firstName()
{
    return _firstName;
}

void SearchParameters::setLastName(QString name)
{
    _lastName = name;
}

QString SearchParameters::lastName()
{
    return _lastName;
}
