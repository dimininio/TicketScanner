#include "lineedit.h"
#include "application.h"
#include "networkmanager.h"

#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringListModel>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "parser.h"

int LineEdit::senderIdentifier = 0;

LineEdit::LineEdit(QWidget *parent):
    QLineEdit(parent),
    currentBegin(""),
    reply(nullptr)
{
    currentCompleter = new QCompleter(this);

    networkManager = Application::instance()->networkManager();
    p_identifier = ++senderIdentifier;

    connect(this,SIGNAL(textEdited(QString)),this,SLOT(checkContent()));
    connect(networkManager,&NetworkManager::responseReady,this,&LineEdit::updateContent);
}

void LineEdit::checkContent()
{
    //Request sends when length of station more than 2.
    //TODO: find a way of good-looking updating of completer.

    //if (text().length()>=2 && currentBegin!=text().mid(0,2).toLower()) {
    if (text().length()>=2 && text().length()<=4 && currentBegin!=text().toLower()) {
        currentBegin = text().toLower();
        networkManager->sendGetStationsRequest(currentBegin,identifier());
        //qDebug()<<"id  "<<identifier();
    }

    //QUrl decode cyrillic letters automatically
    //networkManager->sendGetStationsRequest(QUrl::toPercentEncoding(currentBegin),identifier());
}

void LineEdit::updateContent(QNetworkReply *reply, RequestType::Request id)
{
    if (reply==nullptr) return;

    if(id!=identifier()) return;

    QByteArray data = reply->readAll();
    //qDebug()<<"cities :  "<< data;


    QStringList stationsList;
    //stationsList.clear();


    QMap<QString,QString> tempStations;
    if(!Parser::parseStations(data,tempStations))
    {
        qDebug()<<"parse error. LineEdit";
        return;
    }

    if (tempStations!=stations)
        stations.swap(tempStations);
    else
        return;


    for (auto p = stations.begin();p!=stations.end();++p)
        stationsList<<p.key();


    delete currentCompleter;
    currentCompleter = new QCompleter(stationsList,this);
    currentCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    //currentCompleter->setModel(new QStringListModel(stationsList));
    setCompleter(currentCompleter);
    delete reply;
    //reply->deleteLater();
    reply = nullptr;
}

QString LineEdit::getStationID() const
{
    return stations[this->text()];
}

RequestType::Request LineEdit::identifier()
{
    switch (p_identifier){
        case 1:
                return RequestType::GetStationsFrom;
        case 2:
                return RequestType::GetStationsTo;
    }

    return RequestType::Invalid;
}


LineEdit::~LineEdit()
{

}


