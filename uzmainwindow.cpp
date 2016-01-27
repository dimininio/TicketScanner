#include "uzmainwindow.h"
#include "ui_uzmainwindow.h"

#include "uzapplication.h"
#include "lineedit.h"
#include "networkmanager.h"
#include "requestdata.h"

#include <QGridLayout>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QPushButton>
#include <QByteArray>
#include <QNetworkReply>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

const QByteArray searchRequest = "searchRequest";
const QByteArray coachRequest = "coachRequest";
const QByteArray coachesRequest = "coachesRequest";

UZMainWindow::UZMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UZMainWindow),
    searchReply(nullptr),
    p_identifier("mainwindow")
{
    //ui->setupUi(this);

    networkManager = UZApplication::instance()->networkManager();
    trains = &UZApplication::instance()->trains;

    editFrom = new LineEdit(this);
    editTo = new LineEdit(this);

    calendar = new QCalendarWidget(this);
    calendar->hide();
    dateField = new QDateEdit(QDate::currentDate(),this);
    dateField->setDisplayFormat("MMM d yyyy");
    searchButton = new QPushButton("Search",this);


    textBrowser = new QTextBrowser(this);

    QWidget *centralWidget = new QWidget;

    QGridLayout *mainlayout = new QGridLayout;
    mainlayout->addWidget(editFrom,0,0);
    mainlayout->addWidget(editTo,0,2);
    mainlayout->addWidget(dateField,1,0);
    mainlayout->addWidget(searchButton,2,1);
    mainlayout->addWidget(textBrowser,3,0,3,3);

    centralWidget->setLayout(mainlayout);
    setCentralWidget(centralWidget);


    connect(searchButton,&QPushButton::clicked,this,&UZMainWindow::ticketsSearch);
    connect(textBrowser,&QTextBrowser::anchorClicked,this,&UZMainWindow::trainChosen);

}


UZMainWindow::~UZMainWindow()
{
    delete ui;
}


void UZMainWindow::ticketsSearch()
{
    QString date = dateField->date().toString("MM.dd.yyyy");
    SearchPOSTData searchdata(editFrom->getStationID(),editTo->getStationID(),date);
    searchReply = networkManager->sendSearchRequest(searchdata,searchRequest);
}



void UZMainWindow::showAvailableTrains()
{
    QString trainData;
    textBrowser->clear();

    for(auto train = trains->begin();train!=trains->end();++train)
    {
        trainData = "<a href=\"" + train->number + "\">" +  train->number +"</a>";
        trainData = trainData + "\t" + train->travelTime;
        for(auto tickets = train->availablePlaces.begin();tickets!=train->availablePlaces.end(); ++tickets)
        {
            trainData = trainData + "\t" + tickets.key() + ": " + QString::number(tickets.value());
        }
        textBrowser->append(trainData);
    }

}



void UZMainWindow::showAvailableCoaches(Train *train)
{
    qDebug()<<"run ";
    if (!train->checkComleteness()) return;
qDebug()<<"Run ";

    QString data;
    textBrowser->clear();

    //try to use algorithms....
    textBrowser->append("<span>Type \t           №        Qty</span>\n\n");
    for(auto type = train->availablePlaces.begin(); type!=train->availablePlaces.end();++type)
    {
        //textBrowser->append(type.key());
        textBrowser->append("<span>" + type.key() + "</span>");

        for(auto p = train->coaches.begin();p!= train->coaches.end(); ++p)
        {
            if (p->coachClass == type.key())
            {
                data = "\t" + QString::number(p->number)+ ":    " + QString::number(p->placesNumber);
                data ="           " + QString::number(p->number)+ ":           " + QString::number(p->placesNumber);
                //qDebug()<< p->number + "     " + p->placesNumber;
                textBrowser->append("<span>" + data + "</span>");
            }
        }
    }

}


void UZMainWindow::error(QNetworkReply::NetworkError err)
{
    qDebug()<<"error: "<<searchReply->error();
     qDebug()<<"err: "<<err;
    int ret = QMessageBox::critical(this, tr("UZ scanner"),searchReply->errorString(),QMessageBox::Ok);
}

QByteArray UZMainWindow::identifier()
{
    return p_identifier;
}


void UZMainWindow::trainChosen(const QUrl &link)
{
     qDebug()<<"clicked "<<link;
     QString trainNum = link.toString();
     Train* currentTrain = &(*trains)[trainNum];

     //QDateTime date(dateField->date());
     //uint dateInSeconds = date.toTime_t();

     for(auto p = currentTrain->availablePlaces.begin();p!=currentTrain->availablePlaces.end();++p)
     {
        CoachesPOSTData postdata(editFrom->getStationID(),editTo->getStationID(),currentTrain->dateDeparture,
                              trainNum,p.key());
        networkManager->sendCoachesRequest(postdata,coachesRequest);
     }

}
