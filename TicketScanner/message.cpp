#include "message.h"
#include "windowwrapper.h"
#include <QMessageBox>
#include <QEventLoop>

Message::Message(QString message, QWidget *parent) : QWidget(parent)
{
    eventLoop = new QEventLoop(this);

    customWindow = new WindowWrapper();

    internalMessage = new QMessageBox();
   // internalMessage->setMinimumWidth(200); //BUG: short messages doesn't look nice
    internalMessage->setText(message);
    customWindow->setMainWidget(internalMessage);
    customWindow->setWindowModality(Qt::ApplicationModal);
    customWindow->show();

    //move the window to the better position, otherwise its titlebar in (0,0) position.
    customWindow->move(customWindow->x() + 100,customWindow->y() + 150);

    connect(internalMessage,&QMessageBox::buttonClicked,this,&Message::closeMessage);

    connect(customWindow,&WindowWrapper::closeWrapper,this,&Message::closeMessage);
}


Message::~Message()
{

}



void Message::exec()
{
    eventLoop->exec();
}


void Message::closeMessage()
{
    internalMessage->close();
    customWindow->close();
    if (eventLoop && eventLoop->isRunning())
    {
        eventLoop->exit();
    }

}



