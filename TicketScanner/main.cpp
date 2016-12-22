#include "application.h"
#include <QApplication>



int main(int argc, char *argv[])
{

    QLocale::setDefault(QLocale(QLocale::Ukrainian,QLocale::Ukraine));   

    Application uz(argc,argv);

    return uz.exec();

}


