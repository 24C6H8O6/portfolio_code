#include "map.h"
#include "ui_map.h"
#include "book.h"
#include <QHostAddress>
#include <QtNetwork>
#include <QMouseEvent>
#include "widget.h"

map::map(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::map)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->gyungbubutton->setStyleSheet("QPushButton { border: none; border-bottom: 2px solid rgb(116, 122, 232); color: black; }");

    connect(ui->gyungbubutton, SIGNAL(clicked()), this, SLOT(changeScreen()));
    connect(ui->honambutton, SIGNAL(clicked()), this, SLOT(changeScreen()));
    connect(ui->gyungbubuttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(MapStartButton(QAbstractButton*)));
    connect(ui->honambuttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(MapStartButton(QAbstractButton*)));
    connect(ui->showmyinfobtn_2, SIGNAL(clicked()), this, SLOT(showmyinfo()));

    ui->okbutton->setEnabled(false);
    ui->okbutton->setText("확인");
    ui->okbutton->setStyleSheet("border: none;"
                                  "font-size : 13pt;"
                                  "color: gray;"
                                  "background-color: lightgray;");

    ui->okbutton_2->setEnabled(false);
    ui->okbutton_2->setText("확인");
    ui->okbutton_2->setStyleSheet("border: none;"
                                  "font-size : 13pt;"
                                  "color: gray;"
                                  "background-color: lightgray;");

    connect(ui->okbutton, SIGNAL(clicked()), this, SLOT(showbook()));
    connect(ui->okbutton_2, SIGNAL(clicked()), this, SLOT(showbook()));
    connect(ui->back, SIGNAL(clicked()), this, SLOT(backbook()));

    // 호남선(출발, 도착) 보이기
    for(int i=0; i<8; i++) {
        QPushButton *gyungbuarea = findChild <QPushButton *>(QString("gyungbu_%1").arg(i));
        if(gyungbuarea) {
            connect(gyungbuarea, SIGNAL(clicked()), this, SLOT(gyungbu_button_clicked()));
        }
    }

    // 호남선(출발, 도착) 보이기
    for(int i=0; i<7; i++) {
        QPushButton *honamarea = findChild <QPushButton *>(QString("honam_%1").arg(i));
        if(honamarea) {
            connect(honamarea, SIGNAL(clicked()), this, SLOT(honam_button_clicked()));
        }
    }

    //경부선(출발,도착) 숨기기
    for(int i=0; i<8; i++) {
        QFrame *gyungbuframe = findChild <QFrame *>(QString("gyungbuframe_%1").arg(i));
        if(gyungbuframe) {
            gyungbuframe->setVisible(false);
        }
    }

    //호남선(출발,도착) 숨기기
    for(int i=0; i<7; i++) {
        QFrame *honamframe = findChild <QFrame *>(QString("honamframe_%1").arg(i));
        if(honamframe) {
            honamframe->setVisible(false);
        }
    }

    tcpSocket = new QTcpSocket(this);
}

void map::changeScreen()
{
    QPushButton *changebutton = qobject_cast <QPushButton *> (sender());
    if(changebutton == ui->gyungbubutton) {

        ui->okbutton->setEnabled(false);
        ui->okbutton->setText("확인");
        ui->okbutton->setStyleSheet("border: none;"
                                    "font-size : 13pt;"
                                    "color: gray;"
                                    "background-color: lightgray;");

        lastClickedObjectNumber="50";
        start.clear();
        end.clear();

        ui->honambutton->setStyleSheet("QPushButton { border: none; color: gray; }");
        ui->gyungbubutton->setStyleSheet("QPushButton { border: none; border-bottom: 2px solid rgb(116, 122, 232); color: black; }");

        for(QAbstractButton *button : ui->honambuttonGroup->buttons()) {
            QPushButton *honamButtons = qobject_cast<QPushButton *>(button);
            QFrame *parentFrame = qobject_cast<QFrame *>(honamButtons->parent());
            if(parentFrame) {
                if(honamButtons->objectName().startsWith("start")){
                    honamButtons->setText("출발");
                    honamButtons->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                } else {
                    honamButtons->setFixedSize(30, 25);
                    honamButtons->setText("도착");
                    honamButtons->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                }
                parentFrame->setVisible(false);
            }
        }

        ui->stackedWidget->setCurrentIndex(0);
    } else if(changebutton == ui->honambutton) {

        ui->okbutton_2->setEnabled(false);
        ui->okbutton_2->setText("확인");
        ui->okbutton_2->setStyleSheet("border: none;"
                                      "font-size : 13pt;"
                                      "color: gray;"
                                      "background-color: lightgray;");

        lastClickedObjectNumber="50";
        start.clear();
        end.clear();

        ui->gyungbubutton->setStyleSheet("QPushButton { border: none; color: gray; }");
        ui->honambutton->setStyleSheet("QPushButton { border: none; border-bottom: 2px solid rgb(116, 122, 232); color: black; }");

        for(QAbstractButton *button : ui->gyungbubuttonGroup->buttons()) {
            QPushButton *gyungbuButtons = qobject_cast<QPushButton *>(button);
            QFrame *parentFrame = qobject_cast<QFrame *>(gyungbuButtons->parent());
            if(parentFrame) {
                if(gyungbuButtons->objectName().startsWith("start")){
                    gyungbuButtons->setText("출발");
                    gyungbuButtons->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                } else {
                    gyungbuButtons->setFixedSize(30, 25);
                    gyungbuButtons->setText("도착");
                    gyungbuButtons->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                }
                parentFrame->setVisible(false);
            }
        }

        ui->stackedWidget->setCurrentIndex(1);
    }
}

