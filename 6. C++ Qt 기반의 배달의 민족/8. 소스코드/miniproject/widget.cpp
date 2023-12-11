#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QFile>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>

void Widget::compareFlag()
{
    connOpen();
    QSqlQuery query_flag;
    // menuNum, menu, inform, price, route, option1, option2, option3, option4 까지 DB에서 가져와야 함
    if (!query_flag.exec("SELECT menuNum, menu, inform, price, route, option1, option2, option3, option4 FROM menu"))
    {
        qDebug() << "Failed to execute query.";
        return;
    }
    while(query_flag.next())
    {
        int queryFlag = query_flag.value(0).toInt();    // menuNum
        choice_menu = query_flag.value(1).toString();   // menu
        choice_inform = query_flag.value(2).toString(); // inform
        choice_price = query_flag.value(3).toInt();     // price
        choice_route = query_flag.value(4).toString();  // route
        int option1 = query_flag.value(5).toInt();      // option1
        int option2 = query_flag.value(6).toInt();      // option2
        int option3 = query_flag.value(7).toInt();      // option3
        int option4 = query_flag.value(8).toInt();      // option4
        if (queryFlag==flag)
        {
            // 옵션 페이지 DB에서 불러와 생성
            QVBoxLayout* layout = new QVBoxLayout(ui->groupBox_option2);
            // 예시 코드: "your_table" 테이블에서 모든 열 선택하기 (실행 전, db 연결 필요)
            QSqlQuery query("SELECT menu, extraprice FROM options");

            while (query.next()) {
                QString choice_option = query.value(0).toString(); // 원하는 열 번호 또는 이름으로 변경 가능
                int choice_optPrice = query.value(1).toInt();
                QLabel* label = new QLabel(choice_option);
                QLabel* label2 = new QLabel(QString::number(choice_optPrice));
                layout->addWidget(label);
                layout->addWidget(label2);
            }
            ui->groupBox_selectmenu->setLayout(layout);


            ui->stackedWidget->setCurrentIndex(3);
            QPixmap choice_pic(choice_route);
            ui->label_choicepic->setPixmap(choice_pic);
            ui->label_choicemenu->setText(choice_menu);
            ui->label_choiceinform->setText(choice_inform);
            ui->label_choiceprice->setText(QString::number(choice_price));
            ui->groupBox_option1->setVisible(option1);
            ui->groupBox_option2->setVisible(option2);
            ui->groupBox_option3->setVisible(option3);
            ui->groupBox_option4->setVisible(option4);
            qDebug() << "option1 : " << option1 << "option2 : " << option2
                     << "option3 : " << option3 << "option4 : " << option4;
            break;
        }
    }
    connClose();
}

