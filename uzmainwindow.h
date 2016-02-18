#ifndef UZMAINWINDOW_H
#define UZMAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QStackedWidget>
namespace Ui {
class UZMainWindow;
}

class QCalendarWidget;
class QPushButton;
class QDateEdit;
class NetworkManager;
class QNetworkReply;
class LineEdit;
class QTextBrowser;
class TrainSearchPage;
class ScannerPage;
class ProcessingPage;

class Train;
typedef QMap<QString,Train> Trains;

class UZMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UZMainWindow(QWidget *parent = 0);
    ~UZMainWindow();

    QByteArray identifier();

    void showAvailableTrains();
    void showAvailableCoaches(Train* train);

    void showNextPage();

public slots:
    void error(QNetworkReply::NetworkError err);


private:
    Ui::UZMainWindow *ui;
    QCalendarWidget* calendar;
    QPushButton* searchButton;
    QDateEdit* dateField;
    NetworkManager* networkManager;
    QNetworkReply* searchReply;
    LineEdit* editFrom;
    LineEdit* editTo;
    QTextBrowser* textBrowser;

    Trains* trains;
    QStackedWidget* pagesWidget;
    TrainSearchPage* trainSearchPage;
    ScannerPage* scannerPage;
    ProcessingPage* processingPage;
};

#endif // UZMAINWINDOW_H
