#include "parser.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

#define _PREPARATION_FOR_TESTS_
#undef _PREPARATION_FOR_TESTS_


void CreateJsonFileForTest(QByteArray &data,QString filename)
{
#if    defined(_PREPARATION_FOR_TESTS_) &&  defined(QT_DEBUG)
    QFile file("../tests/" + filename); //save file to the tests project
    file.close();
    file.open( QIODevice::WriteOnly);
    file.write(data);
    file.close();
#endif
}


bool Parser::parseSearchResults(QByteArray &data, Trains &trainsContainer, QString &error)
{
    QDateTime datetime;

    CreateJsonFileForTest(data,"test_SearchReply.json");


    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);
    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();
        QJsonObject jsonData = jsonobject["data"].toObject();




         QJsonArray jsonTrains = jsonData["list"].toArray();
         QJsonObject jsonTrain;
         for(auto it = jsonTrains.begin();it != jsonTrains.end();++it)
         {
             jsonTrain = it->toObject();
             QString trainNumber = jsonTrain["num"].toString();
             Train train(trainNumber);
             train.travelTime = jsonTrain["travelTime"].toString();

             datetime.setTime_t((jsonTrain["from"].toObject())["sortTime"].toInt());
             train.dateDeparture = datetime;
             datetime.setTime_t((jsonTrain["to"].toObject())["sortTime"].toInt());
             train.dateArrival = datetime;

             train.stationDeparture = (jsonTrain["from"].toObject())["station"].toString();
             train.stationArrival = (jsonTrain["to"].toObject())["station"].toString();

             QJsonArray ticketTypes = jsonTrain["types"].toArray();
             QJsonObject ticketType;
             for(auto it2 = ticketTypes.begin(); it2!=ticketTypes.end();++it2)
             {
                 ticketType = it2->toObject();
                 train.freePlaces.push_back(FreePlaces(ticketType["letter"].toString(),ticketType["places"].toInt()));
            // tempstr = tempstr + ticketType["letter"].toString()+ "    "+ QString::number(ticketType["places"].toInt()) + ",    ";
             }
             //qDebug()<<tempstr;
             if (train.freePlaces.size() > 0)
                 trainsContainer.insert(trainNumber,train);
         }

        if (jsonobject["error"].toBool()){
            error = jsonobject["value"].toString();
            qDebug()<<error;
            CreateJsonFileForTest(data,"test_SearchErrorReply.json");
            return false;
        }


    }

    return true;
}

bool Parser::parseCoachesSearchResults(QByteArray &data, Train &train,QString coachType)
{

    CreateJsonFileForTest(data,"test_SearchCoachesReply.json");

    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);

    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();


        QJsonObject jsonData = jsonobject["data"].toObject();

         QJsonArray jsonCoaches = jsonData["wagons"].toArray();
         QJsonObject jsonCoach;
         for(auto it = jsonCoaches.begin();it != jsonCoaches.end();++it)
         {
             jsonCoach = it->toObject();
             int number = jsonCoach["num"].toInt();
             Coach coach(number);
             coach.coachTypeID = QString::number(jsonCoach["type_id"].toInt());
             coach.placesNumber = jsonCoach["free"].toInt();
             coach.coachClass = coachType;
             //qDebug()<<coachType;
             train.coaches.insert(number,coach);

          }
         return true;
    }
    else if (responce.isNull())
    {
        qDebug()<< "Wagons_Responce: Invalid JSON.";
    }
    return false;
}

bool Parser::parseStations(QByteArray &data, QMap<QString,QString> &stations)
{
    CreateJsonFileForTest(data,"test_StationReply.json");

    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);

    if (responce.isArray()) {

         QJsonArray jsonStations = responce.array();
         QJsonObject station;
         QString strID;
         QString stationName;
         int intID;

         for(auto it2 = jsonStations.begin();it2 != jsonStations.end();++it2)
         {
             station = it2->toObject();
             //sometimes ["value"] is deduced as int sometimemes as string. So use both..
             intID = station["value"].toInt();
             strID = (intID > 0)
                     ? QString::number(intID)
                     : station["value"].toString();

             stationName = station["label"].toString();
             if (stationName.isEmpty())
                 stationName = station["title"].toString();
             stations.insert(stationName, strID);

             //TODO: POINT for exception/log
         }
        return true;
    }

    return false;
}
