#include "windowpages.h"
#include "lineedit.h"
#include "requestdata.h"
#include "uzapplication.h"
#include "networkmanager.h"
#include "searchparameters.h"

#include "widgetsmediator.h"
#include <memory>

#include <QGridLayout>
#include <QDateEdit>
#include <QPushButton>
#include <QWebView>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>
#include <algorithm>

#include <QLabel>
#include <QFile>

#include "animatedsearchwidget.h"

#include "uzmainwindow.h"

//#include <QUrl>

const QByteArray searchRequest = "searchRequest";
const QByteArray coachRequest = "coachRequest";
const QByteArray coachesRequest = "coachesRequest";
const QString referenceTrains = "refTrains";


BrowserPage::BrowserPage(WidgetsMediator* widgetsMediator,QWidget *parent)
    :BasePage(widgetsMediator),QWidget(parent)
{
    editFrom = new LineEdit(this);
    editTo = new LineEdit(this);

    dateField = new QDateEdit(QDate::currentDate(),this);
    dateField->setDisplayFormat("MMM d yyyy");
    dateField->setCalendarPopup(true);
    searchButton = new QPushButton("Пошук",this);
    showSettingsButton = new QPushButton("Налаштування пошуку",this);

    //QWidget *pageWidget = new QWidget;
    webView = new QWebView;

    webView->settings()->setUserStyleSheetUrl(QUrl("qrc:/resources/styles.css"));
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);


    webView->setHtml("<!DOCTYPE html><html><body></body></html>");
    webView->setMaximumWidth(400);
    webView->setMaximumHeight(250);


    QGridLayout *pagelayout = new QGridLayout;
    pagelayout->addWidget(editFrom,0,0);
    pagelayout->addWidget(editTo,0,2);
    pagelayout->addWidget(dateField,1,0);
    pagelayout->addWidget(searchButton,2,1);
    pagelayout->addWidget(webView,3,0,3,3);
    pagelayout->addWidget(showSettingsButton,6,0,1,3);

    setLayout(pagelayout);

    previousState_fromStation = "";
    previousState_toStation = "";


    connect(searchButton,&QPushButton::clicked,this,&BrowserPage::ticketsSearch);
    connect(showSettingsButton,&QPushButton::clicked,this,&BrowserPage::showSettings);
    connect(webView,&QWebView::linkClicked,this,&BrowserPage::processLink);
    connect(UZApplication::instance(),&UZApplication::searchError,this,&BrowserPage::showError);

}

void BrowserPage::showSettings()
{
    if(checkConditions()==false)
        return;


    mediator()->showSettingPage();
    saveState();
}


void BrowserPage::ticketsSearch()
{
    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    QString date = dateField->date().toString("MM.dd.yyyy");
    SearchPOSTData searchdata(editFrom->getStationID(),editTo->getStationID(),date);
    networkManager->sendSearchRequest(searchdata,NetworkManager::RequestType::SearchRequest);

}

bool BrowserPage::checkConditions()
{
    if (editFrom->text().length()==0 || editTo->text().length()==0) {
        QMessageBox msgBox;
        msgBox.setText("Оберіть станції відправлення та прибуття");
        msgBox.exec();
        return false;
    }
    return true;
}

bool BrowserPage::isChanged()
{
    if(previousState_fromStation!=editFrom->text() ||
           previousState_toStation != editTo->text())
        return true;
    else
        return false;
}

void BrowserPage::saveState()
{
    previousState_fromStation = editFrom->text();
    previousState_toStation = editTo->text();
}


void BrowserPage::processLink(const QUrl &link)
{
    if (link.toString()== referenceTrains)
        showAvailableTrains();
    else
        processTrain(link);
}


void BrowserPage::processTrain(const QUrl &link)
{
     qDebug()<<"clicked "<<link;
     QString trainNum = link.toString();
     const Train* currentTrain = UZApplication::instance()->getTrain(trainNum);

     NetworkManager* networkManager = UZApplication::instance()->networkManager();

     for(auto&& p = currentTrain->freePlaces.begin();p!=currentTrain->freePlaces.end();++p)
     {
        CoachesPOSTData postdata(editFrom->getStationID(),editTo->getStationID(),QString::number(currentTrain->dateDeparture.toTime_t()),
                              trainNum,p->placeClass);
        networkManager->sendCoachesRequest(postdata,coachesRequest);
     }

}


