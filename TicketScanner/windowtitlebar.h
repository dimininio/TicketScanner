#ifndef WINDOWTITLEBAR
#define WINDOWTITLEBAR
#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>

/*
 * The borderless window lose titlebar and possibility of moving
 * So, this class represents widget for moving the application's window

*/


class WindowTitleBar: public QWidget
{
    Q_OBJECT

public:
    explicit WindowTitleBar(bool showHelp = false, bool showMinimize = false, bool showAbout = false,QWidget *parent = 0);

signals:
    void closeApp();
    void minimize();
    void help();
    void about();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    bool   mousePressed;
    QPoint mousePos;
    QPoint wndPos;

private:
    QLabel* titleText;
    QToolButton* minimizeButton;
    QToolButton* closeButton;
    QToolButton* helpButton;
    QToolButton* aboutButton;
    QHBoxLayout* horizontalLayout;

};

#endif // WINDOWTITLEBAR