void map::gyungbu_button_clicked()
{
    // 클릭된 푸시 버튼 저장하기
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());

    if (senderButton)
    {
        QString objectName = senderButton->objectName();
        QString indexString = objectName.remove("gyungbu_");

        // 추출한 문자열을 정수로 변환
        int index = indexString.toInt();

        // 출발,도착 프레임
        QString frameObjectName = QString("gyungbuframe_%1").arg(index);

        // 해당 이름의 프레임 찾아서 저장하기
        QFrame *frame = findChild<QFrame *>(frameObjectName);

        // isVisble -> 보여지고 있으면 true, setVisble(bool) -> true면 보여지게 만들기
        if(frame->isVisible()) {
            QList<QPushButton *> buttons = frame->findChildren<QPushButton *>();
            for(QPushButton *button : buttons) {
                if(button->objectName().startsWith("start")){
                    button->setText("출발");
                    button->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                } else {
                    button->setFixedSize(30, 25);
                    button->setText("도착");
                    button->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                }
            }
            lastClickedObjectNumber="50";
            frame->setVisible(false);
        } else{
            for(QAbstractButton *button : ui->gyungbubuttonGroup->buttons()) {
                QPushButton *otherButtons = qobject_cast<QPushButton *>(button);
                if(otherButtons->styleSheet().contains("color: rgb(255, 255, 255);")) {
                    QFrame *parentFrame = qobject_cast<QFrame *>(otherButtons->parent());
                    if(parentFrame) {
                        parentFrame->setVisible(false);
                    }
                }
            }
            frame->setVisible(true);
        }
    }
}


void map::honam_button_clicked()
{
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());

    if (senderButton)
    {
        QString objectName = senderButton->objectName();

        QString indexString = objectName.remove("honam_");

        // 추출한 문자열을 정수로 변환
        int index = indexString.toInt();

        QString frameObjectName = QString("honamframe_%1").arg(index);
        QFrame *frame = findChild<QFrame *>(frameObjectName);

        if(frame->isVisible()) {
            QList<QPushButton *> buttons = frame->findChildren<QPushButton *>();
            for(QPushButton *button : buttons) {
                if(button->objectName().startsWith("start")){
                    button->setText("출발");
                    button->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                } else {
                    button->setFixedSize(30, 25);
                    button->setText("도착");
                    button->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                }
            }
            lastClickedObjectNumber="50";
            frame->setVisible(false);
        } else{
            for(QAbstractButton *button : ui->honambuttonGroup->buttons()) {
                QPushButton *otherButtons = qobject_cast<QPushButton *>(button);
                if(otherButtons->styleSheet().contains("color: rgb(255, 255, 255);")) {
                    QFrame *parentFrame = qobject_cast<QFrame *>(otherButtons->parent());
                    if(parentFrame) {
                        parentFrame->setVisible(false);
                    }
                }
            }
            frame->setVisible(true);
        }
    }
}

