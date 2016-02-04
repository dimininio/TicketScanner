#ifndef WINDOWPAGES
#define WINDOWPAGES

#include <QWidget>

class QCalendarWidget;
class QPushButton;
class QDateEdit;
class NetworkManager;
class QNetworkReply;
class LineEdit;
class QTextBrowser;

class Train;

class TrainSearchPage: public QWidget
{
public:
    TrainSearchPage(QWidget* parent=0);

    void ticketsSearch();

    void showAvailableTrains();
    void showAvailableCoaches(Train* train);


    QCalendarWidget* calendar;
    QPushButton* searchButton;
    QPushButton* showSettingsButton;
    QDateEdit* dateField;
   // NetworkManager* networkManager;
    QNetworkReply* searchReply;
    LineEdit* editFrom;
    LineEdit* editTo;
    QTextBrowser* textBrowser;


private:
    void processTrain(const QUrl& link);
};



class ScannerPage: public QWidget
{
public:
    ScannerPage(QWidget* parent=0);
};


#endif // WINDOWPAGES

