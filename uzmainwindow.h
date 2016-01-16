#ifndef UZMAINWINDOW_H
#define UZMAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
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

class UZMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UZMainWindow(QWidget *parent = 0);
    ~UZMainWindow();

    void showSearchResults();

public slots:
    void ticketsSearch();
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
};

#endif // UZMAINWINDOW_H
