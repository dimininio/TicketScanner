#ifndef UZMAINWINDOW_H
#define UZMAINWINDOW_H

#include <QMainWindow>
class Train;

namespace Ui {
class UZMainWindow;
}


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


private:
    Ui::UZMainWindow *ui;
    WidgetsMediator* widgetsMediator;
};

#endif // UZMAINWINDOW_H
