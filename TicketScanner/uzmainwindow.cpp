#include "uzmainwindow.h"
#include "widgetsmediator.h"

#include <QDebug>
#include <QMessageBox>


UZMainWindow::UZMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    widgetsMediator = new WidgetsMediator();

    setCentralWidget(widgetsMediator->getStartWidgets());

   // connect(p_networkManager,&NetworkManager::connectionDoesNotWork,this,&UZMainWindow::connectionErrorSlot);

    //qDebug()<<this->windowFlags();

}


UZMainWindow::~UZMainWindow()
{
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
