#include "browserpage.h"
#include "ui_browserpage.h"

BrowserPage::BrowserPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrowserPage)
{
    ui->setupUi(this);
}

BrowserPage::~BrowserPage()
{
    delete ui;
}
