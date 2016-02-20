#include "uzmainwindow.h"
#include "ui_uzmainwindow.h"

#include "uzapplication.h"
#include "lineedit.h"
#include "networkmanager.h"
#include "requestdata.h"
#include "windowpages.h"
#include "widgetsmediator.h"

#include <QGridLayout>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QPushButton>
#include <QByteArray>
#include <QNetworkReply>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

const QByteArray searchRequest = "searchRequest";
const QByteArray coachRequest = "coachRequest";
const QByteArray coachesRequest = "coachesRequest";

UZMainWindow::UZMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UZMainWindow),
    searchReply(nullptr)
{
    //ui->setupUi(this);

    //trainSearchPage = new TrainSearchPage;
    //scannerPage = new ScannerPage(trainSearchPage);

   // pagesWidget = new QStackedWidget(this);
    //pagesWidget->addWidget(trainSearchPage);
    //pagesWidget->addWidget(scannerPage);

    networkManager = UZApplication::instance()->networkManager();
    //trains = &UZApplication::instance()->trains;

    widgetsMediator = new WidgetsMediator();


    //setCentralWidget(pagesWidget);
    setCentralWidget(widgetsMediator->initializeWidgets());

    //connect(trainSearchPage->showSettingsButton,&QPushButton::clicked,this,&UZMainWindow::showNextPage);
  //  connect(scannerPage->startSearchBtn,&QPushButton::clicked,this,&UZMainWindow::showNextPage);

}


UZMainWindow::~UZMainWindow()
{
    delete ui;
}


void UZMainWindow::showAvailableTrains()
{
   // trainSearchPage->showAvailableTrains();
    widgetsMediator->showAvailableTrains();
}

void UZMainWindow::showAvailableCoaches(Train *train)
{
   // trainSearchPage->showAvailableCoaches(train);
    widgetsMediator->showAvailableCoaches(train);
}

void UZMainWindow::showNextPage()
{
    /*
    if (pagesWidget->currentWidget()==scannerPage) {        //temporary

        processingPage = new ProcessingPage(UZApplication::instance()->getSearchParameters());
        pagesWidget->addWidget(processingPage);
        pagesWidget->setCurrentWidget(processingPage);
    }
    if (pagesWidget->currentWidget()==trainSearchPage) {
        scannerPage->exploreRout();
        pagesWidget->setCurrentWidget(scannerPage);
    }
*/

}


void UZMainWindow::error(QNetworkReply::NetworkError err)
{
  //  qDebug()<<"error: "<<searchReply->error();
     qDebug()<<"err: "<<err;
  //  int ret = QMessageBox::critical(this, tr("UZ scanner"),searchReply->errorString(),QMessageBox::Ok);
}
