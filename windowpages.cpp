#include "windowpages.h"
#include "lineedit.h"
#include "requestdata.h"
#include "uzapplication.h"
#include "networkmanager.h"

#include <QGridLayout>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QPushButton>
#include <QTextBrowser>

const QByteArray searchRequest = "searchRequest";
const QByteArray coachRequest = "coachRequest";
const QByteArray coachesRequest = "coachesRequest";


TrainSearchPage::TrainSearchPage(QWidget *parent)
    :QWidget(parent)
{
    editFrom = new LineEdit(this);
    editTo = new LineEdit(this);

    calendar = new QCalendarWidget(this);
    calendar->hide();
    dateField = new QDateEdit(QDate::currentDate(),this);
    dateField->setDisplayFormat("MMM d yyyy");
    searchButton = new QPushButton("Search",this);
    showSettingsButton = new QPushButton("Prepare",this);


    textBrowser = new QTextBrowser(this);

    //QWidget *pageWidget = new QWidget;

    QGridLayout *pagelayout = new QGridLayout;
    pagelayout->addWidget(editFrom,0,0);
    pagelayout->addWidget(editTo,0,2);
    pagelayout->addWidget(dateField,1,0);
    pagelayout->addWidget(searchButton,2,1);
    pagelayout->addWidget(textBrowser,3,0,3,3);
    pagelayout->addWidget(showSettingsButton,6,0,3,3);

    setLayout(pagelayout);

    connect(searchButton,&QPushButton::clicked,this,&TrainSearchPage::ticketsSearch);
    connect(textBrowser,&QTextBrowser::anchorClicked,this,&TrainSearchPage::processTrain);

}


void TrainSearchPage::ticketsSearch()
{
    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    QString date = dateField->date().toString("MM.dd.yyyy");
    SearchPOSTData searchdata(editFrom->getStationID(),editTo->getStationID(),date);
    //searchReply =
            networkManager->sendSearchRequest(searchdata,searchRequest);
}


void TrainSearchPage::processTrain(const QUrl &link)
{
     qDebug()<<"clicked "<<link;
     QString trainNum = link.toString();
    // Train* currentTrain = &(*trains)[trainNum];
     const Trains trains = UZApplication::instance()->trains();
     const Train* currentTrain = &trains[trainNum];
     NetworkManager* networkManager = UZApplication::instance()->networkManager();

     for(auto p = currentTrain->freePlaces.begin();p!=currentTrain->freePlaces.end();++p)
     {
        CoachesPOSTData postdata(editFrom->getStationID(),editTo->getStationID(),currentTrain->dateDeparture,
                              trainNum,p->placeClass);
        networkManager->sendCoachesRequest(postdata,coachesRequest);
     }

}


void TrainSearchPage::showAvailableTrains()
{
    QString trainData;
    textBrowser->clear();

    Trains trains = UZApplication::instance()->trains();

    for(auto train = trains.begin();train!=trains.end();++train)
    {
        trainData = "<a href=\"" + train->number + "\">" +  train->number +"</a>";
        trainData = trainData + " \t" + train->travelTime;
        for(auto tickets = train->freePlaces.begin();tickets!=train->freePlaces.end(); ++tickets)
        {
            trainData = trainData + "\t" + tickets->placeClass  + ": " + QString::number(tickets->placesNumber);
        }
        textBrowser->append(trainData);
    }

}



void TrainSearchPage::showAvailableCoaches(Train *train)
{
 //   qDebug()<<"run ";
    if (!train->checkComleteness()) return;
//qDebug()<<"Run ";

    QString data;
    textBrowser->clear();

    textBrowser->append("<span>Type \t   №   Qty</span>\n\n");

    for(auto type = train->freePlaces.begin(); type!=train->freePlaces.end();++type)
    {
        //textBrowser->append(type->placeClass);
        textBrowser->append("<span>" + type->placeClass + "</span>");
        for(auto p = train->coaches.begin();p!= train->coaches.end(); ++p)
        {
            if (p->coachClass == type->placeClass)
            {
                data = "\t" + QString::number(p->number)+ ":    " + QString::number(p->placesNumber);
                data ="           " + QString::number(p->number)+ ":           " + QString::number(p->placesNumber);
                //qDebug()<< p->number + "     " + p->placesNumber;
                textBrowser->append("<span>" + data + "</span>");
            }
        }
    }

}
