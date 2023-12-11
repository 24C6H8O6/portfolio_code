#ifndef TRAINFEEPAGE_H
#define TRAINFEEPAGE_H

#include <QWidget>
#include <QtWidgets>

namespace Ui {
class trainFeePage;
}

class trainFeePage : public QWidget
{
    Q_OBJECT

public:
    explicit trainFeePage(QWidget *parent = nullptr);
    ~trainFeePage();
    Ui::trainFeePage *ui;
    // 열차번호와 일반실가격, 특실가격을 trainlist클래스로 부터 받아와서
    // trainfeepage 페이지에 연령별 좌석 가격 출력
private slots:
    // trainfeepage 페이지를 숨겨서 trainlist 페이지가 보이게 함
    void on_pushButton_feetolist_clicked();

};

#endif // TRAINFEEPAGE_H
