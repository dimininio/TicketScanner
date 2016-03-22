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
#include <QDebug>


WidgetsMediator::WidgetsMediator()
    :browserPage(nullptr),settingsPage(nullptr),
      processingPage(nullptr),stackedWidget(nullptr)
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
    if(!settingsPage) {
        settingsPage = new SettingsPage(this);
        stackedWidget->addWidget(settingsPage);
    }
    stackedWidget->setCurrentWidget(settingsPage);
}

void WidgetsMediator::showProcessingPage()
{
    if (!processingPage) {
        processingPage = new ProcessingPage(this);
        stackedWidget->addWidget(processingPage);
    }
    processingPage->updatePage();
    stackedWidget->setCurrentWidget(processingPage);
}

void WidgetsMediator::setSearchParameters(std::shared_ptr<SearchParameters>& p)
{
    if (p) searchParameters = p;
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
