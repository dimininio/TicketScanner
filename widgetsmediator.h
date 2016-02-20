#ifndef WIDGETMEDIATOR
#define WIDGETMEDIATOR

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
#include <QObject>

class WidgetsMediator: public QObject
{
    Q_OBJECT
public:
    WidgetsMediator() {}
    ~WidgetsMediator();

    QWidget* initializeWidgets();
    void prepareScannerPage();
    void prepareProcessingPage();
    void setSearchParameters();

    void showAvailableTrains();
    void showAvailableCoaches(Train *train);

    QString getStationFrom();
    QString getStationTo();
    QString getStationIDFrom();
    QString getStationIDTo();
    QDate tripDate();

    const QVector<QString>& getChosenTrains() const;

private:
    TrainSearchPage* trainSearchPage;
    ScannerPage* scannerPage;
    ProcessingPage* processingPage;
    QStackedWidget* stackedWidget;
    SearchParameters* searchParameters;
};


#endif // WIDGETMEDIATOR

