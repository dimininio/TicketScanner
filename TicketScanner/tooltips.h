#ifndef TOOLTIPS
#define TOOLTIPS

#include <QMap>
#include <QString>

class ToolTipsWagonTypes
{
public:
    QString getToolTip(QString key)
    {
        static QMap<QString, QString> tooltips;
        tooltips.insert("Л","Люкс");
        tooltips.insert("К","Купе");
        tooltips.insert("П","Плацкарт");
        tooltips.insert("С1","1 клас");
        tooltips.insert("С2","2 клас");
        tooltips.insert("С3","3 клас");

        return tooltips[key];
    }
};


#endif // TOOLTIPS

