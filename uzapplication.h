#ifndef UZAPPLICATION
#define UZAPPLICATION

#include <QtWidgets/QApplication>
#include "train.h"

class UZMainWindow;
class NetworkManager;
class QNetworkReply;

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


private slots:
    void showWindow();
    void analizeResponse(QNetworkReply *reply, QByteArray id);

private:

    NetworkManager* p_networkManager;
    Trains p_trains;

};


#endif // UZAPPLICATION

