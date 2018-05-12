#ifndef REQUESTDATA_H
#define REQUESTDATA_H

#include <QString>
#include <QMap>

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


struct AddToCartPOSTData{
    AddToCartPOSTData() = default;

    AddToCartPOSTData(QString from,QString to,QString date,QString trainNum,QString type,QString num)
        :stationFrom(from),stationTo(to),tripDate(date),train(trainNum),coachType(type),coachNum(num) {}


    QString stationFrom;
    QString stationTo;
    QString tripDate;
    QString train;
    QString coachNum;
    QString coachType;
    QString firstName;
    QString lastName;
    QString placeNum;
};




//This class wraps enum only
//QMap and functions need for convertation from enum to string types and back.
class RequestType{
public:
    RequestType(){
        fillRequestTypes();
    }

    enum Request {
        SearchRequest,   //search available tickets beetween stations
        ScanRequest,     //periodic SearchRequest with checking
        CoachRequest,    //get available places of the coach
        CoachesRequest,  //get coaches of special type (with number of free places)
        TrainsOnRoute,    //the same as SearchRequest, but doesn't update browser and current trains
        GetStationsFrom,
        GetStationsTo,
        AddToCart,

        Invalid
    };

    static Request getRequestTypeByString(QByteArray identifier){
         return requestTypes.key(identifier);
    }

    static QByteArray getStringByRequestType(Request identifier) {
        return requestTypes[identifier];
    }

private:
    static QMap<RequestType::Request,QByteArray> requestTypes;

    void fillRequestTypes(){
        requestTypes.insert(RequestType::SearchRequest,"searchRequest");
        requestTypes.insert(RequestType::ScanRequest,"scanRequest");
        requestTypes.insert(RequestType::CoachRequest,"coachRequest");
        requestTypes.insert(RequestType::CoachesRequest,"coachesRequest");
        requestTypes.insert(RequestType::TrainsOnRoute,"trainsOnRoute");
        requestTypes.insert(RequestType::GetStationsFrom,QByteArray("fr"));
        requestTypes.insert(RequestType::GetStationsTo,"to");

        requestTypes.insert(RequestType::Invalid,"invalid");
    }
};



#endif // REQUESTDATA_H

