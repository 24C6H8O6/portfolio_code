#ifndef PAY_H
#define PAY_H

#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>

class Book;

QT_BEGIN_NAMESPACE
namespace Ui { class Pay; }
QT_END_NAMESPACE

class Pay : public QWidget
{
    Q_OBJECT

public:
    void setBookInstance(Book *book) { this->bookWidget = book; }
    Pay(QWidget *parent = nullptr);
    ~Pay();
    void setUnderButton();

private:
    Book *bookWidget;

    Ui::Pay *ui;

    //승욱오빠
    QVector<QString> seats;

    QString trainNum;
    QString depStation;
    QString arrStation;
    QString depTime;
    QString arrTime;

    QVector<QString> seatsinfo;

private slots:
    void backmenu();
    void setScreen();
    void setScreen1();
    void test(bool);
    void payment();
    void payment_card();
    void lastpay();
    void out();
};
#endif // PAY_H
