#include "widget.h"
#include "ui_widget.h"
#include "ui_login.h"
#include "ui_join.h"
#include "ui_find.h"
#include "ui_trainlist.h"
#include "ui_traintime.h"
#include "ui_trainfeepage.h"
#include "screen.h"
#include <QMovie>
#include <QTimer>
#include <QHostAddress>
#include <QtNetwork>
#include <QMessageBox>
#include "login.h"
#include "join.h"
#include "find.h"
#include "book.h"
#include "trainlist.h"
#include "traintime.h"
#include "trainfeepage.h"
#include "Seat.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this);

    connectserver();

    movie = new QMovie(":/resources/Spinner-1s-200px.gif");
    ui->label_2->setMovie(movie);
    movie->start();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::showlogin);
    timer->start(2000);

    connect(tcpSocket, SIGNAL(readyRead()),
            this,      SLOT(readMessage()));
}

QHostAddress Widget::getMyIP()
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

void Widget::connectserver()
{
    QString serverIP = "10.10.21.130";
    QString serverPort = "8080";

    // 순서 5.2 입력된 IP를 프로토콜에 맞는 형태로 가공한다.
    QHostAddress serverAddress(serverIP);

    // 순서 5.3 서버 IP로 연결 요청을 보낸다.
    // [2] 연결 : 서버 IP로 연결 요청, Port 는 qint16 자료형
    tcpSocket->connectToHost(serverAddress, serverPort.toUShort());
}