void Widget::changePage()
{
    ui->stackedWidget->setCurrentIndex(4); // 예시: 다음 페이지로 전환
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(5);
    setWindowTitle("배달의 만족");
    reset();
    // QButtonGroup 객체 생성
    QButtonGroup* buttonGroup = new QButtonGroup(this);
    QButtonGroup* buttonGroup2 = new QButtonGroup(this);
    QButtonGroup* buttonGroup3 = new QButtonGroup(this);
    // 옵션 창에 radioButton 묶기
    QRadioButton* radioButton_choice1 = ui->frame_3->findChild<QRadioButton*>("radioButton_choice1");
    buttonGroup->addButton(radioButton_choice1);
    QRadioButton* radioButton_choice2 = ui->frame_3->findChild<QRadioButton*>("radioButton_choice2");
    buttonGroup->addButton(radioButton_choice2);


    QRadioButton* radioButton_price1 = ui->frame_3->findChild<QRadioButton*>("radioButton_price1");
    buttonGroup2->addButton(radioButton_price1);
    QRadioButton* radioButton_price2 = ui->frame_3->findChild<QRadioButton*>("radioButton_price2");
    buttonGroup2->addButton(radioButton_price2);
    QRadioButton* radioButton_price3 = ui->frame_3->findChild<QRadioButton*>("radioButton_price3");
    buttonGroup2->addButton(radioButton_price3);

    QRadioButton* radioButton_taste1 = ui->frame_3->findChild<QRadioButton*>("radioButton_taste1");
    buttonGroup3->addButton(radioButton_taste1);
    QRadioButton* radioButton_taste2 = ui->frame_3->findChild<QRadioButton*>("radioButton_taste2");
    buttonGroup3->addButton(radioButton_taste2);


    // 장바구니 페이지
    // page_shopbasket 페이지 생성
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);

    // ScrollArea 생성
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true); // 스크롤 영역 크기 자동 조정

    // ScrollArea 내부의 컨테이너 위젯 생성
    QWidget* containerWidget = new QWidget();
    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget);

    // 10개의 groupBox 추가
    for (int i = 0; i < 10; ++i) {
        QGroupBox* groupBox = new QGroupBox(QString("Group Box %1").arg(i+1));
        containerLayout->addWidget(groupBox);
    }

    containerLayout->addStretch(); // 추가된 groupBox 아래 여백을 위해 stretch 추가

    scrollArea->setWidget(containerWidget); // ScrollArea에 컨테이너 위젯 설정

    layout->addWidget(scrollArea); // 페이지 레이아웃에 ScrollArea 추가

    ui->stackedWidget->addWidget(page); // stackWidget에 페이지 추가 (stackedWidget는 이미 생성되어 있다고 가정)

    // QObject::connect(ui->pushButton_menuSelect, &QPushButton::clicked, this, &Widget::changePage);



    QPixmap pix(":mainImage/image2.jpg");
    QPixmap pix2(":mainImage/image_inform.jpg");
    QPixmap pix3(":mainImage/image_review.jpg");
    QPixmap pix4(":mainImage/image_main.jpg");

    QPixmap menu1(":mainImage/menu1.jpg");
    QPixmap menu2(":mainImage/menu4.jpg");
    QPixmap menu3(":mainImage/menu5.jpg");
    QPixmap menu4(":mainImage/menu6.jpg");
    QPixmap menu5(":mainImage/menu7.jpg");

    QPixmap intro(":mainImage/image_intro2.png");

    ui->label_pic->setPixmap(pix);
    ui->label_inform->setPixmap(pix2);
    ui->label_review->setPixmap(pix3);
    ui->label_name->setPixmap(pix4);
    ui->label_menu1->setPixmap(menu1);
    ui->label_menu2->setPixmap(menu1);
    ui->label_menu3->setPixmap(menu1);
    ui->label_menu4->setPixmap(menu2);
    ui->label_menu5->setPixmap(menu2);
    ui->label_menu6->setPixmap(menu2);
    ui->label_menu7->setPixmap(menu3);
    ui->label_menu8->setPixmap(menu3);
    ui->label_menu9->setPixmap(menu3);
    ui->label_menu10->setPixmap(menu3);
    ui->label_menu11->setPixmap(menu4);
    ui->label_menu12->setPixmap(menu5);
    ui->label_10->setPixmap(intro);

    connOpen();
    QSqlQuery query;
    if (!query.exec("SELECT menu, inform, price FROM menu")) {
        qDebug() << "Failed to execute query.";
        return;
    }

    QVector<QString> menus;
    QVector<QString> informs;
    QVector<int> prices;
    while (query.next()) {
        QString menu = query.value(0).toString();
        QString inform = query.value(1).toString();
        int price = query.value(2).toInt();
        menus.append(menu);
        informs.append(inform);
        prices.append(price);
    }
    // menus, informs, prices 벡터에서 값을 가져와서 label_sample11, label_sample12, label_sample13에 출력
    if (!menus.isEmpty()) {
        // 1번 메뉴
        QString menu = menus.at(0);
        ui->label_top1->setText(menu);
        QString inform = informs.at(0);
        ui->label_mid1->setText(inform);
        int price = prices.at(0);
        ui->label_bot1->setText(QString::number(price)+"원");
        // 2번 메뉴
        QString menu1 = menus.at(1);
        ui->label_top2->setText(menu1);
        QString inform1 = informs.at(1);
        ui->label_mid2->setText(inform1);
        int price1 = prices.at(1);
        ui->label_bot2->setText(QString::number(price1)+"원");
        // 3번 메뉴
        QString menu2 = menus.at(2);
        ui->label_top3->setText(menu2);
        QString inform2 = informs.at(2);
        ui->label_mid3->setText(inform2);
        int price2 = prices.at(2);
        ui->label_bot3->setText(QString::number(price2)+"원");
        // 4번 메뉴
        QString menu3 = menus.at(3);
        ui->label_top4->setText(menu3);
        QString inform3 = informs.at(3);
        ui->label_mid4->setText(inform3);
        int price3 = prices.at(3);
        ui->label_bot4->setText(QString::number(price3)+"원");
        // 5번 메뉴
        QString menu4 = menus.at(4);
        ui->label_top5->setText(menu4);
        QString inform4 = informs.at(4);
        ui->label_mid5->setText(inform4);
        int price4 = prices.at(4);
        ui->label_bot5->setText(QString::number(price4)+"원");
        // 6번 메뉴
        QString menu5 = menus.at(5);
        ui->label_top6->setText(menu5);
        QString inform5 = informs.at(5);
        ui->label_mid6->setText(inform5);
        int price5 = prices.at(5);
        ui->label_bot6->setText(QString::number(price5)+"원");
        // 7번 메뉴
        QString menu6 = menus.at(6);
        ui->label_top7->setText(menu6);
        QString inform6 = informs.at(6);
        ui->label_mid7->setText(inform6);
        int price6 = prices.at(6);
        ui->label_bot7->setText(QString::number(price6)+"원");
        // 8번 메뉴
        QString menu7 = menus.at(7);
        ui->label_top8->setText(menu7);
        QString inform7 = informs.at(7);
        ui->label_mid8->setText(inform7);
        int price7 = prices.at(7);
        ui->label_bot8->setText(QString::number(price7)+"원");
        // 9번 메뉴
        QString menu8 = menus.at(8);
        ui->label_top9->setText(menu8);
        QString inform8 = informs.at(8);
        ui->label_mid9->setText(inform8);
        int price8 = prices.at(8);
        ui->label_bot9->setText(QString::number(price8)+"원");
        // 10번 메뉴
        QString menu9 = menus.at(9);
        ui->label_top10->setText(menu9);
        QString inform9 = informs.at(9);
        ui->label_mid10->setText(inform9);
        int price9 = prices.at(9);
        ui->label_bot10->setText(QString::number(price9)+"원");
        // 11번 메뉴
        QString menu10 = menus.at(10);
        ui->label_top11->setText(menu10);
        QString inform10 = informs.at(10);
        ui->label_mid11->setText(inform10);
        int price10 = prices.at(10);
        ui->label_bot11->setText(QString::number(price10)+"원");
        // 12번 메뉴
        QString menu11 = menus.at(11);
        ui->label_top12->setText(menu11);
        QString inform11 = informs.at(11);
        ui->label_mid12->setText(inform11);
        int price11 = prices.at(11);
        ui->label_bot12->setText(QString::number(price11)+"원");
    }
    connClose();

    connOpen();
    // 옵션창 쿼리(변경 필요)
    QSqlQuery query_option;
    if (!query_option.exec("SELECT menu, extraprice FROM options")) {
        qDebug() << "Failed to execute query.";
        return;
    }
    QVector<QString> option_menus;
    QVector<int> option_prices;
    while (query_option.next()) {
        QString option_menu = query_option.value(0).toString();
        int option_price = query_option.value(1).toInt();
        option_menus.append(option_menu);
        option_prices.append(option_price);
    }
    // menus, informs, prices 벡터에서 값을 가져와서 label_sample11, label_sample12, label_sample13에 출력
    if (!option_menus.isEmpty()) {
        // 고기 1번 옵션
        QString option_menu = option_menus.at(0);
        ui->label_meat1_name->setText(option_menu);
        int option_price = option_prices.at(0);
        ui->label_meat1_price->setText("+"+QString::number(option_price)+"원");
        // 2번 옵션
        QString option_menu1 = option_menus.at(1);
        ui->label_meat2_name->setText(option_menu1);
        int option_price1 = option_prices.at(1);
        ui->label_meat2_price->setText("+"+QString::number(option_price1)+"원");
        // 사이드 1번 옵션
        QString option_menu2 = option_menus.at(2);
        ui->label_side1_name->setText(option_menu2);
        int option_price2 = option_prices.at(2);
        ui->label_side1_price->setText("+"+QString::number(option_price2)+"원");
        // 2번
        QString option_menu3 = option_menus.at(3);
        ui->label_side2_name->setText(option_menu3);
        int option_price3 = option_prices.at(3);
        ui->label_side2_price->setText("+"+QString::number(option_price3)+"원");
        // 3번
        QString option_menu4 = option_menus.at(4);
        ui->label_side3_name->setText(option_menu4);
        int option_price4 = option_prices.at(4);
        ui->label_side3_price->setText("+"+QString::number(option_price4)+"원");
        // 4번
        QString option_menu5 = option_menus.at(5);
        ui->label_side4_name->setText(option_menu5);
        int option_price5 = option_prices.at(5);
        ui->label_side4_price->setText("+"+QString::number(option_price5)+"원");
        // 5번
        QString option_menu6 = option_menus.at(6);
        ui->label_side5_name->setText(option_menu6);
        int option_price6 = option_prices.at(6);
        ui->label_side5_price->setText("+"+QString::number(option_price6)+"원");
        // 6번
        QString option_menu7 = option_menus.at(7);
        ui->label_side6_name->setText(option_menu7);
        int option_price7 = option_prices.at(7);
        ui->label_side6_price->setText("+"+QString::number(option_price7)+"원");
        // 7번
        QString option_menu8 = option_menus.at(8);
        ui->label_side7_name->setText(option_menu8);
        int option_price8 = option_prices.at(8);
        ui->label_side7_price->setText("+"+QString::number(option_price8)+"원");
        // 8번
        QString option_menu9 = option_menus.at(9);
        ui->label_side8_name->setText(option_menu9);
        int option_price9 = option_prices.at(9);
        ui->label_side8_price->setText("+"+QString::number(option_price9)+"원");
        // 9번 옵션
        QString option_menu10 = option_menus.at(10);
        ui->label_side9_name->setText(option_menu10);
        int option_price10 = option_prices.at(10);
        ui->label_side9_price->setText("+"+QString::number(option_price10)+"원");
        // 10번 옵션
        QString option_menu11 = option_menus.at(11);
        ui->label_side10_name->setText(option_menu11);
        int option_price11 = option_prices.at(11);
        ui->label_side10_price->setText("+"+QString::number(option_price11)+"원");
        // 11번 옵션
        QString option_menu12 = option_menus.at(12);
        ui->label_side11_name->setText(option_menu12);
        int option_price12 = option_prices.at(12);
        ui->label_side11_price->setText("+"+QString::number(option_price12)+"원");
        // 12번 옵션
        QString option_menu13 = option_menus.at(13);
        ui->label_side12_name->setText(option_menu13);
        int option_price13 = option_prices.at(13);
        ui->label_side12_price->setText("+"+QString::number(option_price13)+"원");
        // 13번 옵션
        QString option_menu14 = option_menus.at(14);
        ui->label_side13_name->setText(option_menu14);
        int option_price14 = option_prices.at(14);
        ui->label_side13_price->setText("+"+QString::number(option_price14)+"원");
        // 14번 옵션
        QString option_menu15 = option_menus.at(15);
        ui->label_side14_name->setText(option_menu15);
        int option_price15 = option_prices.at(15);
        ui->label_side14_price->setText("+"+QString::number(option_price15)+"원");
        // 15번 옵션
        QString option_menu16 = option_menus.at(16);
        ui->label_side15_name->setText(option_menu16);
        int option_price16 = option_prices.at(16);
        ui->label_side15_price->setText("+"+QString::number(option_price16)+"원");
        // 16번 옵션
        QString option_menu17 = option_menus.at(17);
        ui->label_side16_name->setText(option_menu17);
        int option_price17 = option_prices.at(17);
        ui->label_side16_price->setText("+"+QString::number(option_price17)+"원");
        // 17번 옵션
        QString option_menu18 = option_menus.at(18);
        ui->label_side17_name->setText(option_menu18);
        int option_price18 = option_prices.at(18);
        ui->label_side17_price->setText("+"+QString::number(option_price18)+"원");
        // 가격 옵션
        QString option_menu19 = option_menus.at(19);
        ui->label_price1_name->setText(option_menu19);
        int option_price19 = option_prices.at(19);
        ui->label_price1_price->setText("+"+QString::number(option_price19)+"원");
        // 2번 옵션
        QString option_menu20 = option_menus.at(20);
        ui->label_price2_name->setText(option_menu20);
        int option_price20 = option_prices.at(20);
        ui->label_price2_price->setText("+"+QString::number(option_price20)+"원");
        // 30번 옵션
        QString option_menu21 = option_menus.at(21);
        ui->label_price3_name->setText(option_menu21);
        int option_price21 = option_prices.at(21);
        ui->label_price3_price->setText("+"+QString::number(option_price21)+"원");
        // 맛 1번 옵션
        QString option_menu22 = option_menus.at(22);
        ui->label_taste1_name->setText(option_menu22);
        int option_price22 = option_prices.at(22);
        ui->label_taste1_price->setText("+"+QString::number(option_price22)+"원");
        // 2번 옵션
        QString option_menu23 = option_menus.at(23);
        ui->label_taste2_name->setText(option_menu23);
        int option_price23 = option_prices.at(23);
        ui->label_taste2_price->setText("+"+QString::number(option_price23)+"원");
    }
    connClose();


}

