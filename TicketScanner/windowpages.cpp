#include "windowpages.h"
#include "lineedit.h"
#include "requestdata.h"
#include "application.h"
#include "networkmanager.h"
#include "searchparameters.h"
#include "config.h"
#include "widgetsmediator.h"
#include "tooltips.h"
#include "message.h"
#include <memory>

#include <QGridLayout>
#include <QDateEdit>
#include <QPushButton>
#include <QWebView>
#include <QGroupBox>
#include <QCheckBox>
#include <QDesktopServices>
#include <algorithm>

#include <QLabel>
#include <QFile>
#include <QSpacerItem>

#include "animatedsearchwidget.h"

#include "uzmainwindow.h"

//#include <QUrl>
#include "parser.h"
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
    webFrame = new QFrame(this);
    webFrame->setObjectName(QStringLiteral("webViewFrame"));
    webFrame->setMinimumSize(QSize(400,250));
    webFrame->setFrameShape(QFrame::Box);
    webFrame->setFrameShadow(QFrame::Plain);
    webView = new QWebView(webFrame);
    webView->setGeometry(2,2,396,246);
    webView->settings()->setUserStyleSheetUrl(QUrl("qrc:/resources/styles.css"));
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    webView->setHtml("<!DOCTYPE html><html><body></body></html>");
    //webView->setMaximumWidth(400);
    //webView->setMaximumHeight(250);

    QWidget* stationsWidget = new QWidget(this);
    QHBoxLayout* stationsLayout = new QHBoxLayout(this);
    stationsLayout->setMargin(0);
    stationsLayout->addWidget(editFrom);
    stationsLayout->insertSpacing(1,20);
    stationsLayout->addWidget(editTo);
    stationsWidget->setLayout(stationsLayout);


    QGridLayout *pagelayout = new QGridLayout;
    pagelayout->addWidget(stationsWidget,0,0,1,3);
    pagelayout->addWidget(dateField,1,0);
    pagelayout->addWidget(searchButton,2,1);
    pagelayout->addWidget(webFrame,3,0,3,3);
    pagelayout->addWidget(showSettingsButton,6,0,1,3);

    setLayout(pagelayout);

    previousState_fromStation = "";
    previousState_toStation = "";
    previous_tripDate = QDate();


    connect(searchButton,&QPushButton::clicked,this,&BrowserPage::ticketsSearch);
    connect(showSettingsButton,&QPushButton::clicked,this,&BrowserPage::showSettings);
    connect(webView,&QWebView::linkClicked,this,&BrowserPage::processLink);
    connect(Application::instance(),&Application::searchError,this,&BrowserPage::showError);
    connect(Application::instance()->networkManager(),&NetworkManager::connectionLost,this,[&](){showError("Відсутній зв'язок із сервером Укрзалізниці.");}); //TODO: pretify architecture
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
    if (isChanged() ||  previous_tripDate != tripDate())
    {
        //Although changes of date not considered in the BrowserPage::isChanged()
        //We should reset current list of trains, if date was changed
        Application::instance()->resetTrains();   //avoid mix with previous list of trains;
    }

    NetworkManager* networkManager = Application::instance()->networkManager();
    QString date = dateField->date().toString(Config::RequestDateFormat);
    SearchPOSTData searchdata(editFrom->getStationID(),editTo->getStationID(),date);
    networkManager->sendSearchRequest(searchdata,RequestType::SearchRequest);
    saveState();
}

bool BrowserPage::checkConditions()
{
    if (editFrom->text().length()==0 || editTo->text().length()==0) {
        Message msgBox("Оберіть станції відправлення та прибуття");
        msgBox.exec();
        return false;
    }
    if (dateField->date() < QDate::currentDate())
    {
        Message msgBox("Зазначена дата вже пройшла");
        msgBox.exec();
        return false;
    }
    return true;
}


//Changes of BrowserPage cause construction(reconstruction) of SettingsPage
//We should rebuild all controls on the SettingsPage ONLY if stations or date changed
bool BrowserPage::isChanged()
{
    //if (previousState_fromStation.isEmpty() && previousState_toStation.isEmpty() )  //for the first check;
    //    return false;
    //problems if connection broke.

    if(previousState_fromStation!=editFrom->text() ||
           previousState_toStation != editTo->text() ||
            previous_tripDate != QDate::fromString(editTo->text()))
        return true;
    else
        return false;
}

void BrowserPage::saveState()
{
    previousState_fromStation = editFrom->text();
    previousState_toStation = editTo->text();
    previous_tripDate = tripDate();
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
     const Train* currentTrain = Application::instance()->getTrain(trainNum);

     NetworkManager* networkManager = Application::instance()->networkManager();

     for(auto&& p = currentTrain->freePlaces.begin();p!=currentTrain->freePlaces.end();++p)
     {
        CoachesPOSTData postdata(editFrom->getStationID(),editTo->getStationID(),currentTrain->dateDeparture.toString(Config::RequestDateFormat),
                              trainNum,p->placeClass);
        networkManager->sendCoachesRequest(postdata,RequestType::CoachesRequest);
     }

}


