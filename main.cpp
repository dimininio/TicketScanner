#include "uzapplication.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    UZApplication uz(argc,argv);
    return uz.exec();


}





/**

    QApplication a(argc, argv);
    QWebView web;
    web.setUrl(QUrl("http://booking.uz.gov.ua/en/"));
    //web.setUrl(QUrl("http://i.ua"));
    qDebug()<<web.page()->bytesReceived()<<"  -----------";
    //web.setHtml("<!DOCTYPE html><html  xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\"> <head> <title>Online reservation and purchase tickets - Ukrzaliznytsia<as> 7777 </as>  </title> </head> <body>  <div id=\"wrapper\">  <div id=\"content\"> <div class=\"bonus-intro\">Since 30.09.2013 introduced  <a >Bonus program</a> for passangers of 1st class wagons trains «INTERCITY+»  </div><div class=\"bonus-intro\">Since 30.09.2015 introduced </div></div> </div></body> </html>");
    web.show();
    QString s = web.page()->mainFrame()->toHtml();
    qDebug()<<s;
    QWebElement elem = web.page()->mainFrame()->findFirstElement("div [class=bonus-intro]");
  //  elem.
    qDebug()<<elem.toPlainText();
    qDebug()<<"-----------";

**/