Widget::~Widget()
{
    delete ui;
}

void Widget::menu_sql()
{
    connOpen();
    QSqlQuery insertQuery;
    QString sql = "UPDATE menu SET `check` = 1 WHERE menuNum = " + QString::number(flag);
    qDebug() << sql;
    insertQuery.prepare(sql);
    if (insertQuery.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery.lastError().text();
    }
    connClose();
    compareFlag();
}
// 메뉴 선택 하면 DB의 메뉴 테이블의 check에 1값이 들어감
void Widget::on_pushButton_1_clicked()
{
    flag = 1;
    menu_sql();
}
void Widget::on_pushButton_2_clicked()
{
    flag = 2;
    menu_sql();

}
void Widget::on_pushButton_3_clicked()
{
    flag = 3;
    menu_sql();
}
void Widget::on_pushButton_41_clicked()
{
    flag = 4;
    menu_sql();
}
void Widget::on_pushButton_51_clicked()
{
    flag = 5;
    menu_sql();
}
void Widget::on_pushButton_61_clicked()
{
    flag = 6;
    menu_sql();
}
void Widget::on_pushButton_7_clicked()
{
    flag = 7;
    menu_sql();
}
void Widget::on_pushButton_8_clicked()
{
    flag = 8;
    menu_sql();
}
void Widget::on_pushButton_9_clicked()
{
    flag = 9;
    menu_sql();
}
void Widget::on_pushButton_10_clicked()
{
    flag = 10;
    menu_sql();
}
void Widget::on_pushButton_11_clicked()
{
    flag = 11;
    menu_sql();
}
void Widget::on_pushButton_12_clicked()
{
    flag = 12;
    menu_sql();
}

