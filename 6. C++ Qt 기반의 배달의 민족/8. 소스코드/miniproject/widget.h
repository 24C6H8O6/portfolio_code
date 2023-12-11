#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    QSqlDatabase mydb;
    void connClose()
    {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen()
    {
        mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/Users/user/Downloads/미니프로젝트3/새 폴더/0825/kiosk.db");

        if(!mydb.open()) {
            qDebug() << ("Failed to open the database");
            return false;
        }
        else{
            qDebug() << ("Connected.....");
            return true;
        }
    }
    // sql에서 가져온 값과 비교 후 창 띄우기
    void compareFlag();

    void changePage();

    void menu_sql();

    void option_sql(int arg1);

    void reset();
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_61_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_before_clicked();

    void on_pushButton_payment_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_plus_clicked();

    void on_pushButton_minus_clicked();

    void on_radioButton_choice1_clicked();

    void on_radioButton_choice2_clicked();

    void on_pushButton_menuSelect_clicked();

    void on_pushButton_basket_clicked();

    void on_checkBox_side1_stateChanged(int arg1);

    void on_checkBox_side2_stateChanged(int arg1);

    void on_checkBox_side3_stateChanged(int arg1);

    void on_checkBox_side4_stateChanged(int arg1);

    void on_checkBox_side5_stateChanged(int arg1);

    void on_checkBox_side6_stateChanged(int arg1);

    void on_checkBox_side7_stateChanged(int arg1);

    void on_checkBox_side8_stateChanged(int arg1);

    void on_checkBox_side9_stateChanged(int arg1);

    void on_checkBox_side10_stateChanged(int arg1);

    void on_checkBox_side11_stateChanged(int arg1);

    void on_checkBox_side12_stateChanged(int arg1);

    void on_checkBox_side13_stateChanged(int arg1);

    void on_checkBox_side14_stateChanged(int arg1);

    void on_checkBox_side15_stateChanged(int arg1);

    void on_checkBox_side16_stateChanged(int arg1);

    void on_checkBox_side17_stateChanged(int arg1);

    void on_radioButton_price1_clicked();

    void on_radioButton_price2_clicked();

    void on_radioButton_price3_clicked();

    void on_radioButton_taste1_clicked();

    void on_radioButton_taste2_clicked();


    void on_pushButton_kakao_clicked();

    void on_pushButton_naver_clicked();

    void on_pushButton_payco_clicked();

    void on_pushButton_toss_clicked();

    void on_pushButton_samsung_clicked();

    void on_pushButton_apple_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_meet_clicked();

    void on_pushButton_clicked();

protected:
    QString choice_menu;
    QString choice_inform;
    int choice_price;
    QString choice_route;
    int optioncount = 0;
    int total_price = 0;
    int total_payment = 0;
    int option1=0;
    int option2=0;
    int option3=0;
    int option4=0;
    QString way_to_pay="";
    QString final_menu="";
    QString final_option="";

private:
    Ui::Widget *ui;
    int flag=0;  // menu DB테이블 관련
    int flag2=0; // options DB테이블 관련
};
#endif // WIDGET_H
