#ifndef NETWORKMANAGER
#define NETWORKMANAGER

#include <QNetworkAccessManager>
#include <QString>
#include <QByteArray>
#include <QWebView>
#include <QNetworkCookie>
#include <QNetworkReply>
#include "requestdata.h"

//class QNetworkCookie;
//class QWebSettings;

class NetworkManager: public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = 0);
    ~NetworkManager();

signals:
    void responseReady(QNetworkReply* reply,QByteArray identifier);
    void networkManagerReady();

public slots:

    QNetworkReply* sendGetStationsRequest(QString prefix,QByteArray sender);
    QNetworkReply* sendSearchRequest(SearchPOSTData searchdata,QByteArray sender);
    void sendCoachesRequest(CoachesPOSTData postdata, QByteArray sender);
    void sendCoachRequest(CoachPOSTData postdata, QByteArray sender);
    void replyHandling(QNetworkReply* reply);

    /*
    QNetworkReply sendGet();
    QNetworkReply sendGetCoachesRequest();
*/
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

