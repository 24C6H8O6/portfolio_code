#include "traintime.h"
#include "ui_traintime.h"

trainTime::trainTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::trainTime)
{
    ui->setupUi(this);


}

trainTime::~trainTime()
{
    delete ui;
}

// 이전 버튼 누를시 열차시각 페이지 hide
void trainTime::on_traintimeBackBtn_clicked()
{
//    ui->verticalLayout_7->clear();
    this->hide();
}
