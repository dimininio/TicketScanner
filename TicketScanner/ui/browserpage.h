#ifndef BROWSERPAGE_H
#define BROWSERPAGE_H

#include <QWidget>

namespace Ui {
class BrowserPage;
}

class BrowserPage : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserPage(QWidget *parent = 0);
    ~BrowserPage();

private:
    Ui::BrowserPage *ui;
};

#endif // BROWSERPAGE_H
