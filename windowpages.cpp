#include "windowpages.h"
#include "lineedit.h"
#include "requestdata.h"
#include "uzapplication.h"
#include "networkmanager.h"
#include "searchparameters.h"

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

const LineEdit* TrainSearchPage::fromEdit() const
{
    return editFrom;
}

const LineEdit* TrainSearchPage::toEdit() const
{
    return editTo;
}

QDate TrainSearchPage::tripDate()
{
    return dateField->date();
}





ScannerPage::ScannerPage(TrainSearchPage* trainsSearchPage,QWidget *parent)
    :QWidget(parent), searchConfiguration(trainsSearchPage)
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

    QVBoxLayout* pagelayout = new QVBoxLayout(this);
    //pagelayout->addLayout(trainsLayout);
    pagelayout->addWidget(trainsBox);
    pagelayout->setAlignment(trainsBox,Qt::AlignTop);
    pagelayout->addWidget(coachesBox);
    pagelayout->addWidget(startSearchBtn);
    pagelayout->setAlignment(startSearchBtn,Qt::AlignBottom);

    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    connect(networkManager,&NetworkManager::responseReady,this,&ScannerPage::getTrainsOnRoute);

    connect(allTrainsBtn,&QRadioButton::clicked,this,&ScannerPage::onRadioButtonClick);
    connect(oneTrainBtn,&QRadioButton::clicked,this,&ScannerPage::onRadioButtonClick);
    connect(startSearchBtn,&QPushButton::clicked,this,&ScannerPage::startScanner);
}

const QByteArray trainsOnRoute = "trainsOnRoute";

void ScannerPage::exploreRout()
{
    const LineEdit* from = searchConfiguration->fromEdit();
    const LineEdit* to = searchConfiguration->toEdit();
    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    QDate futuredate = searchConfiguration->tripDate();
    //futuredate = futuredate.addMonths(1);
    futuredate = futuredate.addDays(24);

    SearchPOSTData searchdata(from->getStationID(),to->getStationID(),futuredate.toString("MM.dd.yyyy"));

    networkManager->sendSearchRequest(searchdata,trainsOnRoute);

}


void ScannerPage::getTrainsOnRoute(QNetworkReply *reply, QByteArray id)
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


void ScannerPage::onRadioButtonClick()
{
    if (allTrainsBtn->isChecked())
        foreach (auto checkbox, trainsGroup) {
            checkbox->setEnabled(false);
        }
    else
     if (oneTrainBtn->isChecked())
        foreach (auto checkbox, trainsGroup) {
            checkbox->setEnabled(true);
        }
}



void ScannerPage::startScanner()
{
    const LineEdit* from = searchConfiguration->fromEdit();
    const LineEdit* to = searchConfiguration->toEdit();
    //share_ptr ??
    SearchParameters searchparams(from->getStationID(),to->getStationID(),searchConfiguration->tripDate());
    searchparams.setStationsName(from->text(),to->text());
    for(auto trains: trainsGroup)
        if (trains->isChecked())
            searchparams.setTrains().push_back(trains->text());
    for(auto coach: coachesTypes)
        if (coach->isChecked())
            searchparams.setCoachTypes().push_back(coach->text());
    UZApplication::instance()->startScanning(searchparams);
}



ProcessingPage::ProcessingPage(SearchParameters searchparams,QWidget* parent)
    :QWidget(parent)
{
    infoLabel = new QLabel;
    statusLabel = new QLabel;

    QString info = "Пошук залізничних квитків між станціями" + searchparams.stationFromName() + " - " + searchparams.stationToName() +
                    ", для поїздів: ";
    for(auto& num: searchparams.getTrains())
        info = info + num +  ",";
    info = info + "Дата відправлення " + searchparams.getTripDate().toString();

    infoLabel->setText(info);


    QVBoxLayout* pagelayout = new QVBoxLayout(this);
    pagelayout->addWidget(infoLabel);
    pagelayout->addWidget(statusLabel);
}
