#include <QString>
#include <QtTest>

#include "train.h"

class TestsTicketScanner : public QObject
{
    Q_OBJECT

public:
    TestsTicketScanner() {}

private Q_SLOTS:
    void testCase1_data();
    void testCase1();
    void train();


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



QTEST_APPLESS_MAIN(TestsTicketScanner)

#include "test_main.moc"

