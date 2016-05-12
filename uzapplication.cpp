#include "uzapplication.h"
#include "uzmainwindow.h"
#include "networkmanager.h"
#include "searchparameters.h"
//#include "requestdata.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QString>
#include <QTimer>
#include <algorithm>
#include <QFile>
#include <QSound>

static const QByteArray searchRequest = "searchRequest";
static const QByteArray scanRequest = "scanRequest";
static const QByteArray coachRequest = "coachRequest";
static const QByteArray coachesRequest = "coachesRequest";


NetworkManager* UZApplication::p_networkManager = 0;


UZApplication::UZApplication(int &argc, char **argv):
    QApplication(argc,argv),
    mainWindow(nullptr),searchParameters(nullptr),p_trains(nullptr),p_scanTrains(nullptr),
    timer(nullptr),p_interval(200000) //10 min
{
    p_networkManager = new NetworkManager(this);
    p_trains = new Trains();

    QFile styleF;
    styleF.setFileName(":/resources/widgetstyles.qss");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();
    setStyleSheet(qssStr);

    connect(p_networkManager,&NetworkManager::networkManagerReady,this,&UZApplication::showWindow);
    connect(p_networkManager,&NetworkManager::responseReady,this,&UZApplication::analizeResponse);

    //fillRequestTypes(); //Fill QMap for enum QRequestType
    RequestType requestType;
    qDebug()<<RequestType::getRequestType(RequestType::SearchRequest);
    qDebug()<<"fr "<<RequestType::getRequestType(RequestType::SearchRequest);

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
    delete p_networkManager;
    delete p_trains;
    if (p_scanTrains)
        delete p_scanTrains;
}





void UZApplication::analizeResponse(QNetworkReply *reply, RequestType::Request id)
{
    //switch(id) {
    //    case searchRequest: showSearchResults(reply);
   // }
    if (reply==nullptr) return;

    switch(id) {
        case RequestType::SearchRequest:

                if(parseSearchResults(reply,*p_trains))
                    mainWindow->showAvailableTrains();

        case RequestType::CoachesRequest:

                parseCoachesSearchResults(reply);

        case RequestType::ScanRequest:

                parseSearchResults(reply,*p_scanTrains);
                if (checkScanningResults())
                {
                    qDebug()<<"FOUND..";
                    setStatus(SearchStatus::Found);
                    //setActiveWindow(mainWindow);
                    mainWindow->activateWindow();
                    timer->stop();
                    QSound::play(":/resources/arfa.wav");
                }
    }

/*
    if (id == searchRequest) {
        if(parseSearchResults(reply,*p_trains))
            mainWindow->showAvailableTrains();
    }
    if (id == coachesRequest){
        parseCoachesSearchResults(reply);
    }
    if (id == scanRequest){
        parseSearchResults(reply,*p_scanTrains);
        if (checkScanningResults())
        {
            qDebug()<<"FOUND..";
            setStatus(SearchStatus::Found);
            //setActiveWindow(mainWindow);
            mainWindow->activateWindow();
            timer->stop();
            QSound::play(":/resources/arfa.wav");
        }
    }

    */
}


QString whatTrain(QNetworkReply *coachesReply)
{
    return coachesReply->request().rawHeader("Train");
}
QString whatType(QNetworkReply *coachesReply)
{
    return coachesReply->request().rawHeader("CoachType");
}


bool UZApplication::parseSearchResults(QNetworkReply *reply,Trains& trainsContainer)
{

    QByteArray data = reply->readAll();

    //qDebug()<<"search result: "<<data;
    trainsContainer.clear();
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
            emit searchError(error);
            return false;
        }


    }

    return true;
    //reply->deleteLater();
    //delete searchReply;
}


bool UZApplication::parseCoachesSearchResults(QNetworkReply *reply )
{
    QByteArray data = reply->readAll();
    QString trainNumber = whatTrain(reply);
    QString coachType = whatType(reply);
    Train* train = setTrain(trainNumber);
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
    return true;
}



void UZApplication::startScanning(std::shared_ptr<SearchParameters>& parameters)
{
    searchParameters = parameters;
    qDebug()<<searchParameters->stationTo()<< "    check search";
    if (!timer) {
        timer = new QTimer(this);
        connect(timer,&QTimer::timeout,this,&UZApplication::sendScanRequest);
        timer->start(p_interval);
    }
    if (p_scanTrains)
        delete p_scanTrains;
    p_scanTrains = new Trains();

    setStatus(SearchStatus::Search);
}


void UZApplication::sendScanRequest()
{
    QString date = searchParameters->getTripDate().toString("MM.dd.yyyy");
    SearchPOSTData searchdata(searchParameters->stationFrom(),searchParameters->stationTo(),date);
    p_networkManager->sendSearchRequest(searchdata,RequestType::ScanRequest);
}


bool UZApplication::checkScanningResults()
{
    //Ukrazaliznitsya doesn't have public request to get all possible trains between stations.
    //Now we check two dates for existing trains, but it cannot guarantee that available trains
    //are all possible existing trains.
    //So, we should to check trains by other way, if user choose "search for any train".
    if (searchParameters->searchForAnyTrain()) {
        //just check all available trains
        for(auto&& train = p_scanTrains->begin();train!=p_scanTrains->end();++train){
                for(auto& placeType:train->freePlaces)
                    for (auto& appropriatePlace:searchParameters->getCoachTypes() )
                        if (appropriatePlace==placeType.placeClass)
                            return true;
        }

    }else {
        for(auto&& rightTrain = searchParameters->getTrains().begin(); rightTrain!=searchParameters->getTrains().end();++rightTrain)
        {
            //compare available trains with selected trains
            auto train = std::find_if(p_scanTrains->begin(),p_scanTrains->end(),[&rightTrain](Train trn){return trn.number==rightTrain;});
            if (train!=p_scanTrains->end()) {
                for(auto& placeType:train->freePlaces)
                    for (auto& appropriatePlace:searchParameters->getCoachTypes() )
                        if (appropriatePlace==placeType.placeClass)
                            return true;
            }
        }
    }
    return false;
}


NetworkManager*  UZApplication::networkManager()
{
    return p_networkManager;
}


const Trains* UZApplication::trains() const
{
    return p_trains;
}


Trains* UZApplication::setTrains()
{
    return p_trains;
}

Train *UZApplication::setTrain(QString number)
{
    return  &p_trains->operator [](number);
}

const Train *UZApplication::getTrain(QString number) const
{
    return  &p_trains->operator [](number);
}



void UZApplication::setStatus(UZApplication::SearchStatus status)
{
    searchStatus = status;
    emit updateSearchStatus(status);
}


UZApplication::SearchStatus UZApplication::status()
{
    return searchStatus;
}