void BrowserPage::showAvailableTrains()
{
    QString trainData;
    const Trains* trains = UZApplication::instance()->trains();


    trainData =trainData+ "<html><body><table>";

    for(auto& train = trains->begin();train!=trains->end();++train)
    {
        trainData += "<tr>";
        trainData += "<td> <a href=\"" + train->number + "\">" +  train->number + "</a> </td>";

        trainData +=  "<td>";
        trainData += "<div class=\"date\">" + train->dateDeparture.toString("ddd dd.MM.yy") + "</div>";
        trainData += "<div class=\"date\">  " + train->dateArrival.toString("ddd dd.MM.yy") + "</div>";
        trainData +=  "</td>";
        trainData +=  "<td>";
        trainData += "<div class=\"time\">" + train->dateDeparture.toString("HH:mm") + "</div>";
        trainData += "<div class=\"time\">" + train->dateArrival.toString("HH:mm") + "</div>";
        trainData +=  "</td>";

        //trainData = trainData + "<td>" + train->travelTime + "</td>";
        trainData += "<td>";
        for(auto tickets = train->freePlaces.begin();tickets!=train->freePlaces.end(); ++tickets)
        {
            trainData += "<div class=\"places\">" + tickets->placeClass  + ": " + QString::number(tickets->placesNumber) + "</div>";
        }
        trainData += "</td></tr>";

    }
    trainData += "</table></body></html>";
    webView->setHtml(trainData);
}



void BrowserPage::showAvailableCoaches(Train *train)
{qDebug()<<"show";
    if (!train->checkComleteness()) return;

    QString data = "<html><body>";
    data += "<h4 class=\"reference\"><a id=\"trainsRef\" href=\"" + referenceTrains + "\">Назад</a></h4>";
    data += "<table><tr><th>Тип</th>"
                              "<th>№</th>"
                              "<th>Кількість</th></tr>";

    for(auto&& type = train->freePlaces.begin(); type!=train->freePlaces.end();++type)
    {
        for(auto& p = train->coaches.begin();p!= train->coaches.end(); ++p)
        {
            if (p->coachClass == type->placeClass)
            {
                data += "<tr><td>" + type->placeClass + "</td>" +
                        + "<td>" + QString::number(p->number) + "</td>" +
                        + "<td>" + QString::number(p->placesNumber) + "</td></tr>";
            }
        }
    }
    data += "</table></body></html>";

    webView->setHtml(data);
}



void BrowserPage::showError(QString error)
{

    webView->setHtml("<html><body><h4>" + error + "</h4></body></html>");
}


QDate BrowserPage::tripDate()
{
    return dateField->date();
}




SettingsPage::SettingsPage(WidgetsMediator *widgetsMediator, QWidget *parent)
    :QWidget(parent), BasePage(widgetsMediator)
{
    QGroupBox* trainsBox = new QGroupBox("Поїзди",this);
    QGroupBox* coachesBox = new QGroupBox("Типи вагонів",this);
    QGroupBox* buttonsBox = new QGroupBox();


    allTrainsBtn = new QRadioButton("Всі",this);
    oneTrainBtn = new QRadioButton("Окремі",this);

    trainsGroupLayout = new QGridLayout;

    QVBoxLayout* trainsLayout = new  QVBoxLayout(this);
    trainsLayout->addWidget(allTrainsBtn);
    trainsLayout->addWidget(oneTrainBtn);
    trainsLayout->addLayout(trainsGroupLayout);
    trainsLayout->setAlignment(trainsGroupLayout,Qt::AlignRight);
    trainsBox->setLayout(trainsLayout);


    coachesTypesLayout = new QHBoxLayout;
    coachesBox->setLayout(coachesTypesLayout);

    startSearchBtn = new QPushButton("Запуск пошуку");
    showBrowserBtn = new QPushButton("Станції");
    QHBoxLayout* buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(showBrowserBtn);
    buttonsLayout->addWidget(startSearchBtn);

    buttonsBox->setFlat(true);
    buttonsBox->setLayout(buttonsLayout);


    QVBoxLayout* pagelayout = new QVBoxLayout(this);
    pagelayout->addWidget(trainsBox);
    pagelayout->setAlignment(trainsBox,Qt::AlignTop);
    pagelayout->addWidget(coachesBox);
    pagelayout->addWidget(buttonsBox);
    pagelayout->setAlignment(buttonsBox,Qt::AlignBottom);

    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    connect(networkManager,&NetworkManager::responseReady,this,&SettingsPage::getTrainsOnRoute);

    connect(allTrainsBtn,&QRadioButton::clicked,this,&SettingsPage::onRadioButtonClick);
    connect(oneTrainBtn,&QRadioButton::clicked,this,&SettingsPage::onRadioButtonClick);
    connect(startSearchBtn,&QPushButton::clicked,this,&SettingsPage::startScanner);
    connect(showBrowserBtn,&QPushButton::clicked,this,&SettingsPage::showBrowser);
    exploreRout();
}

