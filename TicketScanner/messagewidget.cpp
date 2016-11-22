#include "messagewidget.h"
#include <QDebug>

MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent)
{
    setGeometry(0,0,300,100);

    auto w = new QWidget(this);
    w->setGeometry(30,30,50,50);

    qDebug()<<"Message construct";
}


MessageWidget::~MessageWidget()
{
    qDebug()<<"Message destruct";
}

