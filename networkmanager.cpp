#include "networkmanager.h"
#include <QWebPage>
#include <QWebView>
#include <QWebFrame>
//#include <QNetworkAccessManager>
#include <QtWebKitWidgets>
#include <QNetworkReply>

#include <QMessageBox>
#include <QDebug>
#include <QUrl>
#include "uzapplication.h"


static const QUrl searchURL("http://booking.uz.gov.ua/en/purchase/search/");
static const QString stationsURL = "http://booking.uz.gov.ua/en/purchase/station/";
static const QUrl coachesURL("http://booking.uz.gov.ua/en/purchase/coaches/");
static const QUrl coachURL("http://booking.uz.gov.ua/en/purchase/coach/");



NetworkManager::NetworkManager(QObject *parent):
    QNetworkAccessManager(parent),
    networkReply(nullptr)
{
    QWebSettings *defsetting = QWebSettings::globalSettings();
    defsetting->setAttribute(QWebSettings::LocalStorageEnabled,true);
    //defsetting->setAttribute(QWebSettings::LocalStorageDatabaseEnabled,true);
   // defsetting->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled,true);

    hiddenView = new QWebView();
    hiddenView->setUrl(QUrl("http://booking.uz.gov.ua/en/"));

    connect(hiddenView,SIGNAL(loadFinished(bool)),this,SLOT(getAttributes(bool)));

    connect(this,&NetworkManager::finished,this,&NetworkManager::replyHandling);
    //connect(networkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(error(QNetworkReply::NetworkError)));
}


NetworkManager::~NetworkManager()
{
    delete hiddenView;
    //delete cached resourses
}

void NetworkManager::updateAttributes()
{
    hiddenView->reload();
}

void NetworkManager::getAttributes(bool ok)
{
    if (!ok)  qDebug()<<"fatality!!";

    QWebFrame *frame = hiddenView->page()->mainFrame();
    frame->setParent(this);
    //QWebPage* startpage = new QWebPage(hiddenView->page());
    token.clear();
    QVariant vartoken = frame->evaluateJavaScript("localStorage.getItem('gv-token')");
    token.append(vartoken.toString());
    cookies = hiddenView->page()->networkAccessManager()->cookieJar()->cookiesForUrl(QUrl("http://booking.uz.gov.ua/en/"));

    qDebug()<<"token  "<<token;
    for(QList<QNetworkCookie>::iterator it = cookies.begin(); it!=cookies.end(); it++)
    {
       qDebug()<<it->name()<<"  --  "<<it->value();
    }

    emit this->networkManagerReady();
    //frame->deleteLater(); crashed during finishing
   // delete startpage;
}



QNetworkReply *NetworkManager::sendGetStationsRequest(QString prefix, QByteArray sender)
{
    QNetworkRequest request;

    request.setUrl(QUrl(stationsURL + prefix + "//"));
    request.setRawHeader("Host","booking.uz.gov.ua");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Origin","http://booking.uz.gov.ua");
    request.setRawHeader("Referer","http://booking.uz.gov.ua/en/");
    request.setRawHeader("Sender",sender);
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));
    return this->post(request,"");
}


QNetworkReply* NetworkManager::sendSearchRequest(SearchPOSTData searchdata,QByteArray sender)
{
    QNetworkRequest request;

    request.setUrl(searchURL);
    //request.setUrl(QUrl("http://booking.uz.gov.ua/en/purchase/search/"));
    request.setRawHeader("Host","booking.uz.gov.ua");
    request.setRawHeader("GV-Token",token);
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Origin","http://booking.uz.gov.ua");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("GV-Ajax","1");
    request.setRawHeader("GV-Referer","http://booking.uz.gov.ua/en/");
    request.setRawHeader("Referer","http://booking.uz.gov.ua/en/");
    request.setRawHeader("Sender",sender);
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));


    QString postBody = "station_id_from=" + searchdata.stationFrom +
                       "&station_id_till=" + searchdata.stationTo +
                       "&date_dep=" + searchdata.tripDate +
                       "&time_dep=00%3A00&time_dep_till=&another_ec=0&search=";
    qDebug()<<postBody;
    QByteArray bytearrayPOST;bytearrayPOST.append(postBody);

    networkReply = post(request,bytearrayPOST);
    if (networkReply) {
        connect(networkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(errorSlot(QNetworkReply::NetworkError)));
    }
    return networkReply;
    //return this->post(request,bytearrayPOST);
}