void map::MapStartButton(QAbstractButton *button)
{
    start.clear();
    end.clear();
    QString currentClickedObjectNumber = button->objectName().right(1);
    if (currentClickedObjectNumber == lastClickedObjectNumber) {
        if(button->objectName().contains("gyungbu")) {
            QFrame *frame = findChild<QFrame *>("gyungbuframe_"+lastClickedObjectNumber);
            if(frame) {
                frame->setVisible(false);
            }
            QPushButton *startButton = findChild<QPushButton *>("startgyungbu_"+lastClickedObjectNumber);
            if(startButton) {
                startButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                startButton->setText("출발");
            }
            QPushButton *endButton = findChild<QPushButton *>("endgyungbu_"+lastClickedObjectNumber);
            if(endButton){
                endButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                endButton->setText("도착");
                endButton->setFixedSize(30, 25);
            }
            currentClickedObjectNumber="50";
        } else if(button->objectName().contains("honam")) {
            QFrame *frame = findChild<QFrame *>("honamframe_"+lastClickedObjectNumber);
            if(frame) {
                frame->setVisible(false);
            }
            QPushButton *startButton = findChild<QPushButton *>("starthonam_"+lastClickedObjectNumber);
            if(startButton) {
                startButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                startButton->setText("출발");
            }
            QPushButton *endButton = findChild<QPushButton *>("endhonam_"+lastClickedObjectNumber);
            if(endButton){
                endButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                endButton->setText("도착");
                endButton->setFixedSize(30, 25);
            }
        }
    } else{
        if(button->objectName().startsWith("startgyungbu")) {
            if(button->styleSheet().contains("color: blue;")) {
                QPushButton *startButton = findChild<QPushButton *>("startgyungbu_"+currentClickedObjectNumber);
                if(startButton) {
                    startButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                    startButton->setText("출발");
                }
                QPushButton *endButton = findChild<QPushButton *>("endgyungbu_"+currentClickedObjectNumber);
                if(endButton){
                    endButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                    endButton->setText("도착");
                    endButton->setFixedSize(30, 25);
                }
                QFrame *frame = findChild<QFrame *>("gyungbuframe_"+currentClickedObjectNumber);
                if(frame) {
                    frame->setVisible(false);
                }
                currentClickedObjectNumber="50";
            } else {
                for(QAbstractButton *otherbutton : ui->gyungbubuttonGroup->buttons()) {
                    QPushButton *otherButtons = qobject_cast<QPushButton *>(otherbutton);
                    if(otherButtons->isVisible() && otherButtons->text() == "출발" && otherbutton->objectName().right(1) !=currentClickedObjectNumber) {
                        QFrame *parentFrame = qobject_cast<QFrame *>(otherButtons->parent());
                        if(parentFrame) {
                            QString otherObjectNumber = parentFrame->objectName().right(1);
                            otherButtons->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                            QPushButton *otherendButton = findChild<QPushButton *>("endgyungbu_"+otherObjectNumber);
                            otherendButton->setText("도착");
                            otherendButton->setFixedSize(30, 25);
                            otherendButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                            parentFrame->setVisible(false);
                        }
                    }
                }
                QPushButton *startButton = findChild<QPushButton *>("startgyungbu_"+currentClickedObjectNumber);
                if(startButton) {
                    startButton->setStyleSheet("color: blue; background-color: white;");
                    startButton->setText("출발");
                }
                QPushButton *endButton = findChild<QPushButton *>("endgyungbu_"+currentClickedObjectNumber);
                if(endButton){
                    endButton->setText("X");
                    endButton->setStyleSheet("color: rgb(254, 254, 254); background-color: rgb(85, 87, 83);");
                    endButton->setFixedSize(10, 10);
                }
            }
        } else if(button->objectName().startsWith("endgyungbu")) {
            for(QAbstractButton *otherbutton : ui->gyungbubuttonGroup->buttons()) {
                QPushButton *otherButtons = qobject_cast<QPushButton *>(otherbutton);
                if(button->text() != "X") {
                    if(otherButtons->isVisible() && otherButtons->text() == "도착" && otherbutton->objectName().right(1) !=currentClickedObjectNumber) {
                        QFrame *parentFrame = qobject_cast<QFrame *>(otherButtons->parent());
                        if(parentFrame) {
                            QString otherObjectNumber = parentFrame->objectName().right(1);
                            otherButtons->setText("출발");
                            otherButtons->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                            QPushButton *otherendButton = findChild<QPushButton *>("endgyungbu_"+otherObjectNumber);
                            otherendButton->setText("도착");
                            otherendButton->setFixedSize(30, 25);
                            otherendButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                            parentFrame->setVisible(false);
                        }
                    }
                }
            }
            QPushButton *startButton = findChild<QPushButton *>("startgyungbu_"+currentClickedObjectNumber);
            if(startButton) {
                startButton->setStyleSheet("color: blue; background-color: white;");
                startButton->setText("도착");
            }
            QPushButton *endButton = findChild<QPushButton *>("endgyungbu_"+currentClickedObjectNumber);
            if(endButton){
                if(endButton->text() == "X") {
                    startButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                    startButton->setText("출발");
                    endButton->setText("도착");
                    endButton->setFixedSize(30, 25);
                    endButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                    QFrame *buttonframe = findChild<QFrame *>("gyungbuframe_"+currentClickedObjectNumber);
                    currentClickedObjectNumber="50";
                    buttonframe->setVisible(false);
                } else {
                    endButton->setText("X");
                    endButton->setStyleSheet("color: rgb(254, 254, 254); background-color: rgb(85, 87, 83);");
                    endButton->setFixedSize(10, 10);
                }
            }
        } else if(button->objectName().contains("starthonam")) {
            if(button->styleSheet().contains("color: blue;")) {
                QPushButton *startButton = findChild<QPushButton *>("starthonam_"+currentClickedObjectNumber);
                if(startButton) {
                    startButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                    startButton->setText("출발");
                }
                QPushButton *endButton = findChild<QPushButton *>("endhonam_"+currentClickedObjectNumber);
                if(endButton){
                    endButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                    endButton->setText("도착");
                    endButton->setFixedSize(30, 25);
                }
                QFrame *frame = findChild<QFrame *>("honamframe_"+currentClickedObjectNumber);
                if(frame) {
                    frame->setVisible(false);
                }
                currentClickedObjectNumber="50";
            } else {
                for(QAbstractButton *otherbutton : ui->honambuttonGroup->buttons()) {
                    QPushButton *otherButtons = qobject_cast<QPushButton *>(otherbutton);
                    if(otherButtons->isVisible() && otherButtons->text() == "출발" && otherbutton->objectName().right(1) !=currentClickedObjectNumber) {
                        QFrame *parentFrame = qobject_cast<QFrame *>(otherButtons->parent());
                        if(parentFrame) {
                            QString otherObjectNumber = parentFrame->objectName().right(1);
                            otherButtons->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                            QPushButton *otherendButton = findChild<QPushButton *>("endhonam_"+otherObjectNumber);
                            otherendButton->setText("도착");
                            otherendButton->setFixedSize(30, 25);
                            otherendButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                            parentFrame->setVisible(false);
                        }
                    }
                }
                QPushButton *startButton = findChild<QPushButton *>("starthonam_"+currentClickedObjectNumber);
                if(startButton) {
                    startButton->setStyleSheet("color: blue; background-color: white;");
                    startButton->setText("출발");
                }
                QPushButton *endButton = findChild<QPushButton *>("endhonam_"+currentClickedObjectNumber);
                if(endButton){
                    endButton->setText("X");
                    endButton->setStyleSheet("color: rgb(254, 254, 254); background-color: rgb(85, 87, 83);");
                    endButton->setFixedSize(10, 10);
                }
            }
        } else if(button->objectName().startsWith("endhonam")) {
            for(QAbstractButton *otherbutton : ui->honambuttonGroup->buttons()) {
                QPushButton *otherButtons = qobject_cast<QPushButton *>(otherbutton);
                if(button->text() != "X") {
                    if(otherButtons->isVisible() && otherButtons->text() == "도착" && otherbutton->objectName().right(1) !=currentClickedObjectNumber) {
                        QFrame *parentFrame = qobject_cast<QFrame *>(otherButtons->parent());
                        if(parentFrame) {
                            QString otherObjectNumber = parentFrame->objectName().right(1);
                            otherButtons->setText("출발");
                            otherButtons->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                            QPushButton *otherendButton = findChild<QPushButton *>("endhonam_"+otherObjectNumber);
                            otherendButton->setText("도착");
                            otherendButton->setFixedSize(30, 25);
                            otherendButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                            parentFrame->setVisible(false);
                        }
                    }
                }
            }
            QPushButton *startButton = findChild<QPushButton *>("starthonam_"+currentClickedObjectNumber);
            if(startButton) {
                startButton->setStyleSheet("color: blue; background-color: white;");
                startButton->setText("도착");
            }
            QPushButton *endButton = findChild<QPushButton *>("endhonam_"+currentClickedObjectNumber);
            if(endButton){
                if(endButton->text() == "X") {
                    startButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                    startButton->setText("출발");
                    endButton->setText("도착");
                    endButton->setFixedSize(30, 25);
                    endButton->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(85, 87, 83);");
                    QFrame *buttonframe = findChild<QFrame *>("honamframe_"+currentClickedObjectNumber);
                    buttonframe->setVisible(false);
                    currentClickedObjectNumber="50";
                } else {
                    endButton->setText("X");
                    endButton->setStyleSheet("color: rgb(254, 254, 254); background-color: rgb(85, 87, 83);");
                    endButton->setFixedSize(10, 10);
                }
            }
        }

    }

    bool gyungbustart=false;
    bool gyungbuend=false;

    bool honamstart=false;
    bool honamend=false;

    gobookchk_1=false;
    gobookchk_2=false;

    for(QAbstractButton *gyungbubutton : ui->gyungbubuttonGroup->buttons()) {
        if(gyungbubutton->text() == "출발" && gyungbubutton->styleSheet().contains("color: blue;")) {
            start = gyungbubutton->objectName().right(1);
            gyungbustart = true;
        }
        if(gyungbubutton->text() == "도착" && gyungbubutton->styleSheet().contains("color: blue;")) {
            end = gyungbubutton->objectName().right(1);
            gyungbuend = true;
        }
    }
    if(gyungbustart && gyungbuend) {
        gobookchk_1=true;
    }

    for(QAbstractButton *honambutton : ui->honambuttonGroup->buttons()) {
        if(honambutton->text() == "출발" && honambutton->styleSheet().contains("color: blue;")) {
            start = honambutton->objectName().right(1);
            honamstart = true;
        }
        if(honambutton->text() == "도착" && honambutton->styleSheet().contains("color: blue;")) {
            end = honambutton->objectName().right(1);
            honamend = true;
        }
    }
    if(honamstart && honamend) {
        gobookchk_2=true;
    }

    if(gobookchk_1) {
        ui->okbutton->setEnabled(true);
        ui->okbutton->setText("확인");
        ui->okbutton->setStyleSheet("font-size : 13pt;"
                                    "color: rgb(85, 87, 83);"
                                    "background-color: rgb(204, 255, 244);"
                                    "border: none;");
    } else {
        ui->okbutton->setEnabled(false);
        ui->okbutton->setText("확인");
        ui->okbutton->setStyleSheet("border: none;"
                                    "font-size : 13pt;"
                                    "color: gray;"
                                    "background-color: lightgray;");
    }

    if(gobookchk_2) {
        ui->okbutton_2->setEnabled(true);
        ui->okbutton_2->setText("확인");
        ui->okbutton_2->setStyleSheet("border: none;"
                                      "font-size : 13pt;"
                                      "color: rgb(85, 87, 83);"
                                      "background-color: rgb(204, 255, 244);");
    } else {
        ui->okbutton_2->setEnabled(false);
        ui->okbutton_2->setText("확인");
        ui->okbutton_2->setStyleSheet("border: none;"
                                      "font-size : 13pt;"
                                      "color: gray;"
                                      "background-color: lightgray;");
    }

    // 마지막으로 클릭한 버튼의 objectName을 저장   

    lastClickedObjectNumber = currentClickedObjectNumber;
}

