#include "uzapplication.h"
#include <QApplication>



int main(int argc, char *argv[])
{

    QLocale::setDefault(QLocale(QLocale::Ukrainian,QLocale::Ukraine));   

    UZApplication uz(argc,argv);

    return uz.exec();

}