void Widget::readMessage()
{
    // ex6.1 바이트를 읽어들여
    if(tcpSocket->bytesAvailable() >= 0) {
        QByteArray readData = tcpSocket->readAll();
        QString message = QString::fromUtf8(readData);
        qDebug() << message;
        if(message.startsWith("로그인 성공")) {
            message = message.remove("로그인 성공: ");
            QStringList userinfo = message.split(",");
            ID = userinfo.at(0);
            PW = userinfo.at(1);
            NAME = userinfo.at(2);
            QTimer timer;
            QEventLoop loop;
            connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
            timer.start(1000);
            loop.exec();
            showbook();
        } else if(message.startsWith("이미 접속 중인 ID입니다: ")) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "이용안내",
                "다른기기에서 사용중 입니다.\n로그아웃 하시겠습니까?",
                QMessageBox::Yes | QMessageBox::No
                );
            QString id;
            id = message.remove("이미 접속 중인 ID입니다: ");
            id = "로그아웃 요청: " + id;
            if (reply == QMessageBox::Yes) {
                tcpSocket->write(id.toUtf8());
            }
        } else if(message.startsWith("noinput: ")) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, "이용안내", "아이디나 비밀번호를<br>입력하지 않았습니다.", QMessageBox::Ok);
            if (reply == QMessageBox::Ok) {

            }
        } else if(message.startsWith("wronginput: ")) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, "이용안내", "아이디나 비밀번호를<br>잘못 입력하였습니다.", QMessageBox::Ok);
            if (reply == QMessageBox::Ok) {

            }
        } else if(message.startsWith("존재")) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, "이용안내", message, QMessageBox::Ok);
            if (reply == QMessageBox::Ok) {

            }
        } else if(message.startsWith("ID: ")) {

            message = message.remove("ID: ");
            find->ui->label_28->setText("고객님의 아이디는\n["+ message +"]입니다.");
            find->ui->stackedWidget->setCurrentIndex(4);

        } else if(message.startsWith("PW: ")){

            message = message.remove("PW: ");
            find->ui->label_32->setText("고객님의 비밀번호는\n["+ message +"]입니다.");
            find->ui->stackedWidget->setCurrentIndex(5);

        } else if(message.startsWith("useID: ")) {

            message = message.remove("useID: ");
            join->ui->id_chk->setText(message);

        } else if(message.startsWith("useHP: ")) {

            message = message.remove("useHP: ");
            join->ui->hp_chk->setText(message);

        } else if(message.startsWith("useEM: ")) {

            message = message.remove("useEM: ");
            join->ui->em_chk->setText(message);

        } else if(message.startsWith("회원가입 완료")) {
            join->ui->textEdit->setText(message);
            QTimer timer;
            QEventLoop loop;
            connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
            timer.start(2000);
            loop.exec();
            showlogin();
        } else if(message.startsWith("열차조회성공: ")) {
            message = message.remove("열차조회성공: ");
            QVector<QString> messages = message.split(",");
            if(messages[1] == "0"){
                traininfo.trainNum = messages[0];
                traininfo.tripway = messages[1];
                traininfo.station1 = messages[2];
                traininfo.station2 = messages[3];
                traininfo.deptime_year = messages[4];
                traininfo.deptime_month = messages[5];
                traininfo.deptime_day = messages[6];
                traininfo.deptime_hour = messages[7].toInt();
                traininfo.deptime_min = messages[8].toInt();
                traininfo.yoil = messages[9];
                traininfo.dur_time = messages[10];
                traininfo.price = messages[11].toInt();
                traininfo.adult_seat = messages[12].toInt();
                traininfo.child_seat = messages[13].toInt();
                traininfo.elder_seat = messages[14].toInt();
                trainlist->getcustomNum(traininfo.adult_seat+traininfo.child_seat+traininfo.elder_seat);
                train_list();
            }else if (messages[1] == "1"){
                // 출발 정보
                traininfo_arr.trainNum = messages[0];
                traininfo_arr.tripway = messages[1];
                traininfo_arr.station1 = messages[2];
                traininfo_arr.station2 = messages[3];
                traininfo_arr.deptime_year = messages[4];
                traininfo_arr.deptime_month = messages[5];
                traininfo_arr.deptime_day = messages[6];
                traininfo_arr.deptime_hour = messages[7].toInt();
                traininfo_arr.deptime_min = messages[8].toInt();
                traininfo_arr.yoil = messages[9];
                traininfo_arr.dur_time = messages[16];
                traininfo_arr.price = messages[17].toInt();
                traininfo_arr.adult_seat = messages[18].toInt();
                traininfo_arr.child_seat = messages[19].toInt();
                traininfo_arr.elder_seat = messages[20].toInt();
                traininfo = traininfo_arr;
                // 도착 정보
                traininfo_dep.trainNum = messages[0];
                traininfo_dep.tripway = messages[1];
                traininfo_dep.station1 = messages[3];
                traininfo_dep.station2 = messages[2];
                traininfo_dep.deptime_year = messages[10];
                traininfo_dep.deptime_month = messages[11];
                traininfo_dep.deptime_day = messages[12];
                traininfo_dep.deptime_hour = messages[13].toInt();
                traininfo_dep.deptime_min = messages[14].toInt();
                traininfo_dep.yoil = messages[15];
                traininfo_dep.dur_time = messages[16];
                traininfo_dep.price = messages[17].toInt();
                traininfo_dep.adult_seat = messages[18].toInt();
                traininfo_dep.child_seat = messages[19].toInt();
                traininfo_dep.elder_seat = messages[20].toInt();
                trainlist->getcustomNum(traininfo_dep.adult_seat+traininfo_dep.child_seat+traininfo_dep.elder_seat);
                train_list();
            }

        }
    }
}


void Widget::showlogin()
{
    timer->stop();
    movie->stop();

    login = new Login(this);
    login->setWidgetInstance(this);
    connect(this, &Widget::gologin, login, &Login::getsignal);

    QFrame *widgetFrame = findChild<QFrame *>("Widget");
    if (widgetFrame) {
        widgetFrame->hide();
    }
    login -> getTcpSocket(tcpSocket);
    emit gologin(ID, PW, departure, arrival);
    disconnect(this, &Widget::gologin, login, &Login::getsignal);
}

void Widget::backlogin()
{
    login = new Login(this);
    login->setWidgetInstance(this);
    connect(this, &Widget::logout, login, &Login::show);
    login -> getTcpSocket(tcpSocket);
    emit logout();
    disconnect(this, &Widget::logout, login, &Login::show);
}