void map::getuserinfo(QString id, QString password, QString name, QString phonenum, QString email)
{
    ID = id;
    PW = password;
    NAME = name;
    HP = phonenum;
    EM = email;
}

void map::showmyinfo()
{
    if(!widgetContainer_8->isVisible()) {
        QFrame *frame = new QFrame(widgetContainer_8);
        widgetLayout_8->setContentsMargins(0, 0, 0, 0);
        frame -> setFixedSize(330, 700);
        frame->setStyleSheet("QFrame {"
                             "background-color: white;"
                             "border: none"
                             "}");

        QFrame *topframe = new QFrame(frame);
        QHBoxLayout *hbl_1 = new QHBoxLayout;
        QLabel *usernamelabel = new QLabel(NAME);
        QLabel *gogaknim = new QLabel("고객님!");
        QPushButton *logoutbutton = new QPushButton;
        QIcon buttonIcon(":/resources/logout.png");

        QSpacerItem *spacer_1 = new QSpacerItem(150, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        hbl_1->setContentsMargins(0, 0, 0, 0);

        topframe->setFixedSize(330, 50);
        usernamelabel->setFixedSize(50, 50);
        usernamelabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        gogaknim->setFixedSize(50, 50);
        gogaknim->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        logoutbutton->setFixedSize(50, 50);
        logoutbutton->setIcon(buttonIcon);
        logoutbutton->setIconSize(logoutbutton->size());

        topframe->setStyleSheet("background-color: rgb(255, 200, 200); border:none;");
        usernamelabel->setStyleSheet("color: blue; text-decoration: underline; font-size:10pt; background:transparent;");
        gogaknim->setStyleSheet("color:gray; font-size:10pt; background:transparent;");
        logoutbutton->setStyleSheet("border: none;");



        hbl_1->addWidget(usernamelabel);
        hbl_1->addWidget(gogaknim);
        hbl_1->addItem(spacer_1);
        hbl_1->addWidget(logoutbutton);

        widgetLayout_8->addLayout(hbl_1);

        QLabel *myinfo = new QLabel("마이페이지");
        QLabel *showinfo = new QLabel("회원정보");
        QLabel *showID = new QLabel("아이디        \t" + ID);
        QLabel *showPW = new QLabel("비밀번호   \t" + PW);
        QLabel *showHP = new QLabel("핸드폰 번호\t" + HP);
        QLabel *showEM = new QLabel("이메일        \t" + EM);

        myinfo->setStyleSheet("border:none; background: transparent; color: blue; font-size: 14pt");
        myinfo->setFixedSize(330, 50);
        showinfo->setStyleSheet("border:none; background: transparent; color: black; font-size: 12pt; border-top: 1px solid lightgray;");
        showinfo->setFixedSize(300, 50);
        showinfo->setAlignment(Qt::AlignCenter);
        showID->setStyleSheet("border:none; background: transparent; color: black; font-size: 10pt; border-top: 1px solid lightgray;");
        showID->setFixedSize(300, 40);
        showPW->setStyleSheet("border:none; background: transparent; color: black; font-size: 10pt; border-top: 1px solid lightgray;");
        showPW->setFixedSize(300, 40);
        showHP->setStyleSheet("border:none; background: transparent; color: black; font-size: 10pt; border-top: 1px solid lightgray;");
        showHP->setFixedSize(300, 40);
        showEM->setStyleSheet("border:none; background: transparent; color: black; font-size: 10pt; border-top: 1px solid lightgray;");
        showEM->setFixedSize(300, 40);

        widgetLayout_8->addWidget(myinfo);
        widgetLayout_8->addWidget(showinfo);
        widgetLayout_8->addWidget(showID);
        widgetLayout_8->addWidget(showPW);
        widgetLayout_8->addWidget(showHP);
        widgetLayout_8->addWidget(showEM);

        QSpacerItem *spacer_2 = new QSpacerItem(0, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);

        widgetLayout_8->addItem(spacer_2);

        QPushButton *showticket = new QPushButton("승차권 확인");
        showticket->setStyleSheet("border:none; background: transparent; color: black; font-size: 12pt; border-top: 1px solid lightgray;");
        showticket->setFixedSize(300, 50);

        widgetLayout_8->addWidget(showticket);
        QHBoxLayout *centerlayout = new QHBoxLayout;
        QLabel *trainimg = new QLabel;
        QPixmap train(":/resources/trainimg.png");

        trainimg->setStyleSheet("background: transparent;");
        trainimg->setFixedSize(200, 200);
        trainimg->setPixmap(train);
        trainimg->setScaledContents(true);

        centerlayout->addWidget(trainimg);
        centerlayout->setAlignment(Qt::AlignCenter);

        widgetLayout_8->addLayout(centerlayout);

        QLabel *welcome = new QLabel("Welcome to Bok Rail!!!");
        welcome->setStyleSheet("border:none; background: transparent; color: blue; font-size: 14pt; font: bold");
        welcome->setFixedSize(300, 50);
        welcome->setAlignment(Qt::AlignCenter);

        widgetLayout_8->addWidget(welcome);
        widgetLayout_8->addStretch();
        widgetContainer_8->show();

        connect(logoutbutton, &QPushButton::clicked, this, &map::showlogin);

        animation = new QPropertyAnimation(widgetContainer_8, "geometry");
        animation->setDuration(300);


        animation->setStartValue(QRect(360, 0, 330, 700));
        animation->setEndValue(QRect(30, 0, 330, 700));
        animation->start();
    }
}

bool map::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        QRect containerRect = widgetContainer_8->geometry();
        QPoint mousePosInContainer = widgetContainer_8->mapFromGlobal(mouseEvent->globalPos());
        if (!containerRect.contains(mousePosInContainer)) {
            if (widgetContainer_8->isVisible()) {
                animation = new QPropertyAnimation(widgetContainer_8, "geometry");
                animation->setDuration(300);

                animation->setStartValue(QRect(30, 0, 330, 700));
                animation->setEndValue(QRect(360, 0, 330, 700));
                connect(animation, &QPropertyAnimation::finished, widgetContainer_8, &QWidget::hide);
                animation->start();

                QLayoutItem* item8;
                while ((item8 = widgetLayout_8->takeAt(0))) {
                    if (QWidget* widget8 = item8->widget()) {
                        widget8->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                    }
                    delete item8;
                }
            }
        }
    }
    // 이벤트를 계속 처리하도록 반환
    return QWidget::eventFilter(obj, event);
}

