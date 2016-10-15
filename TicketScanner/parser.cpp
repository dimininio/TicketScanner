#include "parser.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

#define _PREPARATION_FOR_TESTS_
//#undef _PREPARATION_FOR_TESTS_

void CreateJsonFileForTest(QByteArray &data,QString filename)
{
    QFile file("../tests/" + filename); //save file to the tests project
    file.close();
    file.open( QIODevice::WriteOnly);
    file.write(data);
    file.close();
}


bool Parser::parseSearchResults(QByteArray &data, Trains &trainsContainer)
{
    QDateTime datetime;
#ifdef    _PREPARATION_FOR_TESTS_
    CreateJsonFileForTest(data,"test_SearchReply.json");
#endif

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

             train.stationDeparture = (jsonTrain["from"].toObject())["station"].toString();
             train.stationArrival = (jsonTrain["till"].toObject())["station"].toString();

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

bool Parser::parseCoachesSearchResults(QByteArray &data, Train &train,QString coachType)
{

#ifdef    _PREPARATION_FOR_TESTS_
    CreateJsonFileForTest(data,"test_SearchCoachesReply.json");
#endif


    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);
    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();

         //QJsonObject jsonValue = jsonobject["value"].toObject();
         //QJsonArray jsonCoaches = jsonValue["coaches"].toArray();
         QJsonArray jsonCoaches = jsonobject["coaches"].toArray();
         QJsonObject jsonCoach;
         for(auto it = jsonCoaches.begin();it != jsonCoaches.end();++it)
         {
             jsonCoach = it->toObject();
             int number = jsonCoach["num"].toInt();
             Coach coach(number);
             coach.coachTypeID = QString::number(jsonCoach["coach_type_id"].toInt());
             coach.placesNumber = jsonCoach["places_cnt"].toInt();
             coach.coachClass = coachType;
             //qDebug()<<coachType;
             train.coaches.insert(number,coach);

          }
         return true;
    }
    return false;
}

bool Parser::parseStations(QByteArray &data, QMap<QString,QString> &stations)
{

#ifdef    _PREPARATION_FOR_TESTS_
    CreateJsonFileForTest(data,"test_StationReply.json");
#endif

    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);
    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();
         QJsonArray jsonStations = jsonobject["value"].toArray();
         QJsonObject station;
         for(auto it2 = jsonStations.begin();it2 != jsonStations.end();++it2)
         {
             station = it2->toObject();
             stations.insert(station["title"].toString(), station["station_id"].toString());
             //stations.insert(station["title"].toString(), QString::number(station["station_id"].toInt()));
            // qDebug()<<"-- :  "<< station["title"].toString()<<" "<<station["station_id"].toString();
         }
        return true;
    }
    return false;
}