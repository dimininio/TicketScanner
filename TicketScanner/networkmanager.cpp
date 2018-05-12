#include "networkmanager.h"
#include <QWebPage>
#include <QWebView>
#include <QWebFrame>
#include <QtWebKitWidgets>
#include <QNetworkReply>

#include "message.h"
#include <QDebug>
#include <QUrl>
#include "application.h"
//#include <QNetworkProxy>

static const QUrl searchURL("https://booking.uz.gov.ua/train_search/");
static const QUrl coachesURL("https://booking.uz.gov.ua/train_wagons/");
static const QUrl coachURL("https://booking.uz.gov.ua/train_wagon/");
static const QUrl addToCartURL("https://booking.uz.gov.ua/cart/add/");
static const QString stationsURL = "https://booking.uz.gov.ua/train_search/station/";
static const QByteArray host = "booking.uz.gov.ua";
static const QByteArray bookingUZ = "https://booking.uz.gov.ua/";
static const QByteArray originURL = "https://booking.uz.gov.ua";
static const QByteArray connectionType = "keep-alive";
static const QByteArray contentType = "application/x-www-form-urlencoded";
static const QByteArray gv_ajax = "1";
static const QByteArray acceptLanguage = "ua-UA,*";



/* ENglish
static const QUrl searchURL("http://booking.uz.gov.ua/en/purchase/search/");
static const QString stationsURL = "http://booking.uz.gov.ua/en/purchase/station/";
static const QUrl coachesURL("http://booking.uz.gov.ua/en/purchase/coaches/");
static const QUrl coachURL("http://booking.uz.gov.ua/en/purchase/coach/");
static const QByteArray host = "booking.uz.gov.ua";
static const QByteArray bookingUZ = "http://booking.uz.gov.ua/en/";
static const QByteArray originURL = "http://booking.uz.gov.ua";
static const QByteArray connectionType = "keep-alive";
static const QByteArray contentType = "application/x-www-form-urlencoded";
static const QByteArray gv_ajax = "1";
*/


/* RUssian
static const QUrl searchURL("http://booking.uz.gov.ua/ru/purchase/search/");
static const QString stationsURL = "http://booking.uz.gov.ua/ru/purchase/station/";
static const QUrl coachesURL("http://booking.uz.gov.ua/ru/purchase/coaches/");
static const QUrl coachURL("http://booking.uz.gov.ua/ru/purchase/coach/");
static const QByteArray host = "booking.uz.gov.ua";
static const QByteArray bookingUZ = "http://booking.uz.gov.ua/ru/";
static const QByteArray originURL = "http://booking.uz.gov.ua";
static const QByteArray connectionType = "keep-alive";
static const QByteArray contentType = "application/x-www-form-urlencoded";
static const QByteArray gv_ajax = "1";
*/



NetworkManager::NetworkManager(QObject *parent):
    QNetworkAccessManager(parent),
    networkReply(nullptr)
{
    QWebSettings *defsetting = QWebSettings::globalSettings();
    defsetting->setAttribute(QWebSettings::LocalStorageEnabled,true);

    connectionTimer = new QTimer(this);
    connectionTimer->setTimerType(Qt::VeryCoarseTimer);
    connectionTimer->setSingleShot(true);
    connectionTimer->setInterval(20000); //time to wait for downloading booking.uz.gov.ua web-page

    hiddenView = new QWebView();
    updateAttributes();
/*
 * Proxy settings for Fiddler, for example
    QNetworkProxy * proxy = new QNetworkProxy() ;
    proxy->setType(QNetworkProxy::HttpProxy);
    proxy->setHostName("127.0.0.1");
    proxy->setPort(8888);
    QNetworkProxy::setApplicationProxy(*proxy);
*/

    connect(hiddenView,SIGNAL(loadFinished(bool)),this,SLOT(getAttributes(bool)));
    connect(this,&NetworkManager::finished,this,&NetworkManager::replyHandling);
    connect(this,&NetworkManager::connectionDoesNotWork,this,&NetworkManager::connectionErrorSlot);
    connect(connectionTimer,SIGNAL(timeout()),this,SLOT(tooLongWaiting()));

    //connect(networkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(error(QNetworkReply::NetworkError)));
}


NetworkManager::~NetworkManager()
{
    delete hiddenView;
    QWebSettings::clearMemoryCaches();
}


