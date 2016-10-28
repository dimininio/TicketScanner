#ifndef WIDGETMEDIATOR
#define WIDGETMEDIATOR
#include <memory>
class SettingsPage;
class BrowserPage;
class ProcessingPage;
class QStackedWidget;
class QWidget;
class QString;
class QDate;
class SearchParameters;
class Train;

#include <QVector>


class WidgetsMediator
{
public:
    WidgetsMediator();
    ~WidgetsMediator();

    QWidget* getStartWidgets();

    void showBrowserPage();
    void showSettingPage();
    void showProcessingPage();

    //void setSearchParameters(std::shared_ptr<SearchParameters> &p);
    void setSearchParametersAndRunSearsh();

    void showAvailableTrains();
    void showAvailableCoaches(Train *train);

    void resetSearch();

    QString getStationFrom();
    QString getStationTo();
    QString getStationIDFrom();
    QString getStationIDTo();
    QDate tripDate();

    const QVector<QString>& getChosenTrains() const;
    std::shared_ptr<SearchParameters> searchParameters;

private:
    BrowserPage* browserPage;
    SettingsPage* settingsPage;
    ProcessingPage* processingPage;
    QStackedWidget* stackedWidget;
};


#endif // WIDGETMEDIATOR

