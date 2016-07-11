#ifndef ANIMATEDSEARCHWIDGET
#define ANIMATEDSEARCHWIDGET

#include <QWidget>

class QPropertyAnimation;

class AnimatedSearchWidget: public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal position READ position WRITE setPosition)
public:
    AnimatedSearchWidget(int widgetWidth=300,QWidget* parent=0);
    ~AnimatedSearchWidget();

    qreal position();
    void setPosition(qreal pos);

  //  int searchStatus();
    //void setSearchStatus(int searchStatus);
    void updateSearchStatus();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    qreal currentInternalPosition;
    int lineWidth = 250;
    int lineHeight = 4;
    int widgetWidth;
    //int status;

    QPropertyAnimation* moveAnimation;
};



#endif // ANIMATEDSEARCHWIDGET

