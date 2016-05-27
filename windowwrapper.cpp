#include "windowwrapper.h"
#include "ui_windowwrapper.h"
#include "uzapplication.h"

WindowWrapper::WindowWrapper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowWrapper)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);

    connect(ui->titleBar,&WindowTitleBar::closeApp,UZApplication::instance(),&UZApplication::quit);
}

WindowWrapper::~WindowWrapper()
{
    delete ui;
}


void WindowWrapper::setMainWidget(QWidget *w)
{
    innerLayout.setMargin(2);
    innerLayout.addWidget(w);
    ui->bodyWindowWidget->setLayout(&innerLayout);
}