void Widget::reset()
{
    // shopbaskets DB 초기화
    connOpen();
    QSqlQuery query;
    QString sql = "DELETE FROM shopbaskets WHERE menu_price < 50000";
    query.prepare(sql);
    if (query.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << query.lastError().text();
    }
    connClose();
    total_price = 0;
    total_payment = 0;
    optioncount = 0;
    // menu DB 'check' 값 초기화
    connOpen();
    for (int i=1;i<13;i++){
        QSqlQuery query1;
        QString sql1 = "UPDATE menu SET 'check'=0 WHERE menuNum = " + QString::number(i);
        query1.prepare(sql1);
        if (query1.exec()) {
            qDebug() << "Update successful";
        } else {
            qDebug() << "Update failed:" << query1.lastError().text();
        }
    }
    // options DB 'check' 값 초기화
    connOpen();
    for (int j=1;j<25;j++){
        QSqlQuery query2;
        QString sql2 = "UPDATE options SET 'check'=0 WHERE num = " + QString::number(j);
        query2.prepare(sql2);
        if (query2.exec()) {
            qDebug() << "Update successful";
        } else {
            qDebug() << "Update failed:" << query2.lastError().text();
        }
    }
    connClose();
    // 라디오버튼 초기화
    ui->radioButton_choice1->setChecked(true);
    ui->radioButton_price1->setChecked(true);
    ui->radioButton_taste1->setChecked(true);
    // 체크박스 체크 초기화
    ui->checkBox_side1->setChecked(false);
    ui->checkBox_side2->setChecked(false);
    ui->checkBox_side3->setChecked(false);
    ui->checkBox_side4->setChecked(false);
    ui->checkBox_side5->setChecked(false);
    ui->checkBox_side6->setChecked(false);
    ui->checkBox_side7->setChecked(false);
    ui->checkBox_side8->setChecked(false);
    ui->checkBox_side10->setChecked(false);
    ui->checkBox_side11->setChecked(false);
    ui->checkBox_side12->setChecked(false);
    ui->checkBox_side13->setChecked(false);
    ui->checkBox_side14->setChecked(false);
    ui->checkBox_side15->setChecked(false);
    ui->checkBox_side16->setChecked(false);
    ui->checkBox_side17->setChecked(false);
    // 수량 관련 텍스트 초기화
    ui->label_count->setText(QString::number(optioncount));
}
// 첫 페이지로 이동(모든 값 초기화)
void Widget::on_pushButton_before_clicked()
{
    reset();
    ui->stackedWidget->setCurrentIndex(5);

}

