#include "uzmainwindow.h"
#include "ui_uzmainwindow.h"

#include "uzapplication.h"
#include "networkmanager.h"
#include "windowpages.h"
#include "widgetsmediator.h"


#include <QByteArray>
#include <QNetworkReply>

#include <QDebug>
#include <QMessageBox>

const QByteArray searchRequest = "searchRequest";
const QByteArray coachRequest = "coachRequest";
const QByteArray coachesRequest = "coachesRequest";

UZMainWindow::UZMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UZMainWindow)
{
    //ui->setupUi(this);

    //networkManager = UZApplication::instance()->networkManager();
    widgetsMediator = new WidgetsMediator();

    setCentralWidget(widgetsMediator->getStartWidgets());

    //Qt::WindowFlags flags = 0;
    //flags = Qt::Window | Qt::WindowCloseButtonHint ;//| Qt::WindowMinimizeButtonHint;
    //setWindowFlags(flags);

    qDebug()<<this->windowFlags();

}


UZMainWindow::~UZMainWindow()
{
    delete ui;
    delete widgetsMediator;
}


void UZMainWindow::showAvailableTrains()
{
    widgetsMediator->showAvailableTrains();
}

void UZMainWindow::showAvailableCoaches(Train *train)
{
    widgetsMediator->showAvailableCoaches(train);
}


/*
void UZMainWindow::error(QNetworkReply::NetworkError err)
{
  //  qDebug()<<"error: "<<searchReply->error();
     qDebug()<<"err: "<<err;
  //  int ret = QMessageBox::critical(this, tr("UZ scanner"),searchReply->errorString(),QMessageBox::Ok);
}
*/
