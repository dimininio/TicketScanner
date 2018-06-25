#ifndef PARSER_
#define PARSER_

#include "train.h"
class QByteArray;

class Parser{

public:
    Parser(){}
    ~Parser(){}

    static bool parseStations(QByteArray& data, QMap<QString, QString> &stations);
    static bool parseSearchResults(QByteArray& data,Trains &trainsContainer,QString& error, bool allPossibleTrains = false);
    static bool parseCoachesSearchResults(QByteArray& data, Train &train,QString&);
    static bool parseCoachResults(QByteArray& data, Coach &train,QString&);
    static bool parseAddToCartResults(QByteArray& data, QString&);



};

#endif // PARSER

