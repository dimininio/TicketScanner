#ifndef WINDOWWRAPPER_H
#define WINDOWWRAPPER_H

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {
class WindowWrapper;
}

class WindowWrapper : public QWidget
{
    Q_OBJECT

public:
    explicit WindowWrapper(QWidget *parent = 0);
    ~WindowWrapper();

    void setMainWidget(QWidget* w);

private:
    Ui::WindowWrapper *ui;
    QHBoxLayout innerLayout;
};

#endif // WINDOWWRAPPER_H