void map::showlogin()
{
    connect(this, &map::logout, parentWidget, &Widget::backlogin);
    QFrame *mapFrame = findChild<QFrame *>("map");
    if (mapFrame) {
        mapFrame->hide();
    }
    emit logout();
    QString message = "logout: ";
    tcpSocket->write(message.toUtf8());
    disconnect(this, &map::logout, parentWidget, &Widget::backlogin);
}

void map::getsignal()
{
    if (!chkconnect && !tcpSocket->isOpen()) {
        // 순서 5.1 QLineEdit 에 입력된 Server IP, Server Port를 가져온다.
        QString serverIP = getMyIP().toString();
        QString serverPort = "25000";

        // 순서 5.2 입력된 IP를 프로토콜에 맞는 형태로 가공한다.
        QHostAddress serverAddress(serverIP);

        // 순서 5.3 서버 IP로 연결 요청을 보낸다.
        tcpSocket->connectToHost(serverAddress, serverPort.toUShort());
        chkconnect = true;

        // 연결 실패시 에러 메시지
        if (tcpSocket->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Failed to connect: " << tcpSocket->errorString();
            // 여기에 사용자에게 연결 실패 메시지를 표시하는 로직 추가
        }
    }
    this->show();
}

void map::getTcpSocket(QTcpSocket *tcpsocket)
{
    tcpSocket = tcpsocket;
}

