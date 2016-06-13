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




class NetworkManager: public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = 0);
    ~NetworkManager();

signals:
    void responseReady(QNetworkReply* reply,RequestType::Request identifier);
    void networkManagerReady();
    void connectionDoesNotWork();

public slots:

    void sendGetStationsRequest(QString prefix,RequestType::Request sender);
    void sendSearchRequest(SearchPOSTData searchdata,RequestType::Request sender);
    void sendCoachesRequest(CoachesPOSTData postdata, RequestType::Request sender);
    void sendCoachRequest(CoachPOSTData postdata, RequestType::Request sender);

    void replyHandling(QNetworkReply* reply);
    void connectionErrorSlot();
    void errorSlot(QNetworkReply::NetworkError err);    
private slots:
    void getAttributes(bool ok);


private:
    QByteArray token;
    QList<QNetworkCookie> cookies;
    QWebView* hiddenView;
    QNetworkReply* networkReply;

    void updateAttributes();

};


#endif // NETWORKMANAGER