//TicketScanner should work despite the network problems.
//If internet would dropped, our keys(token,cookie) will be invalidated.
//So, we should get new keys as soon as internet will be restored.
//updateAttributes() simulates update of the web-browser page (F5).
void NetworkManager::updateAttributes()
{
    //hiddenView->reload();
    QNetworkRequest request;
    request.setUrl(QUrl(bookingUZ));
    request.setRawHeader("Host",host);
    request.setRawHeader("Connection",connectionType);
    request.setRawHeader("Referer",bookingUZ);
    request.setRawHeader("Accept-Language","ua-UA,*");
    hiddenView->load(request);
    connectionTimer->start();

    //Some HTTP-headers added to request in the "void QHttpNetworkConnectionPrivate::prepareRequest".
    //Accept-Language depends from the localization (QLocale class)
    //So we force this header for correct work, in other way "token" and "session-id" will be incorrect

}

void NetworkManager::getAttributes(bool ok)
{
    if (!ok)  emit this->connectionDoesNotWork();
    connectionTimer->stop();

    QWebFrame *frame = hiddenView->page()->mainFrame();
    frame->setParent(this);
    //QWebPage* startpage = new QWebPage(hiddenView->page());
    //token.clear();
    //QVariant vartoken = frame->evaluateJavaScript("localStorage.getItem('gv-token')");
    //token.append(vartoken.toString());
    cookies = hiddenView->page()->networkAccessManager()->cookieJar()->cookiesForUrl(QUrl(bookingUZ));

    //qDebug()<<"token  "<<token;
    for(QList<QNetworkCookie>::iterator it = cookies.begin(); it!=cookies.end(); it++)
    {
       qDebug()<<it->name()<<"  --  "<<it->value();
    }

    emit this->networkManagerReady();
    //frame->deleteLater(); crashed during finishing
   // delete startpage;
}



void NetworkManager::sendGetStationsRequest(QString prefix, RequestType::Request sender)
{
    QNetworkRequest request;

    //request.setUrl(QUrl(stationsURL + prefix + "/"));
    request.setUrl(QUrl(stationsURL + "?term=" + prefix));  //POST changed --> GET 23/03/2017
    request.setRawHeader("Host",host);
    request.setRawHeader("Connection",connectionType);
    //request.setRawHeader("Origin",originURL);
    request.setRawHeader("Referer",bookingUZ);
    request.setRawHeader("Sender",RequestType::getStringByRequestType(sender)); //get QByteArray equivalent to RequestType
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));
    //post(request,"");

    get(request);


    //return this->post(request,"");
}


void NetworkManager::sendSearchRequest(SearchPOSTData searchdata,RequestType::Request sender)
{
    QNetworkRequest request;

    request.setUrl(searchURL);
    request.setRawHeader("Host",host);
    request.setRawHeader("GV-Token",token);
    request.setRawHeader("Connection",connectionType);
    request.setRawHeader("Origin",originURL);
    request.setRawHeader("Content-Type",contentType);
    request.setRawHeader("GV-Ajax",gv_ajax);
    request.setRawHeader("GV-Referer",bookingUZ);
    request.setRawHeader("Referer",bookingUZ);
    request.setRawHeader("Accept-Language","ua-UA,*");
    request.setRawHeader("Sender",RequestType::getStringByRequestType(sender));
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));


    QString postBody = "from=" + searchdata.stationFrom +
                       "&to=" + searchdata.stationTo +
                       "&date=" + searchdata.tripDate +
                       "&time=00%3A00&get_tpl=1";


    qDebug()<<postBody;
    QByteArray bytearrayPOST;bytearrayPOST.append(postBody);

    networkReply = post(request,bytearrayPOST);
    if (networkReply) {
        connect(networkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(errorSlot(QNetworkReply::NetworkError)));
    }
   // post(request,bytearrayPOST);
    //return networkReply;
    //return this->post(request,bytearrayPOST);
}

QString URLencode(QString str)
{
    QUrl urldata(str);
    return urldata.toEncoded();
}

void NetworkManager::sendCoachesRequest(CoachesPOSTData postdata, RequestType::Request sender)
{
    QNetworkRequest request;
    QByteArray train;train.append(postdata.train);
    QByteArray coachType;coachType.append(postdata.coachType);


    request.setUrl(coachesURL);
    request.setRawHeader("Host",host);
    request.setRawHeader("GV-Token",token);
    request.setRawHeader("Connection",connectionType);
    request.setRawHeader("Origin",originURL);
    request.setRawHeader("Content-Type",contentType);
    request.setRawHeader("GV-Ajax",gv_ajax);
    request.setRawHeader("GV-Referer",bookingUZ);
    request.setRawHeader("Referer",bookingUZ);
    request.setRawHeader("Sender",RequestType::getStringByRequestType(sender));
    request.setRawHeader("Train",train);
    request.setRawHeader("CoachType",coachType);
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));


    QString postBody = "from=" + postdata.stationFrom +
                       "&to=" + postdata.stationTo +
                       "&date=" + postdata.tripDate +
                       "&train=" + URLencode(postdata.train) +
                       "&wagon_type_id=" + URLencode(postdata.coachType) +
                       "&get_tpl=1";


    qDebug()<<postBody;
    QByteArray bytearrayPOST;bytearrayPOST.append(postBody);

    networkReply = post(request,bytearrayPOST);
    if (networkReply) {
        connect(networkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(errorSlot(QNetworkReply::NetworkError)));
    }

 }


