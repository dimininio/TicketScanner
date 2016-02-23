#ifndef WIDGETMEDIATOR
#define WIDGETMEDIATOR
#include <memory>
class ScannerPage;
class TrainSearchPage;
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

    QWidget* initializeWidgets();
    void prepareScannerPage();
    void prepareProcessingPage();
    void setSearchParameters(std::shared_ptr<SearchParameters> &p);

    void showAvailableTrains();
    void showAvailableCoaches(Train *train);

    QString getStationFrom();
    QString getStationTo();
    QString getStationIDFrom();
    QString getStationIDTo();
    QDate tripDate();

    const QVector<QString>& getChosenTrains() const;
    std::shared_ptr<SearchParameters> searchParameters;

private:
    TrainSearchPage* trainSearchPage;
    ScannerPage* scannerPage;
    ProcessingPage* processingPage;
    QStackedWidget* stackedWidget;
};


#endif // WIDGETMEDIATOR

