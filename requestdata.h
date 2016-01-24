#ifndef REQUESTDATA_H
#define REQUESTDATA_H

#include <QString>

struct SearchPOSTData{
    SearchPOSTData(QString from,QString to,QString date)
        :stationFrom(from),stationTo(to),tripDate(date) {}

    QString stationFrom;
    QString stationTo;
    QString tripDate;
};


struct CoachesPOSTData{

    CoachesPOSTData(QString from,QString to,QString date,QString trainNum,QString type)
        :stationFrom(from),stationTo(to),tripDate(date),train(trainNum),coachType(type) {}

    QString stationFrom;
    QString stationTo;
    QString tripDate;
    QString train;
    QString coachType;
};


struct CoachPOSTData{
    CoachPOSTData(QString from,QString to,QString date,QString trainNum,QString type,QString num)
        :stationFrom(from),stationTo(to),tripDate(date),train(trainNum),coachType(type),coachNum(num) {}


    QString stationFrom;
    QString stationTo;
    QString tripDate;
    QString train;
    QString coachNum;
    QString coachType;
};


#endif // REQUESTDATA_H

