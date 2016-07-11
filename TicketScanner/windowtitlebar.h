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
    explicit WindowTitleBar(QWidget *parent = 0);

signals:
    void closeApp();
    void minimize();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    bool   mousePressed;
    QPoint mousePos;
    QPoint wndPos;

private:
    QLabel *titleText;
    QToolButton *minimizeButton;
    QToolButton *closeButton;
    QHBoxLayout *horizontalLayout;

};

#endif // WINDOWTITLEBAR

