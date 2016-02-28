#include "windowpages.h"
#include "lineedit.h"
#include "requestdata.h"
#include "uzapplication.h"
#include "networkmanager.h"
#include "searchparameters.h"

#include "widgetsmediator.h"
#include <memory>

#include <QGridLayout>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QPushButton>
#include <QTextBrowser>

#include <QRadioButton>
#include <QGroupBox>
#include <QCheckBox>
#include <algorithm>

#include <QLabel>
#include <QFile>

const QByteArray searchRequest = "searchRequest";
const QByteArray coachRequest = "coachRequest";
const QByteArray coachesRequest = "coachesRequest";


BrowserPage::BrowserPage(WidgetsMediator* widgetsMediator,QWidget *parent)
    :BasePage(widgetsMediator),QWidget(parent)
{
    editFrom = new LineEdit(this);
    editTo = new LineEdit(this);

    calendar = new QCalendarWidget(this);
    calendar->hide();
    dateField = new QDateEdit(QDate::currentDate(),this);
    dateField->setDisplayFormat("MMM d yyyy");
    searchButton = new QPushButton("Search",this);
    showSettingsButton = new QPushButton("Налаштування пошуку",this);


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

    connect(searchButton,&QPushButton::clicked,this,&BrowserPage::ticketsSearch);
    connect(textBrowser,&QTextBrowser::anchorClicked,this,&BrowserPage::processTrain);
    connect(showSettingsButton,&QPushButton::clicked,this,&BrowserPage::showSettings);

}

void BrowserPage::showSettings()
{

    std::shared_ptr<SearchParameters> sParams = std::make_shared<SearchParameters>(editFrom->getStationID(),editTo->getStationID(),dateField->date());
    sParams->setStationsName(editFrom->text(),editTo->text());
    mediator()->setSearchParameters(sParams);
    mediator()->showSettingPage();

}


void BrowserPage::ticketsSearch()
{
    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    QString date = dateField->date().toString("MM.dd.yyyy");
    SearchPOSTData searchdata(editFrom->getStationID(),editTo->getStationID(),date);
    networkManager->sendSearchRequest(searchdata,searchRequest);
}


void BrowserPage::processTrain(const QUrl &link)
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


void BrowserPage::showAvailableTrains()
{
    QString trainData;
    textBrowser->clear();


    QFile styleF;

    styleF.setFileName("/css/style.css");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();


textBrowser->setStyleSheet(qssStr);
    Trains trains = UZApplication::instance()->trains();

   // trainData="<style> table, th, td {   border: 1px solid black; color:red;  border-collapse: collapse;   }            </style>" ;


    trainData =trainData+ "<table>";
    //textBrowser->append(trainData);

    for(auto train = trains.begin();train!=trains.end();++train)
    {
        trainData = trainData + "<tr>";
        trainData = trainData + "<td> <a href=\"" + train->number + "\">" +  train->number + "</a> </td>";
        trainData = trainData + "<td>" + train->travelTime + "</td>";
        for(auto tickets = train->freePlaces.begin();tickets!=train->freePlaces.end(); ++tickets)
        {
            trainData = trainData + "<td>" + tickets->placeClass  + ": " + QString::number(tickets->placesNumber) + "</td>";
        }
        trainData = trainData + "</tr>";
        //textBrowser->append(trainData);
    }
    trainData = trainData + "</table>";
    textBrowser->append(trainData);
    textBrowser->append(" ");
    qDebug()<<trainData;
}



void BrowserPage::showAvailableCoaches(Train *train)
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



QDate BrowserPage::tripDate()
{
    return dateField->date();
}




SettingsPage::SettingsPage(WidgetsMediator *widgetsMediator, QWidget *parent)
    :QWidget(parent), BasePage(widgetsMediator)
{
    QGroupBox* trainsBox = new QGroupBox("Поїзди",this);
    allTrainsBtn = new QRadioButton("Всі",this);
    oneTrainBtn = new QRadioButton("Окремі",this);

    trainsGroupLayout = new QGridLayout;

    QVBoxLayout* trainsLayout = new  QVBoxLayout(this);
    trainsLayout->addWidget(allTrainsBtn);
    trainsLayout->addWidget(oneTrainBtn);
    trainsLayout->addLayout(trainsGroupLayout);
    trainsLayout->setAlignment(trainsGroupLayout,Qt::AlignRight);
    trainsBox->setLayout(trainsLayout);

    QGroupBox* coachesBox = new QGroupBox("Типи вагонів",this);
    coachesTypesLayout = new QHBoxLayout;
    coachesBox->setLayout(coachesTypesLayout);

    startSearchBtn = new QPushButton("Запуск пошуку");
    showBrowserBtn = new QPushButton("Станції");
    QHBoxLayout* buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(showBrowserBtn);
    buttonsLayout->addWidget(startSearchBtn);
    QGroupBox* buttonsBox = new QGroupBox();
    buttonsBox->setFlat(true);
    buttonsBox->setLayout(buttonsLayout);



    QVBoxLayout* pagelayout = new QVBoxLayout(this);
    //pagelayout->addLayout(trainsLayout);
    pagelayout->addWidget(trainsBox);
    pagelayout->setAlignment(trainsBox,Qt::AlignTop);
    pagelayout->addWidget(coachesBox);
    //pagelayout->addWidget(startSearchBtn);
    pagelayout->addWidget(buttonsBox);
    pagelayout->setAlignment(buttonsBox,Qt::AlignBottom);
    //pagelayout->setAlignment(startSearchBtn,Qt::AlignBottom);

    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    connect(networkManager,&NetworkManager::responseReady,this,&SettingsPage::getTrainsOnRoute);

    connect(allTrainsBtn,&QRadioButton::clicked,this,&SettingsPage::onRadioButtonClick);
    connect(oneTrainBtn,&QRadioButton::clicked,this,&SettingsPage::onRadioButtonClick);
    connect(startSearchBtn,&QPushButton::clicked,this,&SettingsPage::startScanner);
    connect(showBrowserBtn,&QPushButton::clicked,this,&SettingsPage::showBrowser);
    exploreRout();
}

