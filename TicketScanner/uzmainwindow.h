#ifndef UZMAINWINDOW_H
#define UZMAINWINDOW_H

#include <QMainWindow>
class Train;
//class NetworkManager;


class WidgetsMediator;



class UZMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UZMainWindow(QWidget *parent = 0);
    ~UZMainWindow();


    void showAvailableTrains();
    void showAvailableCoaches(Train* train);



public slots:
    //void error(QNetworkReply::NetworkError err);
    //void connectionErrorSlot();


private:
    WidgetsMediator* widgetsMediator;
    //NetworkManager* p_networkManager;
};

#endif // UZMAINWINDOW_H