// 결제완료 페이지로 이동
void Widget::on_pushButton_payment_clicked()
{
    QString address, comment, phone;
    address = ui->lineEdit_address->text();
    comment = (ui->comboBox_comment->currentText()) + " " + (ui->lineEdit_comment->text());
    phone = ui->lineEdit_phone->text();
    qDebug() << "주소 : " << address << "요청사항" << comment;

    connOpen();
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO payment (address,comment,phone,payment,menu,total_option,total_price) VALUES (?,?,?,?,?,?,?)");
    insertQuery.addBindValue(address);
    insertQuery.addBindValue(comment);
    insertQuery.addBindValue(phone);
    insertQuery.addBindValue(way_to_pay);
    insertQuery.addBindValue(final_menu);
    insertQuery.addBindValue(final_option);
    insertQuery.addBindValue(total_payment);

    if (insertQuery.exec()) {
        QMessageBox::information(this, "결제 완료", way_to_pay+"로 결제가 완료되셨습니다!!");
    } else {
        QMessageBox::warning(this, "Error", "Record not added.");
    }
    connClose();
    ui->stackedWidget->setCurrentIndex(1);
}

// 옵션페이지에서 메인페이지로 이동
void Widget::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    // 선택했던 메뉴에 대한 check 값이 0으로 바뀜
    connOpen();
    QSqlQuery insertQuery;
    QString sql = "UPDATE menu SET `check` = 0 WHERE menuNum = " + QString::number(flag);
    qDebug() << sql;
    insertQuery.prepare(sql);
    if (insertQuery.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery.lastError().text();
    }
    connClose();
    // 개수 선택도 0으로 초기화
    optioncount = 0;
    ui->label_count->setText(QString::number(optioncount));
}

// 옵션창의 수량 더하고 빼기
void Widget::on_pushButton_plus_clicked()
{
    optioncount += 1;
    ui->label_count->setText(QString::number(optioncount));
}
void Widget::on_pushButton_minus_clicked()
{
    optioncount -= 1;
    if (optioncount<0){
        optioncount = 0;
    }
    ui->label_count->setText(QString::number(optioncount));
}


