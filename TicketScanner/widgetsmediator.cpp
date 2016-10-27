#include "widgetsmediator.h"
#include "windowpages.h"

#include "uzapplication.h"
#include "searchparameters.h"
#include "lineedit.h"
#include "networkmanager.h"
#include "requestdata.h"

#include <QWidget>
#include <QStackedWidget>
//#include <QGridLayout>
//#include <QCalendarWidget>
//#include <QDateTime>
#include <memory>
#include <QDebug>


WidgetsMediator::WidgetsMediator()
    :browserPage(nullptr),settingsPage(nullptr),
      processingPage(nullptr),stackedWidget(nullptr),searchParameters(nullptr)
{
    stackedWidget = new QStackedWidget;
    browserPage = new BrowserPage(this);
    stackedWidget->addWidget(browserPage);
}

WidgetsMediator::~WidgetsMediator()
{
    delete browserPage;
    if (settingsPage)
        delete settingsPage;
    if (processingPage)
        delete processingPage;
    delete stackedWidget;
}

QWidget* WidgetsMediator::getStartWidgets()
{
    return stackedWidget;
}


void WidgetsMediator::showBrowserPage()
{
    stackedWidget->setCurrentWidget(browserPage);
}

void WidgetsMediator::showSettingPage()
{
    if (browserPage->isChanged() && settingsPage) {
        delete settingsPage;
        settingsPage = nullptr;
     }
    if(!settingsPage) {
        settingsPage = new SettingsPage(this);
        stackedWidget->addWidget(settingsPage);
    }
    stackedWidget->setCurrentWidget(settingsPage);
}

void WidgetsMediator::showProcessingPage()
{
    if (settingsPage->isChanged() && processingPage) {
        processingPage->updatePage();
    }
    if (!processingPage) {
        processingPage = new ProcessingPage(this);
        stackedWidget->addWidget(processingPage);
    }
    stackedWidget->setCurrentWidget(processingPage);
}

void WidgetsMediator::setSearchParameters()
{
    if (settingsPage->isChanged()) {

        searchParameters = std::make_shared<SearchParameters>(getStationIDFrom(),getStationIDTo(),tripDate());
        searchParameters->setStationsName(getStationFrom(),getStationTo());

        searchParameters->setTrains().clear();
        searchParameters->setCoachTypes().clear();

        for(auto train: settingsPage->trainsGroup)
            if (train->isChecked())
                searchParameters->setTrains().push_back(train->text());
        for(auto coach: settingsPage->coachesTypes)
            if (coach->isChecked())
                searchParameters->setCoachTypes().push_back(coach->text());

        if (settingsPage->allTrainsBtn->isChecked())
            searchParameters->setSearchForAnyTrain(true);
        else
            searchParameters->setSearchForAnyTrain(false);

        UZApplication::instance()->startScanning(searchParameters);
    }

}


void WidgetsMediator::resetSearch()
{
    browserPage->webView->setHtml("<html><body></body></html>");
    stackedWidget->setCurrentWidget(browserPage);
    //delete processingPage;
    //processingPage->deleteLater();
    //processingPage = nullptr;
}

void WidgetsMediator::showAvailableTrains()
{
    browserPage->showAvailableTrains();
}

void WidgetsMediator::showAvailableCoaches(Train *train)
{
    browserPage->showAvailableCoaches(train);
}


QString WidgetsMediator::getStationFrom()
{
    return browserPage->editFrom->text();
}

QString WidgetsMediator::getStationTo()
{
    return browserPage->editTo->text();
}

QString WidgetsMediator::getStationIDFrom()
{
    return browserPage->editFrom->getStationID();
}

QString WidgetsMediator::getStationIDTo()
{    
    return browserPage->editTo->getStationID();
}

QDate WidgetsMediator::tripDate()
{
    return browserPage->tripDate();
}

const QVector<QString> &WidgetsMediator::getChosenTrains() const
{

    return searchParameters->getTrains();

}
