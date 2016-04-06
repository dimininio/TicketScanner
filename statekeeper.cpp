#include "statekeeper.h"
#include "windowpages.h"


#include <QString>

BrowserPageStateKeeper::BrowserPageStateKeeper(BrowserPage* browserPage)
{
    // QString a = browserPage->editFrom->text();
}

bool BrowserPageStateKeeper::isChanged(BrowserPage *)
{
    return true;
}

void BrowserPageStateKeeper::setState(BrowserPage *)
{

}