// 장바구니 창에서 누른 버튼의 이벤트 : 주문하기 창으로 이동
void Widget::on_pushButton_basket_clicked()
{
    qDebug() << "푸쉬 버튼이 클릭되었습니다.";
    ui->label_payment->setText(QString::number(total_price)+"원");
    total_payment = total_price + 2900;
    ui->label_totalpayment->setText(QString::number(total_payment)+"원");
    ui->stackedWidget->setCurrentIndex(2);
}
// 옵션창에서 장바구니 담기 버튼 눌렀을 때
void Widget::on_pushButton_menuSelect_clicked()
{
    if(optioncount!=0){
        ui->stackedWidget->setCurrentIndex(4);
    // 메뉴를 장바구니 DB에 넣기
    connOpen();
    QSqlQuery insertQuery;
    QString sql = "INSERT INTO shopbaskets (menu, menu_price) SELECT menu, price FROM menu WHERE `check` = 1";
    qDebug() << sql;
    insertQuery.prepare(sql);
    if (insertQuery.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery.lastError().text();
    }
    connClose();

    // 옵션을 장바구니 DB에 넣기
    connOpen();
    QSqlQuery insertQuery2;
    QString sql2 = "INSERT INTO shopbaskets (menu, menu_price) SELECT menu, extraprice FROM `options` WHERE `check` = 1";
    qDebug() << sql2;
    insertQuery2.prepare(sql2);
    if (insertQuery2.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery2.lastError().text();
    }
    connClose();


    // QListWidget 초기화
    ui->listWidget->clear();

    // "장바구니" 항목을 추가
    QListWidgetItem* headerItem = new QListWidgetItem("장바구니"); // "장바구니" 텍스트를 포함한 항목 생성
    // 폰트와 크기 설정
    QFont headerFont("Arial", 14); // 원하는 폰트와 크기로 설정
    headerItem->setFont(headerFont);
    // 폰트 크기를 기반으로 항목의 높이를 설정
    QFontMetrics fm(headerFont);
    int headerHeight = fm.height();
    headerItem->setSizeHint(QSize(100, headerHeight)); // 가로 크기는 100으로 설정하거나 필요에 따라 조절
    headerItem->setTextAlignment(Qt::AlignCenter); // 중앙 정렬
    headerItem->setFlags(headerItem->flags() & ~Qt::ItemIsSelectable); // 선택 불가능 설정
    ui->listWidget->addItem(headerItem); // "장바구니" 항목을 QListWidget에 추가

    // 장바구니에서 메뉴 불러오기
    connOpen();
    // 쿼리
    QSqlQuery query;
    if(!query.exec("SELECT menu, menu_price FROM shopbaskets"))
    {
        qDebug() << "Failed to execute query." << query.lastError().text();
        return;
    }
    QVector<QString> basket_menus;
    QVector<int> basket_prices;
    int num = 1;
    while (query.next()) {
        QString basket_menu = query.value(0).toString();
        int basket_price = query.value(1).toInt();
        basket_price = basket_price * optioncount;
        qDebug() << basket_menu << " : " << basket_price;
        basket_menus.append(basket_menu);
        basket_prices.append(basket_price);
        total_price += basket_price;
        if(num==1)
        {
            final_menu = final_menu+basket_menu;
        }
        else if (num==2){
            final_option = basket_menu;
        }
        else {
            final_option = final_option+", "+basket_menu;
        }
        num += 1;
    }
    qDebug() << "최종 메뉴 : " << final_menu;
    qDebug() << "최종 옵션 : " << final_option;
    int itemCount = qMin(basket_menus.size(), basket_prices.size());
    for (int i = 0; i < itemCount; ++i) {
        // 메뉴와 가격을 합쳐서 텍스트 생성
        QString itemText = basket_menus[i] + " - " + QString::number(basket_prices[i]);
        // 사용자 정의 위젯 생성
        QWidget* customWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(customWidget);
        QLabel* menuLabel = new QLabel(basket_menus[i]);
        QLabel* priceLabel = new QLabel(QString::number(basket_prices[i]));
        // 메뉴는 왼쪽 정렬, 가격은 오른쪽 정렬
        menuLabel->setAlignment(Qt::AlignLeft);
        priceLabel->setAlignment(Qt::AlignRight);
        layout->addWidget(menuLabel);
        layout->addWidget(priceLabel);
        // 사용자 정의 위젯을 QListWidget에 추가
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(customWidget->sizeHint());  // 위젯의 크기를 목록 항목의 크기로 설정
        ui->listWidget->setItemWidget(item, customWidget);
    }
    connClose();

    // QPushButton을 생성하고 텍스트를 설정
    QPushButton* pushButton_basket = new QPushButton("결제");
    pushButton_basket->setFixedSize(494, 50);
    // QListWidgetItem을 생성하고 QPushButton을 설정
    QListWidgetItem* item = new QListWidgetItem();
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, pushButton_basket);
    // 푸쉬버튼 핸들링 표시
    connect(pushButton_basket, &QPushButton::clicked, this, &Widget::on_pushButton_basket_clicked);

    }
    else{
        QMessageBox::warning(this, "에러", "1개 미만일 때는 주문할 수 없어요!!!!");
    }
}

