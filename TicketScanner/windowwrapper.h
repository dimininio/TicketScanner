#ifndef WINDOWWRAPPER_H
#define WINDOWWRAPPER_H

/*
 * The borderless window which contains(wraps) the main window of applications.
 * This allow to create your own type of window,title bar, toolbuttons (close, minimize)
*/

#include <QWidget>
#include "windowtitlebar.h"



class WindowWrapper : public QWidget
{
    Q_OBJECT
public:
    explicit WindowWrapper(QWidget *parent = 0);
    ~WindowWrapper();

    void setMainWidget(QWidget* w);

private:

    QVBoxLayout *verticalLayout;
    WindowTitleBar *titleBar;
    QWidget *bodyWindowWidget;
    QHBoxLayout innerLayout;
};

#endif // WINDOWWRAPPER_H
