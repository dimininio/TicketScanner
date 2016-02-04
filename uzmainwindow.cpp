#include "uzmainwindow.h"
#include "ui_uzmainwindow.h"

#include "uzapplication.h"
#include "lineedit.h"
#include "networkmanager.h"
#include "requestdata.h"
#include "windowpages.h"

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

    trainSearchPage = new TrainSearchPage;

    pagesWidget = new QStackedWidget(this);
    pagesWidget->addWidget(trainSearchPage);

    networkManager = UZApplication::instance()->networkManager();
    //trains = &UZApplication::instance()->trains;



    setCentralWidget(pagesWidget);




}


UZMainWindow::~UZMainWindow()
{
    delete ui;
}


void UZMainWindow::showAvailableTrains()
{
    trainSearchPage->showAvailableTrains();
}

void UZMainWindow::showAvailableCoaches(Train *train)
{
    trainSearchPage->showAvailableCoaches(train);
}




void UZMainWindow::error(QNetworkReply::NetworkError err)
{
  //  qDebug()<<"error: "<<searchReply->error();
     qDebug()<<"err: "<<err;
  //  int ret = QMessageBox::critical(this, tr("UZ scanner"),searchReply->errorString(),QMessageBox::Ok);
}
