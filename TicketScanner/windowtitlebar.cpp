#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include "windowtitlebar.h"
#include "application.h"

WindowTitleBar::WindowTitleBar(QWidget *parent): QWidget(parent)
{
    QFile styleF;
    styleF.setFileName(":/resources/titleBar.qss");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();
    setStyleSheet(qssStr);


    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);

    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(5, 5, 5, 5);

    QLabel* iconHandler = new QLabel(this);
    //iconHandler->setMinimumWidth(24);
    iconHandler->setPixmap(QPixmap(":/resources/titleBarIcon.png"));
    horizontalLayout->addWidget(iconHandler);

    titleText = new QLabel(this);
    titleText->setText("  TicketScanner");
    titleText->setObjectName(QStringLiteral("titleText"));

    //QFont font;
    //font->setPixelSize(14);
    //font->setBold(true);
    //font->setWeight(75);
    //titleText->setFont(*font);
    //titleText->setAlignment(Qt::AlignCenter);
    horizontalLayout->addWidget(titleText,Qt::AlignLeft);

    minimizeButton = new QToolButton(this);
    minimizeButton->setObjectName(QStringLiteral("minimizeButton"));

    horizontalLayout->addWidget(minimizeButton);

    closeButton = new QToolButton(this);
    closeButton->setObjectName(QStringLiteral("closeButton"));


    horizontalLayout->addWidget(closeButton);
    mousePressed = false;

    setMaximumHeight(29);// fixed height of titlebar widget
    setMinimumHeight(29);

    connect(closeButton,SIGNAL(clicked(bool)),this,SIGNAL(closeApp()));
    connect(minimizeButton,SIGNAL(clicked(bool)),this,SIGNAL(minimize()));
}

void WindowTitleBar::mousePressEvent(QMouseEvent *event)
{
    mousePressed = true;
    mousePos = event->globalPos();

    QWidget *parent = parentWidget();

    if(parent)
        wndPos = parent->pos();
}

void WindowTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    QWidget *parent = parentWidget();

    if(parent && mousePressed)
        parent->move(wndPos + (event->globalPos() - mousePos));
}

void WindowTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed = false;
}


void WindowTitleBar::paintEvent(QPaintEvent *event)
{
    QStyleOption styleOption;
    styleOption.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}