void BrowserPage::showAvailableTrains()
{
    QString trainData;
    const Trains* trains = Application::instance()->trains();


    trainData =trainData+ "<html><body><table>";

    for(auto&& train = trains->begin();train!=trains->end();++train)
    {
        trainData += "<tr>";
        trainData += "<td> <a href=\"" + train->number + "\">" +  train->number + "</a> </td>";

        trainData +=  "<td>";
        trainData += "<div class=\"stations\">" + train->stationDeparture + "</div>";
        trainData += "<div class=\"stations\">  " + train->stationArrival + "</div>";
        trainData +=  "</td>";

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
{
    if (!train->checkComleteness())
                    return;

    QString data = "<html><body>";
    data += "<h4 class=\"reference\"><a id=\"trainsRef\" href=\"" + referenceTrains + "\">Назад</a></h4>";
    data += "<table><tr><th>Тип</th>"
                              "<th>№</th>"
                              "<th>Кількість</th></tr>";

    for(auto&& type = train->freePlaces.begin(); type!=train->freePlaces.end();++type)
    {
        for(auto&& p = train->coaches.begin();p!= train->coaches.end(); ++p)
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
    webView->setHtml("<html><body><h4 class=\"error\">" + error + "</h4></body></html>");
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
    QWidget* buttonsBox = new QWidget();


    allTrainsBtn = new QRadioButton("Всі",this);
    oneTrainBtn = new QRadioButton("Окремі",this);
    oneTrainBtn->setChecked(true);

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

    buttonsBox->setLayout(buttonsLayout);


    QVBoxLayout* pagelayout = new QVBoxLayout(this);
    pagelayout->addWidget(trainsBox);
    pagelayout->setAlignment(trainsBox,Qt::AlignTop);
    pagelayout->addWidget(coachesBox);
    pagelayout->addWidget(buttonsBox);
    pagelayout->setAlignment(buttonsBox,Qt::AlignBottom);

    NetworkManager* networkManager = Application::instance()->networkManager();
    connect(networkManager,&NetworkManager::responseReady,this,&SettingsPage::getTrainsOnRoute);

    connect(allTrainsBtn,&QRadioButton::clicked,this,&SettingsPage::onRadioButtonClick);
    connect(oneTrainBtn,&QRadioButton::clicked,this,&SettingsPage::onRadioButtonClick);
    connect(startSearchBtn,&QPushButton::clicked,this,&SettingsPage::startScanner);
    connect(showBrowserBtn,&QPushButton::clicked,this,&SettingsPage::showBrowser);
    exploreRout();
}



void SettingsPage::exploreRout()
{
    NetworkManager* networkManager = Application::instance()->networkManager();
    QDate tripDate = mediator()->tripDate();

    SearchPOSTData searchdata(mediator()->getStationIDFrom(),mediator()->getStationIDTo(),tripDate.toString(Config::RequestDateFormat));
    networkManager->sendSearchRequest(searchdata,RequestType::TrainsOnRoute);


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
void SettingsPage::getTrainsOnRoute(QNetworkReply *reply, RequestType::Request id)
{
    if (reply==nullptr) return;

    if (id == RequestType::TrainsOnRoute)
    {
        Trains allTrainsOnRoute;
        Application::instance()->parseRouteResults(reply,allTrainsOnRoute);
        QVector<QString> placeTypes;
        QVector<Train> trainsOnRoute;
        for(auto train : allTrainsOnRoute)
        {
            auto p = std::find_if(trainsGroup.begin(),trainsGroup.end(),[&train](QCheckBox* box){return train.number==box->text();});
            if (p==trainsGroup.end()) {
                trainsOnRoute.push_back(train);
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

void SettingsPage::drawTrainsWidgets(QVector<Train>& trains, QVector<QString>& places)
{
        ToolTipsWagonTypes coachToolTips;

        int j = trainsGroup.size()>0 ? (trainsGroup.size()) % 4  :  0;
        int i = trainsGroup.size()>0 ? (trainsGroup.size()-j) / 4  :  0;

        for(auto& train : trains)
        {
            QCheckBox* box = new QCheckBox(train.number);
            box->setEnabled(true);
            box->setToolTip(train.stationDeparture + " - " + train.stationArrival);
            trainsGroup.push_back(box);
            trainsGroupLayout->addWidget(box,i,j);

            ++j;
            if (j>3){
                j=0; ++i;
            }
        }

        for(auto& coachType: places)
        {
            QCheckBox* box2 = new QCheckBox(coachType);
            box2->setToolTip(std::move(coachToolTips.getToolTip(coachType)));
            box2->setChecked(true);
            coachesTypesLayout->addWidget(box2);
            coachesTypes.push_back(box2);
        }
        QCheckBox* box2 = new QCheckBox("Усі");
        box2->setToolTip("Будь-який тип");
        box2->setChecked(false);
        coachesTypesLayout->addWidget(box2);
        coachesTypes.push_back(box2);

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
        Message msgBox("Визначте умови для пошуку квитків.\n Оберіть доступні поїзди і типи вагонів");
        msgBox.exec();
        return false;
    }
    return true;
}



void SettingsPage::startScanner()
{
    if (checkConditions()==false)
        return;

    mediator()->setSearchParametersAndRunSearsh();
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
    setObjectName("processingPage");

    infoLabel = new QLabel(this);
    infoLabel->setObjectName(QStringLiteral("searchLabel"));
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignJustify);
    statusLabel = new QLabel(this);
    warningLabel = new QLabel(this);
    warningLabel->setObjectName(QStringLiteral("processingLabel"));
    warningLabel->setWordWrap(true);
    warningLabel->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
    animatedSearchWidget =  new AnimatedSearchWidget(Application::instance()->mainWindow->width(),this);
    connectionLostLabel = new QLabel(this);
    connectionLostLabel->setText("Відсутній зв'язок із сервером!\nЯк тільки його буде відновлено, пошук продовжиться автоматично");
    connectionLostLabel->setObjectName(QStringLiteral("connectionLostLabel"));

    showSettingsButton = new QPushButton("Змінити налаштування пошуку");
    openDefaultBrowserButton = new QPushButton("Відкрити браузер для замовлення квитків");
    openDefaultBrowserButton->hide();
    startNewSearchButton = new QPushButton("Новий пошук");
    startNewSearchButton->hide();

    QVBoxLayout* pagelayout = new QVBoxLayout(this);
    pagelayout->addWidget(infoLabel);
    pagelayout->addWidget(statusLabel);
    pagelayout->addWidget(animatedSearchWidget);
    pagelayout->addWidget(warningLabel,Qt::AlignBottom);
    pagelayout->addWidget(connectionLostLabel,Qt::AlignBottom);
    pagelayout->addWidget(openDefaultBrowserButton,Qt::AlignBottom);
    pagelayout->addWidget(showSettingsButton);
    pagelayout->setAlignment(showSettingsButton,Qt::AlignBottom);
    pagelayout->addWidget(startNewSearchButton);
    pagelayout->setAlignment(startNewSearchButton,Qt::AlignBottom);

    pagelayout->setSpacing(10);

    connect(Application::instance(),&Application::updateSearchStatus,this,&ProcessingPage::updatePage);
    connect(showSettingsButton,&QPushButton::clicked,this,&ProcessingPage::showSettings);
    connect(openDefaultBrowserButton,&QPushButton::clicked,this,&ProcessingPage::openBrowser);
    connect(startNewSearchButton,&QPushButton::clicked,[this](){this->mediator()->resetSearch();
                                                                Application::instance()->resetTrains(); //TODO: prettify
                                                                Application::instance()->setStatus(Application::SearchStatus::Check);});
    connect(Application::instance()->networkManager(),&NetworkManager::connectionLost,this,[this](){this->connectionLostLabel->show();});

    updatePage();
setAttribute(Qt::WA_NoSystemBackground);

}

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
    }
    info = info + "\nДата відправлення: " +mediator()->tripDate().toString(Config::RequestDateFormat);

    infoLabel->setText(info);
    warningLabel->setText("Не закривайте програму. Наявність квитків перевіряється щохвилинно.");
    connectionLostLabel->hide(); //If we call this function, connection is OK

    if (Application::instance()->status()==Application::SearchStatus::Found) {
        statusLabel->setText("Знайдено");
        animatedSearchWidget->updateSearchStatus();
        warningLabel->setText("");
        openDefaultBrowserButton->show();
        showSettingsButton->hide();
        startNewSearchButton->show();
        warningLabel->hide();
        mediator()->showProcessingPage(); //TODO: refactor need! But after finish of search, ProcessingPage MUST shown.
    }else {
        statusLabel->setText("Пошук");
        animatedSearchWidget->updateSearchStatus();
        openDefaultBrowserButton->hide();
        startNewSearchButton->hide();
        showSettingsButton->show();
        warningLabel->show();

    }
}

void ProcessingPage::openBrowser()
{
    bool result = QDesktopServices::openUrl(QUrl(Config::BookingUZsite));

    qDebug()<<"open"<<result;
    if (!result) {
        Message msgBox("Не відкривається ваш браузер (Mozilla, Google Chrome, Safari..)."
                       " Перевірте налаштування вашого браузера за замовчуванням"
                       " або відкрийте сайт Укрзалізниці самостійно");
        msgBox.exec();
    }

}
