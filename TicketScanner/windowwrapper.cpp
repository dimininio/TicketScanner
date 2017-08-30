#include "windowwrapper.h"
#include "application.h"
#include "config.h"

#include <QtWidgets/QVBoxLayout>
#include <QGraphicsDropShadowEffect>



WindowWrapper::WindowWrapper(bool showHelp, bool showMinimize, bool showAbout, QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMaximumWidth(Config::ApplicationWidth);


    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0);

    titleBar = new WindowTitleBar(showHelp,showMinimize,showAbout,this);
    titleBar->setObjectName(QStringLiteral("titleBar"));

    bodyWindowWidget = new QWidget(this);
    bodyWindowWidget->setObjectName(QStringLiteral("bodyWindowWidget"));

    verticalLayout->addWidget(titleBar);
    verticalLayout->addWidget(bodyWindowWidget);

    QGraphicsDropShadowEffect *titleShadow = new QGraphicsDropShadowEffect(this);
    titleShadow->setBlurRadius(9.0);
    titleShadow->setColor(QColor(70, 140, 137, 130));
    titleShadow->setOffset(4.0);
    titleBar->setGraphicsEffect(titleShadow);

    QGraphicsDropShadowEffect *bodyWidgetShadow = new QGraphicsDropShadowEffect(this);
    bodyWidgetShadow->setBlurRadius(9.0);
    bodyWidgetShadow->setColor(QColor(0, 0, 0, 160));
    bodyWidgetShadow->setOffset(4.0);
    bodyWindowWidget->setGraphicsEffect(bodyWidgetShadow);

    setAttribute(Qt::WA_NoSystemBackground);
    connect(titleBar,&WindowTitleBar::closeApp,this,&WindowWrapper::closeWrapper);
    connect(titleBar,&WindowTitleBar::minimize,this,&WindowWrapper::showMinimized);
    connect(titleBar,&WindowTitleBar::help,this,&WindowWrapper::helpWrapper);
    connect(titleBar,&WindowTitleBar::about,this,&WindowWrapper::aboutWrapper);
    //For OS_X:
    //It is necessary to use Qt 5.5.1 or higher for building
    //There are some bug with minimizing, and shadows for previous version

}

WindowWrapper::~WindowWrapper()
{
    delete bodyWindowWidget;
    delete titleBar;
}


void WindowWrapper::setMainWidget(QWidget *w)
{
    innerLayout.setMargin(0);
    innerLayout.addWidget(w);
    bodyWindowWidget->setLayout(&innerLayout);

}
