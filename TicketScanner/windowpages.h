#ifndef WINDOWPAGES
#define WINDOWPAGES

#include <QWidget>
#include <QCheckBox>
#include <QRadioButton>
#include "requestdata.h"

class QPushButton;
class QDateEdit;
class NetworkManager;
class QNetworkReply;
class LineEdit;

class QGridLayout;
class QHBoxLayout;
class QRadioButton;
//class QCheckBox;
class Train;

class SearchParameters;
class QLabel;
class QWebView;
class QFrame;

class WidgetsMediator;
class AnimatedSearchWidget;
class UZApplication;


class BasePage
{
public:
    virtual ~BasePage(){}

    virtual WidgetsMediator* mediator()
    {
        return _widgetsMediator;
    }

    virtual bool isChanged(){return false;}
    virtual void saveState() {}

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
    bool isChanged();

    QPushButton* showSettingsButton;
private:

    QPushButton* searchButton;

    QDateEdit* dateField;
   // NetworkManager* networkManager;
    QNetworkReply* searchReply;
    LineEdit* editFrom;
    LineEdit* editTo;

    QWebView* webView;
    QFrame* webFrame;

    void showSettings();
    void processTrain(const QUrl& link);
    void processLink(const QUrl& link);

    bool checkConditions();

    //keep information about previous widget's state.
    QString previousState_fromStation;
    QString previousState_toStation;
    void saveState();

    friend class WidgetsMediator;
};



class SettingsPage: public QWidget, public BasePage
{
public:
    SettingsPage(WidgetsMediator* widgetsMediator,QWidget* parent=0);

    void startScanner();
    void showBrowser();
    bool isChanged();

    QPushButton* startSearchBtn;
    QPushButton* showBrowserBtn;
private:
    QRadioButton* allTrainsBtn;
    QRadioButton* oneTrainBtn;

    BrowserPage* searchConfiguration;

    void getTrainsOnRoute(QNetworkReply *reply, RequestType::Request id);
    void drawTrainsWidgets(QVector<Train> &trains, QVector<QString> &places);
    void onRadioButtonClick();
    bool checkConditions();

    void exploreRout();

    QVector<QCheckBox*> trainsGroup;
    QGridLayout* trainsGroupLayout;

    QVector<QCheckBox*> coachesTypes;
    QHBoxLayout* coachesTypesLayout;

    //keep information about previous widget's state:
    QVector<QString> prevState_trainsGroup;
    QVector<QString> prevState_coachesTypes;
    bool prevState_allTrainBtn = false;
    void saveState();

    friend class WidgetsMediator;
};



class ProcessingPage:public QWidget, public BasePage
{
public:
    ProcessingPage(WidgetsMediator* widgetsMediator, QWidget* parent=0);

    void showSettings();
    void updatePage();
    void openBrowser();

private:
    QLabel* infoLabel;
    QLabel* statusLabel;
    QLabel* warningLabel;
    QPushButton* showSettingsButton;
    QPushButton* openDefaultBrowserButton;
    AnimatedSearchWidget* animatedSearchWidget;
    bool searchStatus;
};

#endif // WINDOWPAGES

