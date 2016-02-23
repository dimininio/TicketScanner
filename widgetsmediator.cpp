#include "widgetsmediator.h"
#include "windowpages.h"

#include "uzapplication.h"
#include "searchparameters.h"
#include "lineedit.h"
#include "networkmanager.h"
#include "requestdata.h"

#include <QWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include <QCalendarWidget>
#include <QDateTime>
#include <QDebug>


WidgetsMediator::WidgetsMediator()
    :trainSearchPage(nullptr),scannerPage(nullptr),
      processingPage(nullptr),stackedWidget(nullptr)
{

}

WidgetsMediator::~WidgetsMediator()
{
    delete trainSearchPage;
    if (scannerPage)
        delete scannerPage;
    if (processingPage)
        delete processingPage;
    delete stackedWidget;
}

QWidget* WidgetsMediator::initializeWidgets()
{
    stackedWidget = new QStackedWidget;
    trainSearchPage = new TrainSearchPage(this);
    stackedWidget->addWidget(trainSearchPage);

    return stackedWidget;
}


void WidgetsMediator::prepareScannerPage()
{
    scannerPage = new ScannerPage(this);
    stackedWidget->addWidget(scannerPage);
    stackedWidget->setCurrentWidget(scannerPage);
}

void WidgetsMediator::prepareProcessingPage()
{
    processingPage = new ProcessingPage(this);
    stackedWidget->addWidget(processingPage);
    stackedWidget->setCurrentWidget(processingPage);
}

void WidgetsMediator::setSearchParameters(std::shared_ptr<SearchParameters>& p)
{
    if (p) searchParameters = p;
}

void WidgetsMediator::showAvailableTrains()
{
    trainSearchPage->showAvailableTrains();
}

void WidgetsMediator::showAvailableCoaches(Train *train)
{
    trainSearchPage->showAvailableCoaches(train);
}


QString WidgetsMediator::getStationFrom()
{
    return trainSearchPage->fromEdit()->text();
}

QString WidgetsMediator::getStationTo()
{
    return trainSearchPage->toEdit()->text();
}

QString WidgetsMediator::getStationIDFrom()
{
    return trainSearchPage->fromEdit()->getStationID();
}

QString WidgetsMediator::getStationIDTo()
{
    return trainSearchPage->toEdit()->getStationID();
    //check if I can send signal getStations from here.
}

QDate WidgetsMediator::tripDate()
{
    return trainSearchPage->tripDate();
}

const QVector<QString> &WidgetsMediator::getChosenTrains() const
{
    //return searchParameters->getTrains();
    return searchParameters->getTrains();

}
