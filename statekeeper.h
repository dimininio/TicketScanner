#ifndef STATEKEEPER
#define STATEKEEPER

/*
 * Class keeps information about previous widget's state.
 * Used for check whether widget need to reload/update data.
 * It prevents unnecessary request to the host.
*/


class BrowserPage;
class SettingPage;
class ProcessingPage;
class WidgetsMediator;

#include <QMap>
#include <QObject>

class StateKeeper{

public:
    virtual ~StateKeeper() {}

    virtual bool isChanged()=0;
    virtual void setState()=0;

protected:
    StateKeeper() {}

};




class BrowserPageStateKeeper: public StateKeeper{
public:
    BrowserPageStateKeeper(BrowserPage* browserPage);
    ~BrowserPageStateKeeper() {}

    bool isChanged(BrowserPage*);
    void setState(BrowserPage*);

private:

};



class StateKeeper2{

public:
    virtual ~StateKeeper2() {}

    virtual bool isChanged()=0;
    virtual void setState()=0;

protected:
    StateKeeper2() {}
    QMap<QString,QObject*> checkMap;

};


void StateKeeper2::setState()
{

}

#endif // STATEKEEPER

