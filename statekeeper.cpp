#include "statekeeper.h"
#include "windowpages.h"
#include "lineedit.h"

#include <QString>
//#include <QDate>

BrowserPageStateKeeper::BrowserPageStateKeeper(BrowserPage* browserPage)
    :StateKeeper(),fromStation(""),toStation("")
{
    // QString a = browserPage->editFrom->text();
}

bool BrowserPageStateKeeper::isChanged(BrowserPage* browserPage)
{
    if(fromStation!=browserPage->editFrom->text() ||
           toStation != browserPage->editTo->text())
        return true;
    else
        return false;
}

void BrowserPageStateKeeper::setState(BrowserPage* browserPage)
{
    fromStation = browserPage->editFrom->text();
    toStation = browserPage->editTo->text();
    tripDate = browserPage->tripDate();
}