QHostAddress map::getMyIP()
{
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // localhost(127.0.0.1) 가 아닌 것을 사용
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            myAddress = ipAddressesList.at(i);
            break;
        }
    }

    // 인터넷이 연결되어 있지 않다면, localhost(127.0.0.1) 사용
    if (myAddress.toString().isEmpty())
        myAddress = QHostAddress(QHostAddress::LocalHost);

    return myAddress;
}

void map::showbook()
{
    QString dep;
    QString arv;
    QPushButton *checkstation = qobject_cast<QPushButton *>(sender());
    if(checkstation == ui->okbutton) {
        dep = gyungbustations[start.toInt()];
        arv = gyungbustations[end.toInt()];
    } else if(checkstation == ui->okbutton_2) {
        dep = honamstations[start.toInt()];
        arv = honamstations[end.toInt()];
    }

    connect(this, &map::sendInfo, book, &Book::getstations);
    emit sendInfo(dep, arv, bookstackedidx);
    disconnect(this, &map::sendInfo, book, &Book::getstations);
}

void map::backbook()
{
    connect(this, SIGNAL(gobook()), book, SLOT(show()));
    this->hide();
    emit gobook();
}

void map::getway(int idx)
{
    bookstackedidx = idx;
}

map::~map()
{
    delete ui;
}

