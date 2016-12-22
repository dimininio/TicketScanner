#include "animatedsearchwidget.h"
#include "application.h"

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
/*
int AnimatedSearchWidget::searchStatus()
{
    return status;
}
*/
void AnimatedSearchWidget::updateSearchStatus()
{
   // status = searchStatus;
    Application::SearchStatus status = Application::instance()->status();
    switch (status) {
    case Application::SearchStatus::Search:
        moveAnimation->start();
        break;
    default:
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

    painter.setBrush(QBrush(QColor(145,217,211)));
    painter.setPen(Qt::transparent);

    painter.drawRect(QRectF(currentInternalPosition,0.0,lineWidth,lineHeight));
    //painter.setPen(Qt::blue);
    //painter.drawLine(currentInternalPosition,0.0,currentInternalPosition+lineWidth,0.0);
    //painter.drawLine(currentInternalPosition,1,currentInternalPosition+lineWidth,1);
    //painter.drawLine(currentInternalPosition,lineHeight,currentInternalPosition+lineWidth,lineHeight);
}
