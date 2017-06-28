#ifndef SEARCHPARAMETERS
#define SEARCHPARAMETERS

class QString;

#include <QVector>
#include <QDate>




class SearchParameters
{
public:
    SearchParameters(QString stationFrom, QString stationTo, QDate tripDate);
    SearchParameters(SearchParameters& sparams);

    QVector<QString>& setTrains();
    const QVector<QString>& getTrains() const;

    QVector<QString>& setCoachTypes();
    const QVector<QString>& getCoachTypes() const;

    void setTripDate(QDate tripdate);
    QDate getTripDate();

    void setStations(QString stationFrom,QString stationTo);      
    void setStationTo(QString stationTo);
    QString stationTo();
    void setStationFrom(QString stationFrom);
    QString stationFrom();

    void setStationsName(QString stationFrom,QString stationTo);
    QString stationFromName();
    QString stationToName();

    bool searchForAnyTrain();
    void setSearchForAnyTrain(bool anytrain);

private:
    QVector<QString> _trains =  QVector<QString>();
    QVector<QString> _coachTypes = QVector<QString>();
    QDate _tripDate;
    QString _stationFrom;
    QString _stationTo;
    QString _stationFromName;
    QString _stationToName;
    bool _searchForAnyTrain;

};

#endif // SEARCHPARAMETERS

