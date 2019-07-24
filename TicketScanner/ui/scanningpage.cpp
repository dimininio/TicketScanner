#include "scanningpage.h"
#include "ui_scanningpage.h"

ScanningPage::ScanningPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanningPage)
{
    ui->setupUi(this);
}

ScanningPage::~ScanningPage()
{
    delete ui;
}
