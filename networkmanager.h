#ifndef NETWORKMANAGER
#define NETWORKMANAGER

#include <QNetworkAccessManager>
#include <QString>
#include <QByteArray>
#include <QWebView>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QMap>
#include "requestdata.h"

//class QNetworkCookie;
//class QWebSettings;
//enum class RequestType;









class NetworkManager: public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = 0);
    ~NetworkManager();

signals:
    void responseReady(QNetworkReply* reply,RequestType::Request identifier);
    void networkManagerReady();

public slots:
/*
    void sendGetStationsRequest(QString prefix,QByteArray sender);
    void sendSearchRequest(SearchPOSTData searchdata,QByteArray sender);
    void sendCoachesRequest(CoachesPOSTData postdata, QByteArray sender);
    void sendCoachRequest(CoachPOSTData postdata, QByteArray sender);
*/

    void sendGetStationsRequest(QString prefix,RequestType::Request sender);
    void sendSearchRequest(SearchPOSTData searchdata,RequestType::Request sender);
    void sendCoachesRequest(CoachesPOSTData postdata, RequestType::Request sender);
    void sendCoachRequest(CoachPOSTData postdata, RequestType::Request sender);


    void replyHandling(QNetworkReply* reply);

    void errorSlot(QNetworkReply::NetworkError err);
private slots:
    void getAttributes(bool ok);


private:
    QByteArray token;
    QList<QNetworkCookie> cookies;
    QWebView* hiddenView;
    QNetworkReply* networkReply;
    //void getAttributes();

    void updateAttributes();

};


#endif // NETWORKMANAGER

