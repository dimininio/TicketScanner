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

        //reply = networkManager->sendGetStationsRequest(currentBegin);
        reply = networkManager->sendGetStationsRequest(currentBegin,identifier());
        //qDebug()<<"id  "<<identifier();
    }
}

void LineEdit::updateContent(QNetworkReply *reply, QByteArray id)
{
    if (reply==nullptr) return;
    //function start only when reply is not empty. it is correct only for 1 element;
    //reimplement signal finished() to finished(*caller);

    if(id!=identifier()) return;
//qDebug()<<"Sender  "<<reply->request().rawHeader("Sender");
    QByteArray data = reply->readAll();
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
             stations.insert(station["title"].toString(), QString::number(station["station_id"].toInt()));
             stationsList<<station["title"].toString();

             //qDebug()<<"-- :  "<< station["title"].toString()<<" "<<QString::number(station["station_id"].toInt());
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

QString LineEdit::getStationID()
{
    return stations[this->text()];
}

QByteArray LineEdit::identifier()
{
    //return QString::number(p_identifier);
    return QByteArray::number(p_identifier);
}


LineEdit::~LineEdit()
{

}



/**


void LineEdit::updateContent()
{
    if (reply==nullptr) return;
    //function start only when reply is not empty. it is correct only for 1 element;
    //reimplement signal finished() to finished(*caller);

    QByteArray data = reply->readAll();
    //qDebug()<<"cities :  "<< data;
    stations.clear();
    QStringList stationsList;
    stationsList.clear();
    QJsonDocument responce;
    responce = QJsonDocument::fromJson(data);
    if (responce.isObject()) {
        QJsonObject jsonobject = responce.object();
        for(auto it=jsonobject.begin(); it!=jsonobject.end();++it)
        {
            if (it.key()=="value" && it.value().isArray()) {
                QJsonArray jsonStations = it.value().toArray();
                QJsonObject station;
                for(auto it2 = jsonStations.begin();it2 != jsonStations.end();++it2)
                {
                    station = it2->toObject();
                    stations.insert(station["title"].toString(), QString::number(station["station_id"].toInt()));
                    stationsList<<station["title"].toString();

                    //qDebug()<<"-- :  "<< station["title"].toString()<<" "<<QString::number(station["station_id"].toInt());
                }
                break;
            }
        }

    }

    delete currentCompleter;
    currentCompleter = new QCompleter(this);
    currentCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    currentCompleter->setModel(new QStringListModel(stationsList));
    setCompleter(currentCompleter);
    delete reply;
    reply = nullptr;
}


*/
