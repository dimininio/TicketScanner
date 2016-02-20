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

class SearchParameters;
class QLabel;

class WidgetsMediator;


class BasePage
{
public:
    BasePage(WidgetsMediator* mediator)
        :_widgetsMediator(mediator) {}

    virtual ~BasePage(){}

    virtual WidgetsMediator* mediator()
    {
        return _widgetsMediator;
    }
private:
    WidgetsMediator* _widgetsMediator;
};



class TrainSearchPage: public QWidget, public BasePage
{
public:
    TrainSearchPage(WidgetsMediator* widgetsMediator,QWidget* parent=0);

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



class ScannerPage: public QWidget, public BasePage
{
public:
    ScannerPage(WidgetsMediator* widgetsMediator,QWidget* parent=0);

    void startScanner();

    QPushButton* startSearchBtn;
private:
    QRadioButton* allTrainsBtn;
    QRadioButton* oneTrainBtn;

    TrainSearchPage* searchConfiguration;

    void getTrainsOnRoute(QNetworkReply *reply, QByteArray id);
    void onRadioButtonClick();

    QVector<QCheckBox*> trainsGroup;
    QGridLayout* trainsGroupLayout;

    QVector<QCheckBox*> coachesTypes;
    QHBoxLayout* coachesTypesLayout;

    void exploreRout();
};



class ProcessingPage:public QWidget, public BasePage
{
public:
    ProcessingPage(WidgetsMediator* widgetsMediator, QWidget* parent=0);
    void setSearchStatus(bool isFound);

private:
    QLabel* infoLabel;
    QLabel* statusLabel;
    bool searchStatus;
};

#endif // WINDOWPAGES