void Widget::showbook()
{
    book = new Book(this);
    book->setWidgetInstance(this);
    book->setScreenInstance(parentWidget);
    connect(this, &Widget::gobook, book, &Book::getsignal);
    QFrame *widgetFrame = findChild<QFrame *>("Widget");
    if (widgetFrame) {
        widgetFrame->hide();
    }
    book -> getTcpSocket(tcpSocket);
    emit gobook(ID, PW, NAME, departure, arrival);
    disconnect(this, &Widget::gobook, book, &Book::getsignal);
}

void Widget::showjoin()
{
    join = new Join(this);
    join->setWidgetInstance(this);
    connect(this, SIGNAL(gojoin()), join, SLOT(getsignal()));
    join -> getTcpSocket(tcpSocket);
    emit gojoin();
}

void Widget::showfind()
{
    find = new Find(this);
    find->setWidgetInstance(this);
    connect(this, SIGNAL(gofind()), find, SLOT(getsignal()));
    find -> getTcpSocket(tcpSocket);
    emit gofind();
}

void Widget::showtrainlist(const QString &message)
{
    qDebug() << message;
    trainlist = new trainList(this);
    trainlist->setWidgetInstance(this);
    trainlist->getMessage(message);
    connect(this, SIGNAL(gotrainlist()), trainlist, SLOT(getsignal()));
    trainlist -> getTcpSocket(tcpSocket);
    emit gotrainlist();
    disconnect(this, SIGNAL(gotrainlist()), trainlist, SLOT(getsignal()));
}

void Widget::showseat(const int &num)
{
    qDebug() << num << ":widget";
    seat = new Seat(this);
    seat->setWidgetInstance(this);
    connect(this, SIGNAL(goseat(int)), seat, SLOT(getsignal(int)));
    seat->getTcpSocket(tcpSocket);
    emit goseat(num);
    disconnect(this, SIGNAL(goseat(int)), seat, SLOT(getsignal(int)));
}


//void Widget::logout()
//{
//    find = new Find(this);
//    join = new Join(this);
//    login = new Login(this);
//    login->setWidgetInstance(this);
//    connect(this, SIGNAL(gologin()), login, SLOT(getsignal()));
//    login -> getTcpSocket(tcpSocket);
//    emit gologin();
//}

void Widget::getuserinfo(const QString &id, const QString &password, const QString &dep, const QString &arv)
{
    ID = id;
    PW = password;
    departure = dep;
    arrival = arv;
    connect(this, &Widget::sendInfo, parentWidget, &Screen::getuserinfo);
    emit sendInfo(ID, PW, departure, arrival);
    disconnect(this, &Widget::sendInfo, parentWidget, &Screen::getuserinfo);
    this->hide();
}

void Widget::getinfo(const QString &id, const QString &password, const QString &dep, const QString &arv)
{
    ID = id;
    PW = password;
    departure = dep;
    arrival = arv;
}

