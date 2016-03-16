#include "uzapplication.h"
#include <QApplication>

int main(int argc, char *argv[])
{
QApplication as();

    UZApplication uz(argc,argv);

    //UZApplication uz2(argc,argv);
    return uz.exec();


}


