#ifndef UZAPPLICATION
#define UZAPPLICATION

#include <QtWidgets/QApplication>
#include "train.h"
#include "requestdata.h"
#include <memory>

#include "windowwrapper.h"

class UZMainWindow;
class QSplashScreen;
class NetworkManager;
class QNetworkReply;
class QTimer;
class SearchParameters;


class Application: public QApplication
{
    Q_OBJECT

public:

    Application(int &argc, char **argv);
    ~Application();

    enum class SearchStatus{
        Waiting,
        Search,
        Found
    };

    static Application* instance();
    static NetworkManager* networkManager();

    UZMainWindow *mainWindow;

    const Trains* trains() const;
    const Train* getTrain(QString number) const;

    bool parseSearchResults(QNetworkReply *reply, Trains &trainsContainer);
    bool parseCoachesSearchResults(QNetworkReply *reply);
    void startScanning(std::shared_ptr<SearchParameters> &parameters);

    void resetTrains();

    SearchStatus status();
    void setStatus(SearchStatus status);

signals:
    void updateSearchStatus(SearchStatus);
    void searchError(QString error);

private slots:
    void showWindow();
    void showHelp();
    void analizeResponse(QNetworkReply *reply, RequestType::Request id);


protected:

    bool eventFilter(QObject* obj, QEvent* event) override;

private:

    static NetworkManager* p_networkManager;
    Trains* p_trains;
    Trains* p_scanTrains;
    int p_interval;
    QTimer* timer;
    SearchStatus searchStatus;
    WindowWrapper* windowWrapper;
    QSplashScreen* splashScreen;

    std::shared_ptr<SearchParameters> searchParameters;

    Trains* setTrains();
    Train* setTrain(QString number);
    bool checkScanningResults();
    void sendScanRequest();


};





#endif // UZAPPLICATION

