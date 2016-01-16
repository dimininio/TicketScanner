#ifndef LINEEDIT_H
#define LINEEDIT_H
#include <QLineEdit>
#include <QCompleter>
#include <QString>
#include <QMap>

class NetworkManager;
class QNetworkReply;

class LineEdit: public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget* parent = 0);
    ~LineEdit();

    QString getStationID();

public slots:
    void checkContent();

private slots:
    void updateContent();

signals:
    void getStations(QString prefix);

private:
    QCompleter *currentCompleter;
    QString currentBegin;
    NetworkManager* networkManager;

    QNetworkReply* reply;
    QMap<QString,QString> stations;

};

#endif // LINEEDIT_H