void Widget::getsignal(const QString &id, const QString &password, const QString &dep, const QString &arv)
{
    ID = id;
    PW = password;
    departure = dep;
    arrival = arv;
    this->show();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::train_list()
{
    // 레이아웃의 아이템을 모두 제거(메모리 누수 방지)
    QVBoxLayout *layout = trainlist->ui->verticalLayout_7;
    QLayoutItem *item;
    while ((item = layout->takeAt(0))) {
        if (QWidget *widget = item->widget()) {
            delete widget;
        }
        delete item;
    }
    trainlist->ui->label_dep->setText(traininfo.station1);
    trainlist->ui->label_arr->setText(traininfo.station2);
    trainlist->ui->label_sel_date->setText(traininfo.deptime_year+"년 "+traininfo.deptime_month+"월 "+traininfo.deptime_day+"일 ("+traininfo.yoil+")");
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    for (int i = 0; i < 24-traininfo.deptime_hour; i++) {
        // QGroupBox 생성
        QGroupBox *groupBox = new QGroupBox;
        groupBox->setStyleSheet("border: 2px solid gray; border-radius: 5px; padding: 2px;");
        // QVBoxLayout에 들어갈 QHBoxLayout과 label 2개, PushButton 2개
        QHBoxLayout *qhlayout1 = new QHBoxLayout(groupBox);
//        QList<QString> train_nums_list = train_nums[i].split(" ");
        QLabel *label1 = new QLabel(traininfo.trainNum);
        // 문자 + 시간 해서 출력
        QString time_dep = QString::number(i+traininfo.deptime_hour)+":" +QString::number(traininfo.deptime_min);
        QTime time1 = QTime::fromString(time_dep, "hh:mm");
        QTime time2 = QTime::fromString(traininfo.dur_time, "hh:mm");
        QTime resultTime = time1.addSecs(time2.hour() * 3600 + time2.minute() * 60);
        QString time_arr = resultTime.toString("hh:mm");
        QLabel *label2 = new QLabel(traininfo.station1+"\n"+time_dep);
        QLabel *label3 = new QLabel(traininfo.station2+"\n"+time_arr);
        QPushButton *button1 = new QPushButton(QString::number(traininfo.price)+"원");
        QPushButton *button2 = new QPushButton(QString::number(traininfo.price*1.6)+"원");
        gbVector.append(groupBox);
        btnVector1.append(button1);
        btnVector2.append(button2);
        button1->setCheckable(true);
        button2->setCheckable(true);
        buttonGroup->addButton(button1);
        buttonGroup->addButton(button2);
        label1->setStyleSheet("font-size: 9pt; border: none;");
        label2->setStyleSheet("font-size: 9pt; border: none;");
        label3->setStyleSheet("font-size: 9pt; border: none;");
        button1->setStyleSheet("font-size: 9pt; border: 1px solid gray;");
        button2->setStyleSheet("font-size: 9pt; border: 1px solid gray;");

        label1->setFixedSize(69, 50);
        label2->setFixedSize(69, 50);
        label3->setFixedSize(69, 50);
        button1->setFixedSize(69, 50);
        button2->setFixedSize(69, 50);

        qhlayout1->addWidget(label1);
        qhlayout1->addWidget(label2);
        qhlayout1->addWidget(label3);
        qhlayout1->addWidget(button1);
        qhlayout1->addWidget(button2);
        groupBox->setLayout(qhlayout1);
        groupBox->setFixedWidth(358);
        groupBox->setFixedHeight(80);

        // 버튼 클릭 시 하나만 테두리가 들어오고 나머지는 안들어오도록 하는 것 필요
        connect(button1, &QPushButton::clicked, this, [=]() {

            if (button1->isChecked()) {
                // 버튼1 선택됐을 때
                button2->setChecked(false); // 다른 버튼 해제

                trainNum = traininfo.trainNum;
                depStation = traininfo.station1;
                arrStation = traininfo.station2;
                depTime = time_dep;
                arrTime = time_arr;
                trainFee = traininfo.price;
                norm_fee = traininfo.price;
                spec_fee = traininfo.price*1.6;
                num=i;
                qDebug() << QString::number(trainFee);
                groupBox->setStyleSheet("border: 2px solid skyblue; border-radius: 5px; padding: 2px;");
                button1->setStyleSheet("font-size: 9pt; border: 1px solid skyblue;");
                button2->setStyleSheet("font-size: 9pt; border: 1px solid gray;");
                trainlist->showTraininfo();
            } else {
                // 버튼1 선택이 해제되었을 때
                groupBox->setStyleSheet("border: 2px solid gray; border-radius: 5px; padding: 2px;");
                button1->setStyleSheet("font-size: 9pt; border: 1px solid gray;");
            }
        });

        connect(button2, &QPushButton::clicked, this, [=]() {

            if (button2->isChecked()) {
                // 버튼2 선택됐을 때
                button1->setChecked(false); // 다른 버튼 해제

                trainNum = traininfo.trainNum;
                depStation = traininfo.station1;
                arrStation = traininfo.station2;
                depTime = time_dep;
                arrTime = time_arr;
                trainFee = traininfo.price;
                norm_fee = traininfo.price;
                spec_fee = traininfo.price*1.6;
                num=i;
                qDebug() << QString::number(trainFee);
                groupBox->setStyleSheet("border: 2px solid skyblue; border-radius: 5px; padding: 2px;");
                button2->setStyleSheet("font-size: 9pt; border: 1px solid skyblue;");
                button1->setStyleSheet("font-size: 9pt; border: 1px solid gray;");
                trainlist->showTraininfo();
            } else {
                // 버튼2 선택이 해제되었을 때
                groupBox->setStyleSheet("border: 2px solid gray; border-radius: 5px; padding: 2px;");
                button2->setStyleSheet("font-size: 9pt; border: 1px solid gray;");
            }

        });
        trainlist->ui->verticalLayout_7->addWidget(groupBox);
        trainlist->ui->verticalLayout_7->addStretch();
    }
}

void Widget::setData(){
    // 출발역에 맞는 시간 값을 DB에서 가져왔을 때
    // stop_time_vector에 벡터에 넣어서 정차역 돌릴 예정
    int index1;
    int index2;
    // 경부선, 호남선 파악
    if (stt_vector1.indexOf(traininfo.station1)!=-1 && stt_vector1.indexOf(traininfo.station2)!=-1){
        index1 = stt_vector1.indexOf(traininfo.station1);
        index2 = stt_vector1.indexOf(traininfo.station2);
        now_stt_vector = stt_vector1;
    }else{
        index1 = stt_vector2.indexOf(traininfo.station1);
        index2 = stt_vector2.indexOf(traininfo.station2);
        now_stt_vector = stt_vector1;
    }
    traintime->ui->label_traintime_num->setText(traininfo.trainNum);
    traintime->ui->label_traintime_dep->setText(traininfo.station1);
    traintime->ui->label_traintime_arr->setText(traininfo.station2);
    QString train_date = traininfo.deptime_year+"년 "
                         +traininfo.deptime_month+"월 "
                         +traininfo.deptime_day+"일 "
                         +"("+traininfo.yoil+")";
    traintime->ui->label_traintime_date->setText(train_date);

    // 출발역
    QVBoxLayout *qvb = new QVBoxLayout();
    QVBoxLayout *qvbLayout = new QVBoxLayout();
    QFrame *qf = new QFrame();
    QLabel *ql1 = new QLabel(qf);
    QLabel *ql2 = new QLabel(traininfo.station1, qf);
    QLabel *ql3 = new QLabel("-", qf);
    QLabel *ql4 = new QLabel(depTime, qf);
    QPixmap imageT(":/traintimeT.png");
    ql1->setStyleSheet("border:none; background:transparent;");
    ql1->setPixmap(imageT);
    qf->setFixedSize(360,54);
    ql1->setFixedSize(360,54);
    ql2->setGeometry(20,0,80,50);
    ql3->setGeometry(190,0,50,50);
    ql4->setGeometry(290,0,50,50);
    qvb->addWidget(qf);
    qvbLayout->addLayout(qvb);
    // 정차역
    if (index1<index2){
        for(int i=index1+1; i<index2; i++){
            QVBoxLayout *qvb1 = new QVBoxLayout();
            QFrame *qf1 = new QFrame();
            QLabel *ql5 = new QLabel(qf1);
            QLabel *ql6 = new QLabel(now_stt_vector[i], qf1);
            // stop_time_vector[i]+depTime을 더해서 -2값
            QTime time1 = QTime::fromString(stop_time_vector[i], "h:mm");
            QTime time2 = QTime::fromString(depTime, "h:mm");
            QTime resultTime = time1.addSecs(time2.hour() * 3600 + time2.minute() * 60);
            QString time_arr = resultTime.toString("h:mm");
            QTime minusTwoMinutes = resultTime.addSecs(-2 * 60);
            QString time_arr_minus2 = minusTwoMinutes.toString("h:mm");
            QLabel *ql7 = new QLabel(time_arr_minus2, qf1);
            QLabel *ql8 = new QLabel(time_arr, qf1);
            QPixmap imageM(":/traintimeM.png");
            ql5->setStyleSheet("border:none; background:transparent;");
            ql5->setPixmap(imageM);
            qf1->setFixedSize(360,54);
            ql5->setFixedSize(360,54);
            ql6->setGeometry(20,0,80,50);
            ql7->setGeometry(190,0,50,50);
            ql8->setGeometry(290,0,50,50);
            qvb1->addWidget(qf1);
            qvbLayout->addLayout(qvb1);
        }
    }else {
        for(int i=index1+1; i<index2; i++){
            QVBoxLayout *qvb1 = new QVBoxLayout();
            QFrame *qf1 = new QFrame();
            QLabel *ql5 = new QLabel(qf1);
            QLabel *ql6 = new QLabel(now_stt_vector[i], qf1);
            // stop_time_vector[i]+depTime을 더해서 -2값
            QTime time1 = QTime::fromString(stop_time_vector[i], "h:mm");
            QTime time2 = QTime::fromString(depTime, "h:mm");
            QTime resultTime = time1.addSecs(time2.hour() * 3600 + time2.minute() * 60);
            QString time_arr = resultTime.toString("h:mm");
            QTime minusTwoMinutes = resultTime.addSecs(-2 * 60);
            QString time_arr_minus2 = minusTwoMinutes.toString("h:mm");
            QLabel *ql7 = new QLabel(time_arr_minus2, qf1);
            QLabel *ql8 = new QLabel(time_arr, qf1);
            QPixmap imageM(":/traintimeM.png");
            ql5->setStyleSheet("border:none; background:transparent;");
            ql5->setPixmap(imageM);
            qf1->setFixedSize(360,54);
            ql5->setFixedSize(360,54);
            ql6->setGeometry(20,0,80,50);
            ql7->setGeometry(190,0,50,50);
            ql8->setGeometry(290,0,50,50);
            qvb1->addWidget(qf1);
            qvbLayout->addLayout(qvb1);
        }
    }

    // 도착역
    QVBoxLayout *qvb2 = new QVBoxLayout();
    QFrame *qf2 = new QFrame();
    QLabel *ql9 = new QLabel(qf2);
    QLabel *ql10 = new QLabel(traininfo.station2, qf2);
    QLabel *ql11 = new QLabel(arrTime, qf2);
    QLabel *ql12 = new QLabel("-", qf2);
    QPixmap imageB(":/traintimeB.png");
    ql9->setStyleSheet("border:none; background:transparent;");
    ql9->setPixmap(imageB);
    qf2->setFixedSize(360,54);
    ql9->setFixedSize(360,54);
    ql10->setGeometry(20,0,80,50);
    ql11->setGeometry(190,0,50,50);
    ql12->setGeometry(290,0,50,50);
    qvb2->addWidget(qf2);
    QGroupBox *qgb = new QGroupBox();


    qvbLayout->addLayout(qvb2);
    qvbLayout->addStretch();
    qvbLayout->setContentsMargins(0,0,0,0);
    qgb->setLayout(qvbLayout);
    traintime->ui->verticalLayout_7->addWidget(qgb);
    traintime->ui->verticalLayout_7->addStretch();
    traintime->ui->verticalLayout_7->setContentsMargins(0, 0, 0, 0);
}

void Widget::receiveData()
{
    trainfeepage->ui->label_feepage_TN->setText(traininfo.trainNum);
    trainfeepage->ui->label_specA->setText(QString::number(traininfo.price*1.6)+"원");
    trainfeepage->ui->label_specC->setText(QString::number(traininfo.price*1.6*0.64)+"원");
    trainfeepage->ui->label_specS->setText(QString::number(traininfo.price*1.6*0.5)+"원");
    trainfeepage->ui->label_normA->setText(QString::number(traininfo.price)+"원");
    trainfeepage->ui->label_normC->setText(QString::number(traininfo.price*0.5)+"원");
    trainfeepage->ui->label_normS->setText(QString::number(traininfo.price*0.7)+"원");
    trainfeepage->ui->label_standA->setText(QString::number(traininfo.price*0.85)+"원");
    trainfeepage->ui->label_standC->setText(QString::number(traininfo.price*0.85*0.5)+"원");
    trainfeepage->ui->label_standS->setText(QString::number(traininfo.price*0.85*0.7)+"원");
}
