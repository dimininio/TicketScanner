#include "windowwrapper.h"
#include "uzapplication.h"
#include "config.h"

#include <QtWidgets/QVBoxLayout>



WindowWrapper::WindowWrapper(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMaximumWidth(Config::ApplicationWidth);


    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0);

    titleBar = new WindowTitleBar(this);
    titleBar->setObjectName(QStringLiteral("titleBar"));

    bodyWindowWidget = new QWidget(this);
    bodyWindowWidget->setObjectName(QStringLiteral("bodyWindowWidget"));

    verticalLayout->addWidget(titleBar);
    verticalLayout->addWidget(bodyWindowWidget);

    connect(titleBar,&WindowTitleBar::closeApp,UZApplication::instance(),&UZApplication::quit);
    connect(titleBar,&WindowTitleBar::minimize,this,&WindowWrapper::showMinimized);
}

WindowWrapper::~WindowWrapper()
{

}


void WindowWrapper::setMainWidget(QWidget *w)
{
    innerLayout.setMargin(0);
    innerLayout.addWidget(w);
    bodyWindowWidget->setLayout(&innerLayout);

}
