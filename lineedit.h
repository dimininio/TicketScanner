#ifndef LINEEDIT_H
#define LINEEDIT_H
#include <QLineEdit>
#include <QCompleter>
#include <QString>
#include <QMap>

class NetworkManager;
class QNetworkReply;
class QByteArray;

class LineEdit: public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget* parent = 0);
    ~LineEdit();

    QString getStationID();
    QByteArray identifier();

public slots:
    void checkContent();

private slots:
    void updateContent(QNetworkReply* reply,QByteArray id);

signals:
    void getStations(QString prefix);

private:
    QCompleter *currentCompleter;
    QString currentBegin;
    NetworkManager* networkManager;

    QNetworkReply* reply;
    QMap<QString,QString> stations;
    static int senderIdentifier;
    int p_identifier;

};

#endif // LINEEDIT_H