const QByteArray trainsOnRoute = "trainsOnRoute";

void SettingsPage::exploreRout()
{
    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    QDate futuredate = QDate::currentDate();
    futuredate = futuredate.addDays(24);

    SearchPOSTData searchdata(mediator()->getStationIDFrom(),mediator()->getStationIDTo(),futuredate.toString("MM.dd.yyyy"));

    networkManager->sendSearchRequest(searchdata,trainsOnRoute);

}


void SettingsPage::getTrainsOnRoute(QNetworkReply *reply, QByteArray id)
{
    if (reply==nullptr) return;

    if (id == trainsOnRoute)
    {
        Trains allTrainsOnRoute;
        UZApplication::instance()->parseSearchResults(reply,allTrainsOnRoute);
        //for(auto train = allTrainsOnRoute.begin();train!=allTrainsOnRoute.end();++train)
        int i = 0; int j = 0;
        QVector<QString> uniqueTypes;
        for(auto train : allTrainsOnRoute)
        {
            //qDebug()<<train.number<<"  "<<typeid(train).name();

            QCheckBox* box = new QCheckBox(train.number);
            box->setEnabled(false);
            trainsGroup.push_back(box);
            trainsGroupLayout->addWidget(box,i,j);

            for(auto&& placeType: train.freePlaces)
                uniqueTypes.push_back(placeType.placeClass);


            ++j;
            if (j>3){
                j=0; ++i;
            }

        }
        std::sort(uniqueTypes.begin(),uniqueTypes.end());
        auto pLast = std::unique(uniqueTypes.begin(),uniqueTypes.end());
        uniqueTypes.erase(pLast,uniqueTypes.end());
        for(auto&& coachType: uniqueTypes)
        {
            QCheckBox* box2 = new QCheckBox(coachType);
            box2->setChecked(true);
            coachesTypesLayout->addWidget(box2);
            coachesTypes.push_back(box2);
        }

    }

}


void SettingsPage::onRadioButtonClick()
{
    if (allTrainsBtn->isChecked())
        foreach (auto checkbox, trainsGroup) {
            checkbox->setEnabled(false);
            checkbox->setChecked(true);
        }
    else
     if (oneTrainBtn->isChecked())
        foreach (auto checkbox, trainsGroup) {
            checkbox->setEnabled(true);
        }
}



void SettingsPage::startScanner()
{

    for(auto trains: trainsGroup)
        if (trains->isChecked())
            mediator()->searchParameters->setTrains().push_back(trains->text());
    for(auto coach: coachesTypes)
        if (coach->isChecked())
            mediator()->searchParameters->setCoachTypes().push_back(coach->text());
    //temp.restart or not searching?
        std::unique(mediator()->searchParameters->setTrains().begin(),mediator()->searchParameters->setTrains().end());
        std::unique(mediator()->searchParameters->setCoachTypes().begin(),mediator()->searchParameters->setCoachTypes().end());
    UZApplication::instance()->startScanning(mediator()->searchParameters);
     mediator()->showProcessingPage();
}

void SettingsPage::showBrowser()
{
    mediator()->showBrowserPage();
}



ProcessingPage::ProcessingPage(WidgetsMediator* widgetsMediator,QWidget* parent)
    :BasePage(widgetsMediator),QWidget(parent),searchStatus(false)
{
    infoLabel = new QLabel;
    infoLabel->setWordWrap(true);
    statusLabel = new QLabel;

    showSettingsButton = new QPushButton("Змінити налаштування пошуку");

    QVBoxLayout* pagelayout = new QVBoxLayout(this);
    pagelayout->addWidget(infoLabel);
    pagelayout->addWidget(statusLabel);
    pagelayout->addWidget(showSettingsButton);
    pagelayout->setAlignment(showSettingsButton,Qt::AlignBottom);

    connect(UZApplication::instance(),&UZApplication::updateSearchStatus,this,&ProcessingPage::setSearchStatus);
    connect(showSettingsButton,&QPushButton::clicked,this,&ProcessingPage::showSettings);

    updatePage();
}

void ProcessingPage::setSearchStatus(bool isFound)
{
    searchStatus = isFound;
    if (searchStatus)
        statusLabel->setText("Знайдено");
    else
        statusLabel->setText("Пошук");
}

void ProcessingPage::showSettings()
{
    mediator()->showSettingPage();
}

void ProcessingPage::updatePage()
{
    QString info = "Пошук залізничних квитків між станціями" + mediator()->getStationFrom() + " - " + mediator()->getStationTo() +
                    ", для поїздів: ";
    for(auto& num: mediator()->getChosenTrains())
        info = info + num +  ",";
    info = info + "Дата відправлення " +mediator()->tripDate().toString();

    infoLabel->setText(info);
    setSearchStatus(searchStatus);
}
