#ifndef UZAPPLICATION
#define UZAPPLICATION

#include <QtWidgets/QApplication>
#include "train.h"

class UZMainWindow;
class NetworkManager;
class QNetworkReply;
class QTimer;
class SearchParameters;

typedef QMap<QString,Train> Trains;

class UZApplication: public QApplication
{
    Q_OBJECT

public:

    UZApplication(int &argc, char **argv);
    ~UZApplication();

    static UZApplication* instance();


    UZMainWindow *mainWindow;

    NetworkManager* networkManager();
    Trains& trains();
    const Trains& trains() const;

    void parseSearchResults(QNetworkReply *reply, Trains &trainsContainer);
    void parseCoachesSearchResults(QNetworkReply *reply);
    void startScanning(SearchParameters searchparams);

    SearchParameters* getSearchParameters(); //temporary

signals:
    void updateSearchStatus(bool found);

private slots:
    void showWindow();
    void analizeResponse(QNetworkReply *reply, QByteArray id);

private:

    NetworkManager* p_networkManager;
    Trains p_trains;
    Trains scan_trains;
    int p_interval;
    QTimer* timer;

    SearchParameters* searchParameters;

    bool checkScanningResults();
    void sendScanRequest();

};


#endif // UZAPPLICATION

