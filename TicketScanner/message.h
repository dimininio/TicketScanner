#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>

/*
Message is a simple class for messages outputting.
TicketScanner has own window style, but QMessageBox uses standard window.
So, this class wraps QMessageBox by WindowWrapper for similar displaying.
*/


class QMessageBox;
class WindowWrapper;
class QEventLoop;

class Message : public QWidget
{
    Q_OBJECT
public:
    explicit Message(QString message,QWidget *parent = nullptr);
    ~Message();

    void exec();
    void closeMessage();

private:
    QMessageBox* internalMessage;
    WindowWrapper* customWindow;
    QEventLoop*    eventLoop;

};

#endif // MESSAGE_H
