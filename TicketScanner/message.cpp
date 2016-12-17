#include "message.h"
#include "windowwrapper.h"
#include <QMessageBox>
#include <QEventLoop>
#include <QDebug>

Message::Message(QString message, QWidget *parent) : QWidget(parent)
{
    eventLoop = new QEventLoop(this);

    customWindow = new WindowWrapper();

    internalMessage = new QMessageBox();
   // internalMessage->setMinimumWidth(200); //BUG for short message is taking place
    internalMessage->setText(message);
    customWindow->setMainWidget(internalMessage);
    customWindow->setWindowModality(Qt::ApplicationModal);
    customWindow->show();



    qDebug()<<"Message construct";

    //After button click
    //internalMessage closes because its own EventLoop is finished
    //we close customWindow and finish our event loop (release paused function)
    connect(internalMessage,&QMessageBox::buttonClicked,[&](){ customWindow->close();eventLoop->exit();});
}


Message::~Message()
{
    qDebug()<<"Message destruct";

}



void Message::exec()
{
    eventLoop->exec();
}




