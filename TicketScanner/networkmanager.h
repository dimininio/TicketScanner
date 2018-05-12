#ifndef NETWORKMANAGER
#define NETWORKMANAGER

#include <QNetworkAccessManager>
#include <QString>
#include <QByteArray>
#include <QWebView>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QTimer>
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
    void connectionLost();

public slots:

    void sendGetStationsRequest(QString prefix,RequestType::Request sender);
    void sendSearchRequest(SearchPOSTData searchdata,RequestType::Request sender);
    void sendCoachesRequest(CoachesPOSTData postdata, RequestType::Request sender);
    void sendCoachRequest(CoachPOSTData postdata, RequestType::Request sender);
    void sendAddToCartRequest(AddToCartPOSTData postdata, RequestType::Request sender);


private slots:
    void getAttributes(bool ok);

    void replyHandling(QNetworkReply* reply);
    void connectionErrorSlot();
    void errorSlot(QNetworkReply::NetworkError err);
    void tooLongWaiting();


private:
    QByteArray token;
    QList<QNetworkCookie> cookies;
    QWebView* hiddenView;
    QNetworkReply* networkReply;
    QTimer* connectionTimer;
    void updateAttributes();

};


#endif // NETWORKMANAGER

