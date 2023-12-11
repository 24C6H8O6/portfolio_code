#include "option.h"
#include "ui_option.h"

option::option(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::option)
{
    ui->setupUi(this);
}

option::~option()
{
    delete ui;
}
