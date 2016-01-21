#include "uzmainwindow.h"
#include "ui_uzmainwindow.h"

#include "uzapplication.h"
#include "lineedit.h"
#include "networkmanager.h"
#include "requestdata.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGridLayout>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QPushButton>
#include <QByteArray>
#include <QNetworkReply>
#include <QDebug>
#include <QMessageBox>

static const QByteArray searchRequest = "searchRequest";
static const QByteArray coachRequest = "coachRequest";
static const QByteArray coachesRequest = "coachesRequest";

UZMainWindow::UZMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UZMainWindow),
    searchReply(nullptr),
    p_identifier("mainwindow")
{
    //ui->setupUi(this);

    networkManager = UZApplication::instance()->networkManager();

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
    connect(networkManager,&NetworkManager::responseReady,this,&UZMainWindow::analizeResponse);
    connect(textBrowser,&QTextBrowser::anchorClicked,this,&UZMainWindow::trainChosen);

}


UZMainWindow::~UZMainWindow()
{
    delete ui;
}


void UZMainWindow::ticketsSearch()
{
    QString date = dateField->date().toString("MM.dd.yyyy");
    SearchData searchdata(editFrom->getStationID(),editTo->getStationID(),date);
    searchReply = networkManager->sendSearchRequest(searchdata,searchRequest);
}



void UZMainWindow::analizeResponse(QNetworkReply *reply, QByteArray id)
{
    //switch(id) {
    //    case searchRequest: showSearchResults(reply);
   // }
    if (id == searchRequest)
        showSearchResults(reply);
}



void UZMainWindow::showSearchResults(QNetworkReply *reply)
{
    //if (searchReply==nullptr) return;

   // if(sender!=identifier()) return;

    QByteArray data = reply->readAll();

    qDebug()<<"search result: "<<data;

    textBrowser->clear();

    QString trainData;
    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);
    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();

         QJsonArray jsonTrains = jsonobject["value"].toArray();
         QJsonObject train;
         for(auto it = jsonTrains.begin();it != jsonTrains.end();++it)
         {
             train = it->toObject();
             trainData = "<a href=\"" + train["num"].toString() + "\">" +  train["num"].toString() +"</a>";
             trainData = trainData + "\t" + train["travel_time"].toString();

             QJsonArray ticketTypes = train["types"].toArray();
             QJsonObject ticketType;
             for(auto it2 = ticketTypes.begin(); it2!=ticketTypes.end();++it2)
             {
                 ticketType = it2->toObject();
                 trainData = trainData + "\t" + ticketType["letter"].toString() + ": " + QString::number(ticketType["places"].toInt());
             }

             textBrowser->append(trainData);

         }

        if (jsonobject["error"].toBool()){
            QString error = jsonobject["value"].toString();
            qDebug()<<error;
        }


    }

    //reply->deleteLater();
    //delete searchReply;
    //searchReply = nullptr;
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



}
