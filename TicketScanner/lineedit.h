/** The LineEdit class extends the usual edit field (QLineEdit). It contains a drop-down list with
 * the railway stations for the easy selection. The LineEdit use
 * the instance of NetworkManager to update the stations list.
*/


#ifndef LINEEDIT_H
#define LINEEDIT_H
#include <QLineEdit>
#include <QCompleter>
#include <QString>
#include <QMap>
#include "requestdata.h"

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
    RequestType::Request identifier();

private slots:
    void checkContent();
    void updateContent(QNetworkReply* reply, RequestType::Request id);


private:
    QCompleter *currentCompleter; //helps to select the station by some first letters (complete the full string)
    QString currentBegin;
    NetworkManager* networkManager;

    QNetworkReply* reply;
    QMap<QString,QString> stations;
    static int senderIdentifier;
    int p_identifier;  //numeric identifier for the LineEdit instances

};

#endif // LINEEDIT_H

