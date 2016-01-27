#ifndef TRAIN
#define TRAIN

#include <QVector>
#include <QMap>
#include <algorithm>



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

    bool checkComleteness()
    {
       //for(const auto& type : availablePlaces)
        for(auto type = availablePlaces.begin();type!= availablePlaces.end();++type)
       {
         auto p = std::find_if(coaches.begin(),coaches.end(),[&type](Coach& coach){return coach.coachClass==type.key();});
         qDebug()<<"check.... "<<type.key();
         if (p==coaches.end()) return false;
        }

        return true;
    }

    QString number;
    QString travelTime;
    QString dateDeparture;
    QMap<QString,int> availablePlaces;
    QMap<int,Coach> coaches;
};


#endif // TRAIN

