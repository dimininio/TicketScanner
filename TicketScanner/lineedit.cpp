#include "lineedit.h"
#include "uzapplication.h"
#include "networkmanager.h"

#include <QStringList>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringListModel>

#include <QFile>
#include <QTextStream>

int LineEdit::senderIdentifier = 0;

LineEdit::LineEdit(QWidget *parent):
    QLineEdit(parent),
    currentBegin(""),
    reply(nullptr)
{
    currentCompleter = new QCompleter(this);

    networkManager = UZApplication::instance()->networkManager();
    p_identifier = ++senderIdentifier;

    connect(this,SIGNAL(textEdited(QString)),this,SLOT(checkContent()));
    connect(networkManager,&NetworkManager::responseReady,this,&LineEdit::updateContent);
}

void LineEdit::checkContent()
{
    if (text().length()>=2 && currentBegin!=text().mid(0,2).toLower()) {
        currentBegin = text().mid(0,2).toLower();
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
    QFile file("lineedit.txt");
    file.close();
    file.open( QIODevice::WriteOnly);
    file.write(data);
    file.close();
    //qDebug()<<"cities :  "<< data;

    stations.clear();
    QStringList stationsList;
    stationsList.clear();
    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);
    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();
         QJsonArray jsonStations = jsonobject["value"].toArray();
         QJsonObject station;
         for(auto it2 = jsonStations.begin();it2 != jsonStations.end();++it2)
         {
             station = it2->toObject();
             stations.insert(station["title"].toString(), station["station_id"].toString());
             //stations.insert(station["title"].toString(), QString::number(station["station_id"].toInt()));
             stationsList<<station["title"].toString();

            // qDebug()<<"-- :  "<< station["title"].toString()<<" "<<station["station_id"].toString();
         }

    }

    delete currentCompleter;
    currentCompleter = new QCompleter(this);
    currentCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    currentCompleter->setModel(new QStringListModel(stationsList));
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
}


LineEdit::~LineEdit()
{

}


