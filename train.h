#ifndef TRAIN
#define TRAIN

#include <QVector>
#include <QMap>


class Coach
{
public:
    Coach(int num)
        :number(num){}

    int number;
    QString coachClass;
    QString coachTypeID;
    QVector<quint16> places;
    int placesNumber;


};



class Train
{
public:
    Train(QString num)
        :number(num) {}
    Train() {}

    QString number;
    QString travelTime;
    QString dateDeparture;
    QMap<QString,int> availablePlaces;
    QMap<int,Coach> coaches;
};


#endif // TRAIN

