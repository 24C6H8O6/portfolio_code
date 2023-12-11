#include "trainfeepage.h"
#include "ui_trainfeepage.h"

trainFeePage::trainFeePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::trainFeePage)
{
    ui->setupUi(this);
    QPainter painter(ui->label);

    // 선의 색상 설정
    painter.setPen(QPen(Qt::red, 2)); // 빨간색 선, 두께 2

    // 시작점과 끝점을 지정하여 선 그리기
    painter.drawLine(50, 50, 200, 50);
}

trainFeePage::~trainFeePage()
{
    delete ui;
}

// trainfeepage 페이지를 숨겨서 trainlist 페이지가 보이게 함
void trainFeePage::on_pushButton_feetolist_clicked()
{
    this->hide();
}
