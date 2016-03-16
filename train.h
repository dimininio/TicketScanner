#ifndef TRAIN
#define TRAIN

#include <QVector>
#include <QMap>
#include <algorithm>
#include <QDebug>



class Coach
{
public:
    Coach(int num)
        :number(num){}
    Coach() = default;

    int number = 0;
    QString coachClass = "";
    QString coachTypeID = "";
    QVector<quint16> places;
    int placesNumber = 0;


};


struct FreePlaces
{
    FreePlaces(QString _placeClass,int _placesNumber)
        :placeClass(_placeClass),placesNumber(_placesNumber) {}
    FreePlaces() = default;

    QString placeClass = "";
    int placesNumber = 0;
};



class Train
{
public:
    Train(QString num)
        :number(num) {}
    Train() = default;

    bool checkComleteness()
    {
        for(auto type = freePlaces.begin();type!= freePlaces.end();++type)
       {
         auto p = std::find_if(coaches.begin(),coaches.end(),[&type](Coach& coach){return coach.coachClass==type->placeClass;});
         if (p==coaches.end()) return false;
        }
        return true;
    }

    QString number = "";
    QString travelTime = "";
    QString dateDeparture = "";
    QVector<FreePlaces> freePlaces;
    QMap<int,Coach> coaches;
};


typedef QMap<QString,Train> Trains;


#endif // TRAIN

