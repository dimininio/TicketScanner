#ifndef WINDOWPAGES
#define WINDOWPAGES

#include <QWidget>

class QCalendarWidget;
class QPushButton;
class QDateEdit;
class NetworkManager;
class QNetworkReply;
class LineEdit;

class QGridLayout;
class QHBoxLayout;
class QRadioButton;
class QCheckBox;
class Train;

class SearchParameters;
class QLabel;
class QWebView;

class WidgetsMediator;


class BasePage
{
public:
    virtual ~BasePage(){}

    virtual WidgetsMediator* mediator()
    {
        return _widgetsMediator;
    }

protected:
    BasePage(WidgetsMediator* mediator)
        :_widgetsMediator(mediator) {}
private:
    WidgetsMediator* _widgetsMediator;
};



class BrowserPage: public QWidget, public BasePage
{
public:
    BrowserPage(WidgetsMediator* widgetsMediator,QWidget* parent=0);

    void ticketsSearch();

    void showAvailableTrains();
    void showAvailableCoaches(Train* train);
    void showError(QString error);

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

    QWebView* webView;

    void showSettings();
    void processTrain(const QUrl& link);
    void processLink(const QUrl& link);

    friend class WidgetsMediator;
};



class SettingsPage: public QWidget, public BasePage
{
public:
    SettingsPage(WidgetsMediator* widgetsMediator,QWidget* parent=0);

    void startScanner();
    void showBrowser();

    QPushButton* startSearchBtn;
    QPushButton* showBrowserBtn;
private:
    QRadioButton* allTrainsBtn;
    QRadioButton* oneTrainBtn;

    BrowserPage* searchConfiguration;

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
    void showSettings();
    void updatePage();

private:
    QLabel* infoLabel;
    QLabel* statusLabel;
    QPushButton* showSettingsButton;
    bool searchStatus;
};

#endif // WINDOWPAGES