const QByteArray trainsOnRoute = "trainsOnRoute";


//Ukrazaliznitsya doesn't have public request to get "all possible trains" between stations.
//So,we send some "search" request for different dates to receive as much as possible existing trains.
void SettingsPage::exploreRout()
{
    NetworkManager* networkManager = UZApplication::instance()->networkManager();
    QDate futuredate = QDate::currentDate();
    futuredate = futuredate.addDays(24);
    SearchPOSTData searchdata(mediator()->getStationIDFrom(),mediator()->getStationIDTo(),futuredate.toString("MM.dd.yyyy"));
    networkManager->sendSearchRequest(searchdata,trainsOnRoute);
    futuredate = QDate::currentDate().addDays(1);
    searchdata.tripDate = futuredate.toString("MM.dd.yyyy");
    networkManager->sendSearchRequest(searchdata,trainsOnRoute);

}

bool SettingsPage::isChanged()
{
    QVector<QString> currentTrains;
    QVector<QString> currentCoachTypes;

    if (prevState_allTrainBtn!=allTrainsBtn->isChecked())
            return true;

    for(auto train : trainsGroup)
        if (train->isChecked())
            currentTrains.push_back(train->text());
    for(auto coachType : coachesTypes)
        if (coachType->isChecked())
            currentCoachTypes.push_back(coachType->text());
    if (currentTrains!=prevState_trainsGroup || currentCoachTypes!=prevState_coachesTypes) {
        qDebug()<<"settings were changed";
        return true;
    } else
        return false;

}

void SettingsPage::saveState()
{
    prevState_trainsGroup.clear();
    prevState_coachesTypes.clear();
    for(auto train : trainsGroup)
    {
        if (train->isChecked())
            prevState_trainsGroup.push_back(train->text());
    }
    for(auto coachType : coachesTypes)
    {
        if (coachType->isChecked())
            prevState_coachesTypes.push_back(coachType->text());
    }
    prevState_allTrainBtn = allTrainsBtn->isChecked();
}

//It's response on the exploreRout() method.
//when we get each responce from Ukrzaliznitsya, we parse existing trains, types of tickets
//and create checkboxes by drawTrainsWidgets() function.
void SettingsPage::getTrainsOnRoute(QNetworkReply *reply, QByteArray id)
{
    if (reply==nullptr) return;

    if (id == trainsOnRoute)
    {
        Trains allTrainsOnRoute;
        UZApplication::instance()->parseSearchResults(reply,allTrainsOnRoute);
        QVector<QString> placeTypes;
        QVector<QString> trainsOnRoute;
        for(auto train : allTrainsOnRoute)
        {
            auto p = std::find_if(trainsGroup.begin(),trainsGroup.end(),[&train](QCheckBox* box){return train.number==box->text();});
            if (p==trainsGroup.end()) {
                trainsOnRoute.push_back(train.number);
                for(auto&& placeType: train.freePlaces)
                    placeTypes.push_back(placeType.placeClass);
            }
        }

        std::sort(placeTypes.begin(),placeTypes.end());
        auto pLast = std::unique(placeTypes.begin(),placeTypes.end());
        placeTypes.erase(pLast,placeTypes.end());

        for(auto pType = coachesTypes.begin();pType!=coachesTypes.end();++pType)
            placeTypes.erase(std::remove(placeTypes.begin(),placeTypes.end(),(*pType)->text()),placeTypes.end());

        drawTrainsWidgets(trainsOnRoute,placeTypes);
    }
}

