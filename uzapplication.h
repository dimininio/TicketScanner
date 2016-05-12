#ifndef UZAPPLICATION
#define UZAPPLICATION

#include <QtWidgets/QApplication>
#include "train.h"
#include "requestdata.h"
#include <memory>

class UZMainWindow;
class NetworkManager;
class QNetworkReply;
class QTimer;
class SearchParameters;
//enum class RequestType;

//typedef QMap<QString,Train> Trains;

class UZApplication: public QApplication
{
    Q_OBJECT

public:

    UZApplication(int &argc, char **argv);
    ~UZApplication();

    enum class SearchStatus{
        Waiting,
        Search,
        Found
    };

    static UZApplication* instance();
    static NetworkManager* networkManager();

    UZMainWindow *mainWindow;

    const Trains* trains() const;
    const Train* getTrain(QString number) const;

    bool parseSearchResults(QNetworkReply *reply, Trains &trainsContainer);
    bool parseCoachesSearchResults(QNetworkReply *reply);
    void startScanning(std::shared_ptr<SearchParameters> &parameters);

    SearchStatus status();

signals:
    void updateSearchStatus(SearchStatus);
    void searchError(QString error);

private slots:
    void showWindow();
    void analizeResponse(QNetworkReply *reply, RequestType::Request id);

private:

    static NetworkManager* p_networkManager;
    Trains* p_trains;
    Trains* p_scanTrains;
    int p_interval;
    QTimer* timer;
    SearchStatus searchStatus;

    std::shared_ptr<SearchParameters> searchParameters;

    Trains* setTrains();
    Train* setTrain(QString number);
    bool checkScanningResults();
    void sendScanRequest();
    void setStatus(SearchStatus status);

};




/*
enum class RequestType{
    SearchRequest,   //search available tickets beetween stations
    ScanRequest,     //periodic SearchRequest with checking
    CoachRequest,    //get available places of the coach
    CoachesRequest,  //get coaches of special type (with number of free places)
    TrainsOnRoute,    //the same as SearchRequest, but doesn't update browser and current trains
    GetStationsFrom,
    GetStationsTo
};

static QMap<RequestType,QByteArray> requestTypes;
*/




/*
class RequestTypeee
{
public:



    RequestTypee(){
        fillRequestTypes();
    }


    static QMap<RequestType::RequestTypel,QByteArray> requestTypes;

    void fillRequestTypes(){
        //requestTypes.insert(RequestType::SearchRequest,QByteArray("searchRequest"));
        requestTypes[RequestType::SearchRequest] = QByteArray("searchRequest");

        requestTypes.insert(RequestType::ScanRequest,"scanRequest");
        requestTypes.insert(RequestType::CoachRequest,"coachRequest");
        requestTypes.insert(RequestType::CoachesRequest,"coachesRequest");
    }
};
*/

#endif // UZAPPLICATION

