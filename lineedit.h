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

    QString getStationID() const;
    QByteArray identifier();

private slots:
    void checkContent();
    void updateContent(QNetworkReply* reply,QByteArray id);


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

