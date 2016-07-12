#ifndef PARSER_
#define PARSER_

#include "train.h"
class QByteArray;

class Parser{

public:
    Parser(){}
    ~Parser(){}

    static bool parseSearchResults(QByteArray& data,Trains &trainsContainer);
    static bool parseCoachesSearchResults(QByteArray& data, Train &train,QString);
    static bool parseStations(QByteArray& data, QMap<QString, QString> &stations);




};

#endif // PARSER

