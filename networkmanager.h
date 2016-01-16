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

public slots:

    QNetworkReply* sendGetStationsRequest(QString prefix);
    QNetworkReply* sendSearchRequest(SearchData searchdata);

    /*
    QNetworkReply sendGet();
    QNetworkReply sendGetCoachesRequest();
*/
    void errorSlot(QNetworkReply::NetworkError err);
private slots:
    void getAttributes();

private:
    QByteArray token;
    QList<QNetworkCookie> cookies;
    QWebView* hiddenView;
    QNetworkReply* networkReply;
    //void getAttributes();

};


#endif // NETWORKMANAGER

