#ifndef NMESSAGE
#define NMESSAGE
#include "windowwrapper.h"
#include <QWidget>
#include <QLabel>
#include "message.h"

class NMessage: public QWidget
{
public:
    NMessage(QWidget* p= nullptr)
    {
        WindowWrapper* w= new WindowWrapper();
        //Message* mes =  new  Message(w);
        //mes->show();
        QLabel* mes = new QLabel("asdasdasd\n ggggggggasdasd");
        w->setMainWidget(mes);
        w->show();

    }
};

#endif // NMESSAGE

