#ifndef UZAPPLICATION
#define UZAPPLICATION

#include <QtWidgets/QApplication>

class UZMainWindow;
class NetworkManager;

class UZApplication: public QApplication
{
    Q_OBJECT

public:

    UZApplication(int &argc, char **argv);
    static UZApplication* instance();
    ~UZApplication();

    UZMainWindow *mainWindow;

    NetworkManager* networkManager();

private slots:
    void showWindow();

private:

    NetworkManager* p_networkManager;

};


#endif // UZAPPLICATION

