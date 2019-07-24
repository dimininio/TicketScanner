#ifndef SCANNINGPAGE_H
#define SCANNINGPAGE_H

#include <QWidget>

namespace Ui {
class ScanningPage;
}

class ScanningPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScanningPage(QWidget *parent = 0);
    ~ScanningPage();

private:
    Ui::ScanningPage *ui;
};

#endif // SCANNINGPAGE_H