void SettingsPage::drawTrainsWidgets(QVector<QString> &trains, QVector<QString> &places)
{

        int j = trainsGroup.size()>0 ? (trainsGroup.size()) % 4  :  0;
        int i = trainsGroup.size()>0 ? (trainsGroup.size()-j) / 4  :  0;

        for(auto train : trains)
        {
            QCheckBox* box = new QCheckBox(train);
            box->setEnabled(false);
            trainsGroup.push_back(box);
            trainsGroupLayout->addWidget(box,i,j);

            ++j;
            if (j>3){
                j=0; ++i;
            }
        }

        for(auto&& coachType: places)
        {
            QCheckBox* box2 = new QCheckBox(coachType);
            box2->setChecked(true);
            coachesTypesLayout->addWidget(box2);
            coachesTypes.push_back(box2);
        }

        std::sort(trainsGroup.begin(),trainsGroup.end(),[](QCheckBox* a, QCheckBox* b){return a->text() < b->text();});
        std::sort(coachesTypes.begin(),coachesTypes.end(),[](QCheckBox* a, QCheckBox* b){return a->text() < b->text();});
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

bool SettingsPage::checkConditions()
{

    auto trainsResult = std::find_if(trainsGroup.begin(),trainsGroup.end(),[](QCheckBox* box){return box->isChecked();});
    auto coachesResult = std::find_if(coachesTypes.begin(),coachesTypes.end(),[](QCheckBox* box){return box->isChecked();});

    if (trainsResult==trainsGroup.end() || coachesResult==coachesTypes.end()) {
        QMessageBox msgBox;
        msgBox.setText("Визначте умови для пошуку квитків. Оберіть доступні поїзди і типи вагонів");
        msgBox.exec();
        return false;
    }
    return true;
}



void SettingsPage::startScanner()
{
    if (checkConditions()==false)
        return;

    mediator()->setSearchParameters();
    mediator()->showProcessingPage();
    saveState();

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
    infoLabel->setAlignment(Qt::AlignJustify);
    statusLabel = new QLabel;
    animatedSearchWidget =  new AnimatedSearchWidget(UZApplication::instance()->mainWindow->width(),this);

    showSettingsButton = new QPushButton("Змінити налаштування пошуку");

    QVBoxLayout* pagelayout = new QVBoxLayout(this);
    pagelayout->addWidget(infoLabel);
    pagelayout->addWidget(statusLabel);
    pagelayout->addWidget(animatedSearchWidget);
    pagelayout->addWidget(showSettingsButton);
    pagelayout->setAlignment(showSettingsButton,Qt::AlignBottom);


    //connect(UZApplication::instance(),&UZApplication::updateSearchStatus,this,&ProcessingPage::setSearchStatus);
    connect(UZApplication::instance(),&UZApplication::updateSearchStatus,this,&ProcessingPage::updatePage);
    connect(showSettingsButton,&QPushButton::clicked,this,&ProcessingPage::showSettings);

    updatePage();


}
/*
void ProcessingPage::setSearchStatus(UZApplication::SearchStatus status)
{
   // searchStatus = isFound;
    if (UZApplication::SearchStatus::Found) {
        statusLabel->setText("Знайдено");
        animatedSearchWidget->setSearchStatus(status);
        }//temp. will change to enum
    else {
        statusLabel->setText("Пошук");
        animatedSearchWidget->setSearchStatus(status);
    }
}
*/
void ProcessingPage::showSettings()
{
    mediator()->showSettingPage();
}

void ProcessingPage::updatePage()
{
    QString info = "Пошук залізничних квитків між станціями " + mediator()->getStationFrom() + " - " + mediator()->getStationTo() + ", ";

    if (mediator()->searchParameters->searchForAnyTrain())
        info = info + "для всіх поїздів";
    else {
        info = info + "для поїздів: ";
        for(auto& num: mediator()->getChosenTrains())
            info = info + num +  " ";
        info = info + "\nДата відправлення: " +mediator()->tripDate().toString("dd.MM.yyyy");
    }

    infoLabel->setText(info);
   // setSearchStatus(UZApplication::instance()->status());
    if (UZApplication::instance()->status()==UZApplication::SearchStatus::Found) {
        statusLabel->setText("Знайдено");
        animatedSearchWidget->updateSearchStatus();
    }else {
        statusLabel->setText("Пошук");
        animatedSearchWidget->updateSearchStatus();
    }
}