QString URLencode(QString str)
{
    QUrl urldata(str);
    return urldata.toEncoded();
}

void NetworkManager::sendCoachesRequest(CoachesPOSTData postdata, QByteArray sender)
{
    QNetworkRequest request;
    QByteArray train;train.append(postdata.train);
    QByteArray coachType;coachType.append(postdata.coachType);

    request.setUrl(coachesURL);
    request.setRawHeader("Host","booking.uz.gov.ua");
    request.setRawHeader("GV-Token",token);
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Origin","http://booking.uz.gov.ua");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("GV-Ajax","1");
    request.setRawHeader("GV-Referer","http://booking.uz.gov.ua/en/");
    request.setRawHeader("Referer","http://booking.uz.gov.ua/en/");
    request.setRawHeader("Sender",sender);
    request.setRawHeader("Train",train);
    request.setRawHeader("CoachType",coachType);
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));

    QString postBody = "station_id_from=" + postdata.stationFrom +
                       "&station_id_till=" + postdata.stationTo +
                       "&train=" + URLencode(postdata.train) +
                       "&coach_type=" + URLencode(postdata.coachType) +
                       "&date_dep=" + postdata.tripDate;
    qDebug()<<postBody;
    QByteArray bytearrayPOST;bytearrayPOST.append(postBody);

    networkReply = post(request,bytearrayPOST);
    if (networkReply) {
        connect(networkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(errorSlot(QNetworkReply::NetworkError)));
    }

 }


void NetworkManager::sendCoachRequest(CoachPOSTData postdata, QByteArray sender)
{
    QNetworkRequest request;

    request.setUrl(searchURL);
    //request.setUrl(QUrl("http://booking.uz.gov.ua/en/purchase/search/"));
    request.setRawHeader("Host","booking.uz.gov.ua");
    request.setRawHeader("GV-Token",token);
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Origin","http://booking.uz.gov.ua");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("GV-Ajax","1");
    request.setRawHeader("GV-Referer","http://booking.uz.gov.ua/en/");
    request.setRawHeader("Referer","http://booking.uz.gov.ua/en/");
    request.setRawHeader("Sender",sender);
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));

    QString postBody = "station_id_from=" + postdata.stationFrom +
                       "&station_id_till=" + postdata.stationTo +
                       "&train=" + postdata.train +
                       "&coach_num=" + postdata.coachNum +
                       "&coach_type_id=" + postdata.coachType +
                       "&date_dep=" + postdata.tripDate;

    qDebug()<<postBody;
    QByteArray bytearrayPOST;bytearrayPOST.append(postBody);

    networkReply = post(request,bytearrayPOST);
    if (networkReply) {
        connect(networkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(errorSlot(QNetworkReply::NetworkError)));
    }
}


void NetworkManager::replyHandling(QNetworkReply *reply)
{
    QByteArray identifier = reply->request().rawHeader("Sender");
    emit responseReady(reply,identifier);
}


void NetworkManager::errorSlot(QNetworkReply::NetworkError err)
{
    qDebug()<<"error: "<<err;

     qDebug()<<"error "<<networkReply->errorString();

     QVariant httpstatus = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
     qDebug()<<"status "<<httpstatus.toInt();
     if (httpstatus.toInt()==400)
         updateAttributes();


    //int ret = QMessageBox::critical(UZApplication::instance()->mainWindow, tr("UZ scanner"),networkReply->errorString(),QMessageBox::Ok);
}


