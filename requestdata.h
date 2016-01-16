#ifndef REQUESTDATA_H
#define REQUESTDATA_H

#include <QString>

struct SearchData{
    SearchData(QString from,QString to,QString date)
        :stationFrom(from),stationTo(to),tripDate(date) {}

    QString stationFrom;
    QString stationTo;
    QString tripDate;
};


#endif // REQUESTDATA_H

