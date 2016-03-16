#ifndef UZAPPLICATION
#define UZAPPLICATION

#include <QtWidgets/QApplication>
#include "train.h"
#include <memory>

class UZMainWindow;
class NetworkManager;
class QNetworkReply;
class QTimer;
class SearchParameters;

//typedef QMap<QString,Train> Trains;

class UZApplication: public QApplication
{
    Q_OBJECT

public:

    UZApplication(int &argc, char **argv);
    ~UZApplication();

    static UZApplication* instance();
    static NetworkManager* networkManager();

    UZMainWindow *mainWindow;

    const Trains* trains() const;
    const Train* getTrain(QString number) const;

    bool parseSearchResults(QNetworkReply *reply, Trains &trainsContainer);
    bool parseCoachesSearchResults(QNetworkReply *reply);
    void startScanning(std::shared_ptr<SearchParameters> &parameters);


signals:
    void updateSearchStatus(bool found);
    void searchError(QString error);

private slots:
    void showWindow();
    void analizeResponse(QNetworkReply *reply, QByteArray id);

private:

    static NetworkManager* p_networkManager;
    Trains* p_trains;
    Trains* p_scanTrains;
    int p_interval;
    QTimer* timer;

    std::shared_ptr<SearchParameters> searchParameters;

    Trains* setTrains();
    Train* setTrain(QString number);
    bool checkScanningResults();
    void sendScanRequest();

};


#endif // UZAPPLICATION