// 옵션창에서 고기선택 라디오버튼을 눌렀을 때 이벤트
void Widget::on_radioButton_choice1_clicked()
{
    connOpen();
    QSqlQuery insertQuery1;
    QString sql1 = "UPDATE `options` SET `check` = 1 WHERE num = 1";
    qDebug() << sql1;
    insertQuery1.prepare(sql1);
    if (insertQuery1.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery1.lastError().text();
    }
    QSqlQuery insertQuery2;
    QString sql2 = "UPDATE `options` SET `check` = 0 WHERE num = 2";
    qDebug() << sql2;
    insertQuery2.prepare(sql2);
    if (insertQuery2.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery2.lastError().text();
    }
    connClose();
}
void Widget::on_radioButton_choice2_clicked()
{
    connOpen();
    QSqlQuery insertQuery1;
    QString sql1 = "UPDATE `options` SET `check` = 0 WHERE num = 1";
    qDebug() << sql1;
    insertQuery1.prepare(sql1);
    if (insertQuery1.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery1.lastError().text();
    }
    QSqlQuery insertQuery2;
    QString sql2 = "UPDATE `options` SET `check` = 1 WHERE num = 2";
    qDebug() << sql2;
    insertQuery2.prepare(sql2);
    if (insertQuery2.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery2.lastError().text();
    }
    connClose();
}

void Widget::option_sql(int arg1)
{
    if (arg1==Qt::Unchecked){
        connOpen();
        QSqlQuery insertQuery;
        QString sql = "UPDATE `options` SET `check` = 0 WHERE num = " + QString::number(flag2);
        qDebug() << sql;
        insertQuery.prepare(sql);
        if (insertQuery.exec()) {
            qDebug() << "Update successful";
        } else {
            qDebug() << "Update failed:" << insertQuery.lastError().text();
        }
        connClose();
    }
    else if (arg1==Qt::Checked){
        connOpen();
        QSqlQuery insertQuery;
        QString sql = "UPDATE `options` SET `check` = 1 WHERE num = " + QString::number(flag2);
        qDebug() << sql;
        insertQuery.prepare(sql);
        if (insertQuery.exec()) {
            qDebug() << "Update successful";
        } else {
            qDebug() << "Update failed:" << insertQuery.lastError().text();
        }
        connClose();
    }
}

void Widget::on_checkBox_side1_stateChanged(int arg1)
{
    flag2 = 3;
    option_sql(arg1);
}
void Widget::on_checkBox_side2_stateChanged(int arg1)
{
    flag2 = 4;
    option_sql(arg1);
}
void Widget::on_checkBox_side3_stateChanged(int arg1)
{
    flag2 = 5;
    option_sql(arg1);
}
void Widget::on_checkBox_side4_stateChanged(int arg1)
{
    flag2 = 6;
    option_sql(arg1);
}
void Widget::on_checkBox_side5_stateChanged(int arg1)
{
    flag2 = 7;
    option_sql(arg1);
}
void Widget::on_checkBox_side6_stateChanged(int arg1)
{
    flag2 = 8;
    option_sql(arg1);
}
void Widget::on_checkBox_side7_stateChanged(int arg1)
{
    flag2 = 9;
    option_sql(arg1);
}
void Widget::on_checkBox_side8_stateChanged(int arg1)
{
    flag2 = 10;
    option_sql(arg1);
}
void Widget::on_checkBox_side9_stateChanged(int arg1)
{
    flag2 = 11;
    option_sql(arg1);
}
void Widget::on_checkBox_side10_stateChanged(int arg1)
{
    flag2 = 12;
    option_sql(arg1);
}
void Widget::on_checkBox_side11_stateChanged(int arg1)
{
    flag2 = 13;
    option_sql(arg1);
}
void Widget::on_checkBox_side12_stateChanged(int arg1)
{
    flag2 = 14;
    option_sql(arg1);
}
void Widget::on_checkBox_side13_stateChanged(int arg1)
{
    flag2 = 15;
    option_sql(arg1);
}
void Widget::on_checkBox_side14_stateChanged(int arg1)
{
    flag2 = 16;
    option_sql(arg1);
}
void Widget::on_checkBox_side15_stateChanged(int arg1)
{
    flag2 = 17;
    option_sql(arg1);
}
void Widget::on_checkBox_side16_stateChanged(int arg1)
{
    flag2 = 18;
    option_sql(arg1);
}
void Widget::on_checkBox_side17_stateChanged(int arg1)
{
    flag2 = 19;
    option_sql(arg1);
}

