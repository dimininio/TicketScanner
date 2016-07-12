#include "parser.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


bool Parser::parseSearchResults(QByteArray &data, Trains &trainsContainer)
{
    QDateTime datetime;

    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);
    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();

         QJsonArray jsonTrains = jsonobject["value"].toArray();
         QJsonObject jsonTrain;
         for(auto it = jsonTrains.begin();it != jsonTrains.end();++it)
         {
             jsonTrain = it->toObject();
             QString trainNumber = jsonTrain["num"].toString();
             Train train(trainNumber);
             train.travelTime = jsonTrain["travel_time"].toString();

             datetime.setTime_t((jsonTrain["from"].toObject())["date"].toInt());
             train.dateDeparture = datetime;
             datetime.setTime_t((jsonTrain["till"].toObject())["date"].toInt());
             train.dateArrival = datetime;

             QJsonArray ticketTypes = jsonTrain["types"].toArray();
             QJsonObject ticketType;
             for(auto it2 = ticketTypes.begin(); it2!=ticketTypes.end();++it2)
             {
                 ticketType = it2->toObject();
                 train.freePlaces.push_back(FreePlaces(ticketType["letter"].toString(),ticketType["places"].toInt()));
            // tempstr = tempstr + ticketType["letter"].toString()+ "    "+ QString::number(ticketType["places"].toInt()) + ",    ";
             }
             //qDebug()<<tempstr;
             trainsContainer.insert(trainNumber,train);
         }

        if (jsonobject["error"].toBool()){
            QString error = jsonobject["value"].toString();
            qDebug()<<error;
            //emit searchError(error);
            return false;
        }


    }

    return true;
}

bool Parser::parseCoachesSearchResults(QByteArray &data, Train &trainsContainer)
{
    return true;
}

bool Parser::parseStations(QByteArray &data, QStringList &stations)
{
    return true;
}
