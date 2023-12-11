#ifndef TRAINTIME_H
#define TRAINTIME_H

#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QFrame>
#include <QGroupBox>
#include <QVBoxLayout>
namespace Ui {
class trainTime;
}

class trainTime : public QWidget
{
    Q_OBJECT

public:
    Ui::trainTime *ui;
    explicit trainTime(QWidget *parent = nullptr);
    ~trainTime();


private slots:
    // 이전 버튼
    void on_traintimeBackBtn_clicked();

};

#endif // TRAINTIME_H
