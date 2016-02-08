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

class QGridLayout;
class QHBoxLayout;
class QRadioButton;
class QCheckBox;
class Train;

class TrainSearchPage: public QWidget
{
public:
    TrainSearchPage(QWidget* parent=0);

    void ticketsSearch();

    void showAvailableTrains();
    void showAvailableCoaches(Train* train);

    const LineEdit* fromEdit() const;
    const LineEdit* toEdit() const;
    QDate tripDate();

    QPushButton* showSettingsButton;
private:

    QCalendarWidget* calendar;
    QPushButton* searchButton;

    QDateEdit* dateField;
   // NetworkManager* networkManager;
    QNetworkReply* searchReply;
    LineEdit* editFrom;
    LineEdit* editTo;
    QTextBrowser* textBrowser;



    void processTrain(const QUrl& link);
};



class ScannerPage: public QWidget
{
public:
    ScannerPage(TrainSearchPage* trainsSearchPage,QWidget* parent=0);
    void exploreRout();
    void startScanner();


private:
    QRadioButton* allTrainsBtn;
    QRadioButton* oneTrainBtn;

    QPushButton* startSearchBtn;
    TrainSearchPage* searchConfiguration;

    void getTrainsOnRoute(QNetworkReply *reply, QByteArray id);
    void onRadioButtonClick();

    QVector<QCheckBox*> trainsGroup;
    QGridLayout* trainsGroupLayout;

    QVector<QCheckBox*> coachesTypes;
    QHBoxLayout* coachesTypesLayout;
};


#endif // WINDOWPAGES

