#include "mainwindowwrappe2r.h"
#include "widgetsmediator.h"
//#include <QVBoxLayout>
#include <QtWidgets/QVBoxLayout>
/*
UZMainWindow::UZMainWindow(QWidget *parent)
    :QWidget(parent)
{
    resize(500,300);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    titleBar = new WindowTitleBar(this);
    bodyWindowWidget = new QWidget(this);
    bodyWindowWidget->setObjectName("bodyWidget");
    widgetsMediator = new WidgetsMediator();

    innerLayout = new QHBoxLayout();
    setCentralWidget(widgetsMediator->getStartWidgets());

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(bodyWindowWidget);


}

UZMainWindow::~UZMainWindow()
{
    delete widgetsMediator;
}


void UZMainWindow::setCentralWidget(QWidget *w)
{
    innerLayout->setMargin(2);
    innerLayout->addWidget(w);
    bodyWindowWidget->setLayout(innerLayout);
}


void UZMainWindow::showAvailableTrains()
{
     widgetsMediator->showAvailableTrains();
}

void UZMainWindow::showAvailableCoaches(Train *train)
{
    widgetsMediator->showAvailableCoaches(train);
}


*/
