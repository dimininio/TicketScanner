#include "animatedsearchwidget.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>
AnimatedSearchWidget::AnimatedSearchWidget(int width, QWidget *parent)
    :QWidget(parent),widgetWidth(width)
{
    setMinimumHeight(10);
    setMinimumWidth(350);
    currentInternalPosition = -lineWidth;

    moveAnimation = new QPropertyAnimation(this,"position");
    moveAnimation->setStartValue(-lineWidth);
    moveAnimation->setEndValue(widgetWidth);
    moveAnimation->setDuration(3000);
    moveAnimation->setLoopCount(-1);


    //connect(moveAnimation,SIGNAL(finished()),this,SLOT(update()));


}

AnimatedSearchWidget::~AnimatedSearchWidget()
{
    delete moveAnimation;
}

qreal AnimatedSearchWidget::position()
{
    return currentInternalPosition;
}

void AnimatedSearchWidget::setPosition(qreal pos)
{
    currentInternalPosition = pos;
    repaint();
}

int AnimatedSearchWidget::searchStatus()
{
    return status;
}

void AnimatedSearchWidget::setSearchStatus(int searchStatus)
{
    status = searchStatus;
    switch (status) {
    case 0:
        moveAnimation->start();
        break;
    case 1:
        setPosition(-lineWidth-100);
        repaint();
        moveAnimation->stop();
        break;
    }
}

void AnimatedSearchWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);

    painter.drawRect(QRectF(currentInternalPosition,0.0,lineWidth,lineHeight));
    //painter.setPen(Qt::blue);
    //painter.drawLine(currentInternalPosition,0.0,currentInternalPosition+lineWidth,0.0);
    //painter.drawLine(currentInternalPosition,1,currentInternalPosition+lineWidth,1);
    //painter.drawLine(currentInternalPosition,lineHeight,currentInternalPosition+lineWidth,lineHeight);
}
