#include "uzapplication.h"
#include "uzmainwindow.h"
#include "networkmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QString>



static const QByteArray searchRequest = "searchRequest";
static const QByteArray coachRequest = "coachRequest";
static const QByteArray coachesRequest = "coachesRequest";


UZApplication::UZApplication(int &argc, char **argv):
    QApplication(argc,argv),
    mainWindow(nullptr)
{
    p_networkManager = new NetworkManager(this);
    connect(p_networkManager,&NetworkManager::networkManagerReady,this,&UZApplication::showWindow);
    connect(p_networkManager,&NetworkManager::responseReady,this,&UZApplication::analizeResponse);
}


void UZApplication::showWindow()
{
    if (!mainWindow) {
        mainWindow = new UZMainWindow();
        mainWindow->show();
     }
}


UZApplication* UZApplication::instance()
{
    return (static_cast<UZApplication*>(QCoreApplication::instance()));
}

UZApplication::~UZApplication()
{
    delete mainWindow;
   // p_networkManager->clearAccessCache();  //does solve the leak problem
    delete p_networkManager;
}





void UZApplication::analizeResponse(QNetworkReply *reply, QByteArray id)
{
    //switch(id) {
    //    case searchRequest: showSearchResults(reply);
   // }
    if (reply==nullptr) return;

    if (id == searchRequest)
        parseSearchResults(reply);
    if (id == coachesRequest)
        parseCoachesSearchResults(reply);
}


QString whatTrain(QNetworkReply *coachesReply)
{
    return coachesReply->request().rawHeader("Train");
}
QString whatType(QNetworkReply *coachesReply)
{
    return coachesReply->request().rawHeader("CoachType");
}


void UZApplication::parseSearchResults(QNetworkReply *reply)
{

    QByteArray data = reply->readAll();

    qDebug()<<"search result: "<<data;

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
             int dateInSeconds = (jsonTrain["from"].toObject())["date"].toInt();
             train.dateDeparture = QString::number(dateInSeconds);

             QJsonArray ticketTypes = jsonTrain["types"].toArray();
             QJsonObject ticketType;
             for(auto it2 = ticketTypes.begin(); it2!=ticketTypes.end();++it2)
             {
                 ticketType = it2->toObject();
                 train.availablePlaces.insert(ticketType["letter"].toString(),ticketType["places"].toInt());
             }
             trains.insert(trainNumber,train);
         }

        if (jsonobject["error"].toBool()){
            QString error = jsonobject["value"].toString();
            qDebug()<<error;
        }


    }

    mainWindow->showAvailableTrains();
    //reply->deleteLater();
    //delete searchReply;
}


void UZApplication::parseCoachesSearchResults(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QString trainNumber = whatTrain(reply);
    QString coachType = whatType(reply);
    Train* train = &trains[trainNumber];
    //qDebug()<<"coaches data: "<<data;

    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);
    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();

         QJsonObject jsonValue = jsonobject["value"].toObject();
         QJsonArray jsonCoaches = jsonValue["coaches"].toArray();
         QJsonObject jsonCoach;
         for(auto it = jsonCoaches.begin();it != jsonCoaches.end();++it)
         {
             jsonCoach = it->toObject();
             int number = jsonCoach["num"].toInt();
             Coach coach(number);
             coach.coachTypeID = QString::number(jsonCoach["coach_type_id"].toInt());
             coach.placesNumber = jsonCoach["places_cnt"].toInt();
             coach.coachClass = coachType;
             train->coaches.insert(number,coach);
             //qDebug()<<jsonCoach["num"];

          }
    }

    mainWindow->showAvailableCoaches(train);
}




NetworkManager*  UZApplication::networkManager()
{
    return p_networkManager;
}
