#include "uzapplication.h"
#include "uzmainwindow.h"
#include "networkmanager.h"

UZApplication::UZApplication(int &argc, char **argv):
    QApplication(argc,argv),
    mainWindow(nullptr)
{
    p_networkManager = new NetworkManager(this);

    connect(p_networkManager,&NetworkManager::networkManagerReady,this,&UZApplication::showWindow);
}


void UZApplication::showWindow()
{
    if (!mainWindow) {
        mainWindow = new UZMainWindow();
        mainWindow->show();
     }
}


UZApplication* UZApplication::instance()
{
    return (static_cast<UZApplication*>(QCoreApplication::instance()));
}

UZApplication::~UZApplication()
{
    delete mainWindow;
   // p_networkManager->clearAccessCache();  //does solve the leak problem
    delete p_networkManager;
}


NetworkManager*  UZApplication::networkManager()
{
    return p_networkManager;
}
