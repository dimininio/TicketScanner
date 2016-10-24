#include <QString>
#include <QtTest>
#include <QFile>

#include "../TicketScanner/train.h"
#include "../TicketScanner/parser.h"

/*
*  -- Manual tests for Parser --
* These tests checks whether Parser's logic corresponds to the  network API of booking.uz.gov.ua
* The folder ../tests/ contains some .json files. These are HTTP-bodies of the server's(booking.uz.gov.ua) response.
* If you want to update these files, you should make next steps:
*
* Steps for testing:
* 1) check if #defined macro  _PREPARATION_FOR_TESTS_ in the parser.cpp
*    It allows to create .json files with responses during programm working.
* 2) Move generated test_..._.json files from the build folder to the project folder  TicketScanner/tests/.
*   Add these files to the project resources: tests->resources->Add existing files.
* 3) Make sure, that json-files correspond to the tests functions.
*
*
*/


class TestsTicketScanner : public QObject
{
    Q_OBJECT

public:
    TestsTicketScanner() {}

private Q_SLOTS:
    void testCase1_data();
    void testCase1();
    void train();
    void parseSearchResponse();
    void parseSearchErrorResponse();
    void parseStationsResponse();
    void parseCoachesResponse();

private:

};



void TestsTicketScanner::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
    QTest::newRow("1") << QString();
    QTest::newRow("4") << QString();
}

void TestsTicketScanner::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}


void TestsTicketScanner::train()
{
    Train train;
    train.freePlaces.push_back(FreePlaces("A",27));
    train.freePlaces.push_back(FreePlaces("B",13));

    QVERIFY2(train.checkComleteness()==false,"train isn't complete..1");


    Coach coach1(1);
        coach1.coachClass="A";
        coach1.placesNumber = 3;
    Coach coach2(3);
        coach2.coachClass="A";
        coach2.placesNumber = 1;
    Coach coach3(5);
        coach3.coachClass="A";
        coach3.placesNumber = 23;
    Coach coach4(12);
        coach4.coachClass="B";
        coach4.placesNumber = 13;

    train.coaches.insert(1,coach1);
    train.coaches.insert(3,coach2);
    train.coaches.insert(5,coach3);


    QVERIFY2(train.checkComleteness()==false,"train isn't complete..2");

    train.coaches.insert(12,coach4);
    QVERIFY2(train.checkComleteness()==true,"train is complete");


}

void TestsTicketScanner::parseSearchResponse()
{
    //test_SearchReply.json is result of next request:
    //Kyiv - Lviv,  date departure 04.08.2016
    //Київ - Львів

    QFile file1(":/test_SearchReply.json");
    QVERIFY2(file1.open(QFile::ReadOnly)==true,"file reading");
    QByteArray data = file1.readAll();


    Trains trains;
    Parser parser;
    QString errors = "";

    auto result = parser.parseSearchResults(data,trains,errors);


    QVERIFY2(trains.size()==6,"number of available trains is 6");

    Train train = trains["141К"]; //K - is cyrillic letter, not latin!!

    QVERIFY2(train.number=="141К","train's number check");
    QVERIFY2(train.travelTime=="12:56","train's time check");

    //qDebug()<<"# "<<train.number<<"  "<<train.travelTime;


}


void TestsTicketScanner::parseSearchErrorResponse()
{
    //test_SearchErrorReply.json is result of next request:
    //any stations and just invalid date


    QFile file1(":/test_SearchErrorReply.json");
    QVERIFY2(file1.open(QFile::ReadOnly)==true,"file reading");
    QByteArray data = file1.readAll();


    Trains trains;
    Parser parser;
    QString errors = "";

    auto result = parser.parseSearchResults(data,trains,errors);


    QVERIFY2(trains.size()==0,"not found trains");
    QVERIFY2(!errors.isEmpty(),"errors catched. OK");


    //qDebug()<<"# "<<train.number<<"  "<<train.travelTime;


}


void TestsTicketScanner::parseStationsResponse()
{
    //test_StationReply.json is result of next request: "Ль" (in the edit field)

    QFile file1(":/test_StationReply.json");
    QVERIFY2(file1.open(QFile::ReadOnly)==true,"file reading");
    QByteArray data = file1.readAll();
    QMap<QString,QString> stations;

    Parser::parseStations(data,stations);
    QVERIFY2(stations["Львів"]=="2218000","city's index");
}



void TestsTicketScanner::parseCoachesResponse()
{
    //test_SearchCoachesReply.json is result of next request:
    //Click on the "141K" train (in the search result window)


    QFile file1(":/test_SearchCoachesReply.json");
    QVERIFY2(file1.open(QFile::ReadOnly)==true,"file reading");
    QByteArray data = file1.readAll();

    Train train;
    QString coachType = "К";
    Parser::parseCoachesSearchResults(data,train,coachType);

    //Coach coach  = train.coaches[15];

    QVERIFY2(train.coaches.size()==5,"number of free coaches in the train");
    QVERIFY2(train.coaches[15].number==15,"check parsed coach .. 1");
    QVERIFY2(train.coaches[15].placesNumber==1,"check parsed coach .. 2");
}



QTEST_APPLESS_MAIN(TestsTicketScanner)

#include "test_main.moc"

