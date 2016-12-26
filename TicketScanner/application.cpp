#include "application.h"
#include "uzmainwindow.h"
#include "networkmanager.h"
#include "searchparameters.h"
#include "config.h"
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
#include <QSplashScreen>
#include <QKeyEvent>
#include <QDesktopServices>
#include "parser.h"


NetworkManager* Application::p_networkManager = nullptr;


Application::Application(int &argc, char **argv):
    QApplication(argc,argv),
    mainWindow(nullptr),searchParameters(nullptr),p_trains(nullptr),p_scanTrains(nullptr),
    timer(nullptr),p_interval(100000) //1:40 min
{
    splashScreen = new QSplashScreen(QPixmap(":/resources/splash.jpg"));
    splashScreen->show(); //need tests: destroing while errors

    p_networkManager = new NetworkManager(this);
    p_trains = new Trains();

    windowWrapper = new WindowWrapper(true,true);

    QFile styleF;
    styleF.setFileName(":/resources/widgetstyles.qss");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();
    setStyleSheet(qssStr);

    connect(p_networkManager,&NetworkManager::networkManagerReady,this,&Application::showWindow);
    connect(p_networkManager,&NetworkManager::responseReady,this,&Application::analizeResponse);
    connect(windowWrapper,&WindowWrapper::closeWrapper,this,&Application::quit);
    connect(windowWrapper,&WindowWrapper::helpWrapper,this,&Application::showHelp);

    RequestType requestType; //initialization of requests' types

    installEventFilter(this);
}


void Application::showWindow()
{
    if (!mainWindow) {
        mainWindow = new UZMainWindow();

        windowWrapper->setMainWidget(mainWindow);
        windowWrapper->show();
        splashScreen->finish(windowWrapper);
        //mainWindow->show();
     }
}


Application* Application::instance()
{
    return (static_cast<Application*>(QCoreApplication::instance()));
}

Application::~Application()
{
    delete mainWindow;
    delete p_networkManager;
    delete p_trains;
    if (p_scanTrains)
        delete p_scanTrains;
}





void Application::analizeResponse(QNetworkReply *reply, RequestType::Request id)
{
    if (reply==nullptr) return;

    switch(id) {
        case RequestType::SearchRequest:

                if(parseSearchResults(reply,*p_trains))
                    mainWindow->showAvailableTrains();
                break;

        case RequestType::CoachesRequest:

                parseCoachesSearchResults(reply);
                break;

        case RequestType::ScanRequest:               
                parseSearchResults(reply,*p_scanTrains);
                if (checkScanningResults())
                {
                    qDebug()<<"FOUND..";
                    setStatus(SearchStatus::Found);
                    //setActiveWindow(mainWindow);
                    //mainWindow->activateWindow();
                    alert(mainWindow,0); //causes flash of icon on taskbar (if window is not active)
                    timer->stop();
                    delete timer;timer = nullptr;
                    QSound::play(":/resources/arfa.wav");
                    searchStatus = SearchStatus::Waiting; //to be opened for new searches
                }
                else
                {
                    setStatus(SearchStatus::Search);
                }
                break;

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


bool Application::parseSearchResults(QNetworkReply *reply,Trains& trainsContainer)
{

    QByteArray data = reply->readAll();
    QString errors = "";

    bool parseResult = Parser::parseSearchResults(data,trainsContainer,errors);

    if (!errors.isEmpty())
       emit searchError(errors);

    return parseResult;


    //reply->deleteLater();
    //delete searchReply;
}


bool Application::parseCoachesSearchResults(QNetworkReply *reply )
{
    QByteArray data = reply->readAll();
    QString trainNumber = whatTrain(reply);
    QString coachType = whatType(reply);
    Train* train = setTrain(trainNumber);
    //qDebug()<<"coaches data: "<<data;

    Parser::parseCoachesSearchResults(data,*train,coachType); {
        mainWindow->showAvailableCoaches(train);
        return true;
    }

    return true;
}



void Application::startScanning(std::shared_ptr<SearchParameters>& parameters)
{
    searchParameters = parameters;
    qDebug()<<searchParameters->stationTo()<< "    check search";
    if (!timer) {
        timer = new QTimer(this);
        connect(timer,&QTimer::timeout,this,&Application::sendScanRequest);
        timer->start(p_interval);
    }
    if (p_scanTrains)
        delete p_scanTrains;
    p_scanTrains = new Trains();

    setStatus(SearchStatus::Search);
}



void Application::sendScanRequest()
{
    QString date = searchParameters->getTripDate().toString(Config::RequestDateFormat);
    SearchPOSTData searchdata(searchParameters->stationFrom(),searchParameters->stationTo(),date);
    p_networkManager->sendSearchRequest(searchdata,RequestType::ScanRequest);
}


bool Application::checkScanningResults()
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


NetworkManager*  Application::networkManager()
{
    return p_networkManager;
}


const Trains* Application::trains() const
{
    return p_trains;
}


Trains* Application::setTrains()
{
    return p_trains;
}

Train *Application::setTrain(QString number)
{
    return  &p_trains->operator [](number);
}

const Train *Application::getTrain(QString number) const
{
    return  &p_trains->operator [](number);
}



void Application::setStatus(Application::SearchStatus status)
{
    searchStatus = status;
    emit updateSearchStatus(status);
}


Application::SearchStatus Application::status()
{
    return searchStatus;
}


void Application::resetTrains()
{
    if (p_trains)
    {
        p_trains->clear();
    }
    if (p_scanTrains)
    {
        p_scanTrains->clear();
    }
}



bool Application::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key()==Qt::Key_F1)
        {
            showHelp();
            return true;
        }
        else  {
            return QApplication::eventFilter(obj,event);
        }
    }
    else{
        return QApplication::eventFilter(obj,event);
    }


}


void Application::showHelp()
{
    bool helpRequest = QDesktopServices::openUrl(QUrl(Config::HelpURL));
}