void NetworkManager::sendCoachRequest(CoachPOSTData postdata, RequestType::Request sender)
{
    QNetworkRequest request;

    request.setUrl(searchURL);
    request.setRawHeader("Host",host);
    request.setRawHeader("GV-Token",token);
    request.setRawHeader("Connection",connectionType);
    request.setRawHeader("Origin",originURL);
    request.setRawHeader("Content-Type",contentType);
    request.setRawHeader("GV-Ajax",gv_ajax);
    request.setRawHeader("GV-Referer",bookingUZ);
    request.setRawHeader("Referer",bookingUZ);
    request.setRawHeader("Sender",RequestType::getStringByRequestType(sender));
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));


    QString postBody = "from=" + postdata.stationFrom +
                       "&to=" + postdata.stationTo +
                       "&train=" + postdata.train +
                       "&date=" + postdata.tripDate +
                       "&wagon_num=" + postdata.coachNum +
                       "&wagon_class=" + postdata.coachType;


    qDebug()<<postBody;
    QByteArray bytearrayPOST;bytearrayPOST.append(postBody);

    networkReply = post(request,bytearrayPOST);
    if (networkReply) {
        connect(networkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(errorSlot(QNetworkReply::NetworkError)));
    }
}

void NetworkManager::sendAddToCartRequest(AddToCartPOSTData postdata, RequestType::Request sender)
{
    QNetworkRequest request;

    request.setUrl(addToCartURL);
    request.setRawHeader("Host",host);
    request.setRawHeader("Connection",connectionType);
    request.setRawHeader("Origin",originURL);
    request.setRawHeader("Content-Type",contentType);
    request.setRawHeader("Referer",bookingUZ);
    request.setRawHeader("Sender",RequestType::getStringByRequestType(sender));
    request.setHeader(QNetworkRequest::CookieHeader,QVariant::fromValue(cookies));


    QString postBody =
            "roundtrip=0&places[0][ord]=0&places[0][from]=" + postdata.stationFrom +
            "&places[0][to]=" + postdata.stationTo +
            "&places[0][train]=" + postdata.train +
            "&places[0][date]=" + postdata.tripDate +
            "&places[0][wagon_num]=" + postdata.coachNum +
            "&places[0][wagon_type]=" + postdata.coachType +
            "&places[0][firstname]=" + postdata.firstName +
            "&places[0][lastname]=" + postdata.lastName +
            "&places[0][bedding]=1&places[0][child]=&places[0][student]=&places[0][reserve]=0" +
            "&places[0][place_num]=" + postdata.placeNum;





    //roundtrip=0&places[0][ord]=0&places[0][from]=2208001&places[0][to]=2200001&places[0][train]=148Ш&places[0][date]=2018-05-03&places[0]
    //[wagon_num]=10&places[0][wagon_class]=Д&places[0][wagon_type]=П&places[0][wagon_railway]=40&places[0][charline]=Б&places[0]
    //[firstname]=Leonid&places[0][lastname]=Daras&places[0][bedding]=1&places[0][child]=&places[0][student]=&places[0][reserve]=0&places[0][place_num]=032


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
    RequestType::Request requestType = RequestType::getRequestTypeByString(identifier);
    //qDebug()<<"id   " << identifier;
    if (reply->error() == QNetworkReply::NoError)
    {
        emit responseReady(reply,requestType);
    }

}


void NetworkManager::errorSlot(QNetworkReply::NetworkError err)
{
    qDebug()<<"error: "<<err;

     qDebug()<<"error "<<networkReply->errorString();

     QVariant httpstatus = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
     qDebug()<<"status "<<httpstatus.toInt();
     if (httpstatus.toInt()==400)
         updateAttributes();

     emit connectionLost();

    //int ret = QMessageBox::critical(UZApplication::instance()->mainWindow, tr("UZ scanner"),networkReply->errorString(),QMessageBox::Ok);
}


//temporary
void NetworkManager::connectionErrorSlot()
{
    Message message("Відсутній зв'язок із сервером Укрзалізниці. Перевірте інтернет");
    message.exec();

}

void NetworkManager::tooLongWaiting()
{
    Message message("Сервер Укрзалізниці не відповідає");
    message.exec();
}