void Widget::on_radioButton_price1_clicked()
{
    connOpen();
    QSqlQuery insertQuery1;
    QString sql1 = "UPDATE `options` SET `check` = 1 WHERE num = 20";
    qDebug() << sql1;
    insertQuery1.prepare(sql1);
    if (insertQuery1.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery1.lastError().text();
    }
    QSqlQuery insertQuery2;
    QString sql2 = "UPDATE `options` SET `check` = 0 WHERE num = 21";
    qDebug() << sql2;
    insertQuery2.prepare(sql2);
    if (insertQuery2.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery2.lastError().text();
    }
    QSqlQuery insertQuery3;
    QString sql3 = "UPDATE `options` SET `check` = 0 WHERE num = 22";
    qDebug() << sql3;
    insertQuery3.prepare(sql3);
    if (insertQuery3.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery3.lastError().text();
    }
    connClose();
}
void Widget::on_radioButton_price2_clicked()
{
    connOpen();
    QSqlQuery insertQuery1;
    QString sql1 = "UPDATE `options` SET `check` = 0 WHERE num = 20";
    qDebug() << sql1;
    insertQuery1.prepare(sql1);
    if (insertQuery1.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery1.lastError().text();
    }
    QSqlQuery insertQuery2;
    QString sql2 = "UPDATE `options` SET `check` = 1 WHERE num = 21";
    qDebug() << sql2;
    insertQuery2.prepare(sql2);
    if (insertQuery2.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery2.lastError().text();
    }
    QSqlQuery insertQuery3;
    QString sql3 = "UPDATE `options` SET `check` = 0 WHERE num = 22";
    qDebug() << sql3;
    insertQuery3.prepare(sql3);
    if (insertQuery3.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery3.lastError().text();
    }
    connClose();
}
void Widget::on_radioButton_price3_clicked()
{
    connOpen();
    QSqlQuery insertQuery1;
    QString sql1 = "UPDATE `options` SET `check` = 0 WHERE num = 20";
    qDebug() << sql1;
    insertQuery1.prepare(sql1);
    if (insertQuery1.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery1.lastError().text();
    }
    QSqlQuery insertQuery2;
    QString sql2 = "UPDATE `options` SET `check` = 0 WHERE num = 21";
    qDebug() << sql2;
    insertQuery2.prepare(sql2);
    if (insertQuery2.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery2.lastError().text();
    }
    QSqlQuery insertQuery3;
    QString sql3 = "UPDATE `options` SET `check` = 1 WHERE num = 22";
    qDebug() << sql3;
    insertQuery3.prepare(sql3);
    if (insertQuery3.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery3.lastError().text();
    }
    connClose();
}


void Widget::on_radioButton_taste1_clicked()
{
    connOpen();
    QSqlQuery insertQuery1;
    QString sql1 = "UPDATE `options` SET `check` = 1 WHERE num = 23";
    qDebug() << sql1;
    insertQuery1.prepare(sql1);
    if (insertQuery1.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery1.lastError().text();
    }
    QSqlQuery insertQuery2;
    QString sql2 = "UPDATE `options` SET `check` = 0 WHERE num = 24";
    qDebug() << sql2;
    insertQuery2.prepare(sql2);
    if (insertQuery2.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery2.lastError().text();
    }
    connClose();
}
void Widget::on_radioButton_taste2_clicked()
{
    connOpen();
    QSqlQuery insertQuery1;
    QString sql1 = "UPDATE `options` SET `check` = 1 WHERE num = 23";
    qDebug() << sql1;
    insertQuery1.prepare(sql1);
    if (insertQuery1.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery1.lastError().text();
    }
    QSqlQuery insertQuery2;
    QString sql2 = "UPDATE `options` SET `check` = 0 WHERE num = 24";
    qDebug() << sql2;
    insertQuery2.prepare(sql2);
    if (insertQuery2.exec()) {
        qDebug() << "Update successful";
    } else {
        qDebug() << "Update failed:" << insertQuery2.lastError().text();
    }
    connClose();
}


void Widget::on_pushButton_kakao_clicked()
{
    way_to_pay="카카오페이";
}
void Widget::on_pushButton_naver_clicked()
{
    way_to_pay="네이버페이";
}
void Widget::on_pushButton_payco_clicked()
{
    way_to_pay="페이코";
}
void Widget::on_pushButton_toss_clicked()
{
    way_to_pay="토스";
}
void Widget::on_pushButton_samsung_clicked()
{
    way_to_pay="삼성페이";
}
void Widget::on_pushButton_apple_clicked()
{
    way_to_pay="애플페이";
}
void Widget::on_pushButton_meet_clicked()
{
    way_to_pay="만나서 결제";
}

void Widget::on_pushButton_exit_clicked()
{
    this->close();
}

void Widget::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

