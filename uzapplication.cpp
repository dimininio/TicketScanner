#include "uzapplication.h"
#include "uzmainwindow.h"
#include "networkmanager.h"
#include "searchparameters.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QString>
#include <QTimer>
#include <algorithm>


static const QByteArray searchRequest = "searchRequest";
static const QByteArray scanRequest = "scanRequest";
static const QByteArray coachRequest = "coachRequest";
static const QByteArray coachesRequest = "coachesRequest";


UZApplication::UZApplication(int &argc, char **argv):
    QApplication(argc,argv),
    mainWindow(nullptr),
    p_interval(10000) //10 min
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
    if (!searchParameters)
        delete searchParameters;
    delete p_networkManager;
}





void UZApplication::analizeResponse(QNetworkReply *reply, QByteArray id)
{
    //switch(id) {
    //    case searchRequest: showSearchResults(reply);
   // }
    if (reply==nullptr) return;

    if (id == searchRequest) {
        parseSearchResults(reply,p_trains);
        mainWindow->showAvailableTrains();
    }
    if (id == coachesRequest){
        parseCoachesSearchResults(reply);       
    }
    if (id == scanRequest){
        parseSearchResults(reply,scan_trains);
        if (checkScanningResults()) qDebug()<<"FOUND..";
    }
}


QString whatTrain(QNetworkReply *coachesReply)
{
    return coachesReply->request().rawHeader("Train");
}
QString whatType(QNetworkReply *coachesReply)
{
    return coachesReply->request().rawHeader("CoachType");
}


void UZApplication::parseSearchResults(QNetworkReply *reply,Trains& trainsContainer)
{

    QByteArray data = reply->readAll();

    qDebug()<<"search result: "<<data;
    trainsContainer.clear();

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
                 train.freePlaces.push_back(FreePlaces(ticketType["letter"].toString(),ticketType["places"].toInt()));
             }
             trainsContainer.insert(trainNumber,train);
         }

        if (jsonobject["error"].toBool()){
            QString error = jsonobject["value"].toString();
            qDebug()<<error;
        }


    }


    //reply->deleteLater();
    //delete searchReply;
}


void UZApplication::parseCoachesSearchResults(QNetworkReply *reply )
{
    QByteArray data = reply->readAll();
    QString trainNumber = whatTrain(reply);
    QString coachType = whatType(reply);
    Train* train = &p_trains[trainNumber];
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

          }
    }
    mainWindow->showAvailableCoaches(train);

}



void UZApplication::startScanning(SearchParameters searchparams)
{

    searchParameters = new SearchParameters(searchparams);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&UZApplication::sendScanRequest);
    timer->start(p_interval);

}

void UZApplication::sendScanRequest()
{
    QString date = searchParameters->getTripDate().toString("MM.dd.yyyy");
    SearchPOSTData searchdata(searchParameters->stationFrom(),searchParameters->stationTo(),date);
    p_networkManager->sendSearchRequest(searchdata,scanRequest);
}

bool UZApplication::checkScanningResults()
{

    for(auto rightTrain = searchParameters->getTrains().begin(); rightTrain!=searchParameters->getTrains().end();++rightTrain)
    {
        auto train = std::find_if(scan_trains.begin(),scan_trains.end(),[&rightTrain](Train trn){return trn.number==rightTrain;});
        if (train!=scan_trains.end()) {
            for(auto placeType:train->freePlaces)
                for (auto& rightPlace:searchParameters->getCoachTypes() )
                    if (rightPlace==placeType.placeClass)
                        return true;
        }
    }
    return false;
}


NetworkManager*  UZApplication::networkManager()
{
    return p_networkManager;
}


Trains& UZApplication::trains()
{
    return p_trains;
}

const Trains& UZApplication::trains() const
{
    return p_trains;
}


