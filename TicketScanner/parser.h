#ifndef PARSER
#define PARSER

#include "train.h"
class QByteArray;

class Parser{

public:
    Parser(){}
    ~Parser(){}

    static bool parseSearchResults(QByteArray& data,Trains &trainsContainer);
    static bool parseCoachesSearchResults(QByteArray& data,Train &trainsContainer);
    static bool parseStations(QByteArray& data,QStringList &stations);




};

#endif // PARSER

