#include "trainlist.h"
#include "ui_trainlist.h"
#include <QMouseEvent>
#include <QLineEdit>
#include <QDateTime>
#include "widget.h"
#include "traintime.h"
#include "trainfeepage.h"
#include "Seat.h"
#include "book.h"


trainList::trainList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::trainList)
{
    ui->setupUi(this);
    // 열차시각, 운임정보 페이지 열기 위한 변수
    traintime = new trainTime();
    trainfeepage = new trainFeePage();
    // 시간 넣기(우선은 오늘 시간을 예로 들음)
    currentDateTime = QDateTime::currentDateTime();
    year = currentDateTime.date().year();
    month = currentDateTime.date().month();
    day = currentDateTime.date().day();
    // 이전날, 다음날 버튼을 구현하기 위한 것
    QDate date1 = QDate(year, month, day);
    QDate date2 = QDate(QDateTime::currentDateTime().date().year(),
                        QDateTime::currentDateTime().date().month(),
                        QDateTime::currentDateTime().date().day());
    day_dif = date2.daysTo(date1);
    hour = currentDateTime.time().hour();
    minute = currentDateTime.time().minute();
    dayOfWeek = currentDateTime.toString("dddd");
    dow_eng = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    dow_kor = {"일", "월", "화", "수", "목", "금", "토"};
    int index = dow_eng.indexOf(dayOfWeek);
    yoil = dow_kor[index];
    ui->label_sel_date->setText(QString("%1년 %2월 %3일(%4)").arg(year).arg(month).arg(day).arg(yoil));

    // 아래의 문장이 있어야 eventFilter 함수가 실행됨
    qApp->installEventFilter(this);

    // DB 오픈
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("localhost");        // MySQL 서버 호스트
//    db.setPort(3306);
//    db.setDatabaseName("TESTDB"); // 사용할 데이터베이스 이름
//    db.setUserName("test");       // MySQL 사용자 이름
//    db.setPassword("test");        // MySQL 비밀번호
//    if (db.open()) {
//        qDebug() << "데이터베이스 연결 성공";
//    } else {
//        qDebug() << "데이터베이스 연결 실패: " << db.lastError().text();
//    }

    // 메뉴 애니메이션 관련
    widgetContainer_ani = new QWidget(this);
    widgetLayout_ani = new QVBoxLayout(widgetContainer_ani);
    widgetContainer_ani->hide();
    connect(ui->showMyinfoBtn, SIGNAL(clicked()), this, SLOT(showMyinfo()));
    // 버튼 애니메이션 관련
    widgetContainer_ani2 = new QWidget(this);
    widgetLayout_ani2 = new QVBoxLayout(widgetContainer_ani2);
    widgetContainer_ani2->hide();
    setUnderButton();
    connect(ui->backbutton, SIGNAL(clicked()), this, SLOT(backbook()));

    tcpSocket = new QTcpSocket(this);
}

trainList::~trainList()
{
    delete ui;
}

// 내정보 페이지
void trainList::showMyinfo()
{
    if(!widgetContainer_ani->isVisible()) {
        QFrame *frame = new QFrame(widgetContainer_ani);
        frame -> setFixedSize(330, 700);
        QVBoxLayout *vbl = new QVBoxLayout(frame);
        QLineEdit *search = new QLineEdit;
        frame->setStyleSheet("QFrame {"
                             "background-color: rgb(252, 251, 218);"
                             "border-radius: 20px;"
                             "border: 5px solid rgb(85, 170, 255)"
                             "}");

        QLabel *expandingWidget;
        QPushButton *button;
        QComboBox *comboBox;
        expandingWidget = new QLabel("늘어나는 위젯");
        button = new QPushButton("버튼");
        comboBox = new QComboBox;
        vbl->addWidget(expandingWidget);
        vbl->addWidget(search);
        vbl->addWidget(button);
        vbl->addWidget(comboBox);

        search->setPlaceholderText("역 명을 입력해주세요.");
        widgetLayout_ani->addStretch();
        widgetContainer_ani->show();

        animation = new QPropertyAnimation(widgetContainer_ani, "geometry");
        animation->setDuration(300);


        animation->setStartValue(QRect(360, 0, 330, 700));
        animation->setEndValue(QRect(30, 0, 330, 700));
        animation->start();

        if (button) {
            button->setVisible(true);
        }
        if (comboBox) {
            comboBox->setVisible(true);
        }
    }
}

// 범위 지정 후 애니메이션 사라지게 하는 함수
bool trainList::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        QRect containerRect = widgetContainer_ani->geometry();
//        QRect containerRect2 = widgetContainer_ani2->geometry();

        if (!containerRect.contains(mouseEvent->pos())) {
            if (widgetContainer_ani->isVisible()) {
                animation = new QPropertyAnimation(widgetContainer_ani, "geometry");
                animation->setDuration(300);

                animation->setStartValue(QRect(30, 0, 330, 700));
                animation->setEndValue(QRect(360, 0, 330, 700));
                connect(animation, &QPropertyAnimation::finished, widgetContainer_ani, &QWidget::hide);
                animation->start();

                QLayoutItem* item8;
                while ((item8 = widgetLayout_ani->takeAt(0))) {
                    if (QWidget* widget8 = item8->widget()) {
                        widget8->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                    }
                    delete item8;
                }
            }

        }
        // 버튼의 범위에서
        else if (containerRect2.contains(mouseEvent->pos())) {
            QWidget *mainwidget = QApplication::activeWindow();
            if(mainwidget) {
                QList<QPushButton *> buttons = mainwidget->findChildren<QPushButton *>();
                for (QPushButton* button : buttons) {
                    button->setEnabled(true);
                }
            }

            if (widgetContainer_ani2->isVisible()) {
                gbVector[num]->setStyleSheet("border: 2px solid gray; border-radius: 5px; padding: 2px;");
                btnVector1[num]->setStyleSheet("font-size: 9pt; border: 1px solid gray;");
                btnVector2[num]->setStyleSheet("font-size: 9pt; border: 1px solid gray;");
                animation = new QPropertyAnimation(widgetContainer_ani2, "geometry");
                animation->setDuration(500);

                animation->setStartValue(QRect(0, 520, 360, 120));
                animation->setEndValue(QRect(0, 640, 360, 120));
                connect(animation, &QPropertyAnimation::finished, widgetContainer_ani2, &QWidget::hide);
                animation->start();

                QLayoutItem* item9;
                while ((item9 = widgetLayout_ani2->takeAt(0))) {
                    if (QWidget* widget9 = item9->widget()) {
                        widget9->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                    }
                    delete item9;
                }
            }

        }

    }

    // 이벤트를 계속 처리하도록 반환
    return QWidget::eventFilter(obj, event);
}

// 열차 조회 페이지에서 금액 누를 시 나오는
// 열차시각, 운임요금을 조회할 수 있는 페이지 열기
void trainList::showTraininfo()
{
    QWidget *mainwidget = QApplication::activeWindow();
    if(mainwidget) {
        QList<QPushButton *> buttons = mainwidget->findChildren<QPushButton *>();
        for (QPushButton* button : buttons) {
            button->setEnabled(false);
        }
    }

    if(!widgetContainer_ani2->isVisible()) {
        QFrame *frame = new QFrame(widgetContainer_ani2);
        frame->setFixedSize(360,120);
        QVBoxLayout *frameLayout = new QVBoxLayout(frame);
        QHBoxLayout *qhbl = new QHBoxLayout;
        QHBoxLayout *qhbl2 = new QHBoxLayout;
        QVBoxLayout *qvbl = new QVBoxLayout;
        QPushButton *qpb = new QPushButton();
        QPushButton *qpb1 = new QPushButton("열차시각");
        QPushButton *qpb2 = new QPushButton("운임요금");
        QPushButton *qpb3 = new QPushButton("좌석선택");
        QPushButton *qpb4 = new QPushButton("예매");
        qpb->setFixedSize(20,20);
        qpb1->setFixedSize(80,40);
        qpb2->setFixedSize(80,40);
        qpb3->setFixedSize(80,40);
        qpb4->setFixedSize(337,30);
        QString imagePath = ":/below.png";
        QPixmap image(imagePath);
        qpb->setIcon(image);
        qpb->setIconSize(image.rect().size());
        frame->setStyleSheet("QFrame {"
                             "background-color: rgb(14, 60, 96);"
                             "border-radius: 10px;"
                             "border:3px solid rgb(14, 220, 96);"
                             "}");
        qpb->setStyleSheet("QPushButton {"
                           "border:none;"
                           "background:transparent;"
                           "}");
        qpb1->setStyleSheet("QPushButton {"
                            "border:none;"
                            "background:transparent;"
                            "}");
        qpb2->setStyleSheet("QPushButton {"
                            "border:none;"
                            "background:transparent;"
                            "}");
        qpb3->setStyleSheet("QPushButton {"
                            "border:none;"
                            "background:transparent;"
                            "}");
        qpb4->setStyleSheet("QPushButton {"
                            "border:none;"
                            "background:transparent;"
                            "background-color:rgb(14,220,96);"
                            "}");
        containerRect2 = qpb->geometry();
        // 열차시각 위젯(페이지)이 등장하도록 할 것
        connect(qpb1, &QPushButton::clicked, this, [=](){
            showTrainTime();
        });
        connect(qpb2, &QPushButton::clicked, this, [=](){
            // 운임요금 위젯(페이지)이 등장하도록 할 것
            showTrainFeePage();
        });
        connect(qpb3, &QPushButton::clicked, this, [=](){

            showseat();
        });
        connect(qpb4, &QPushButton::clicked, this, [=](){

            // 예매 버튼 누를 시 랜덤 좌석 생성

            frame->hide();

            QList<QPushButton *> buttons = mainwidget->findChildren<QPushButton *>();
            for (QPushButton* button : buttons) {
                button->setEnabled(true);
            }
            animation = new QPropertyAnimation(widgetContainer_ani2, "geometry");
            animation->setDuration(500);
            animation->setStartValue(QRect(0, 520, 360, 120));
            animation->setEndValue(QRect(0, 640, 360, 120));
            connect(animation, &QPropertyAnimation::finished, widgetContainer_ani2, &QWidget::hide);
            animation->start();
            QLayoutItem* item9;
            while ((item9 = widgetLayout_ani2->takeAt(0))) {
                if (QWidget* widget9 = item9->widget()) {
                    widget9->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                }
                delete item9;
            }
            if (parentWidget->traininfo.tripway == "편도"){
                QMessageBox::information(this, "예매", "편도 승차권 예약이 완료되었습니다.");
            }
            else if (parentWidget->traininfo.tripway == "왕복"){
                if (flag == 0){
                    QMessageBox::information(this, "예매", "가는 승차권 예약이 완료되었습니다.");
                    flag = 1;
                    parentWidget->traininfo = parentWidget->traininfo_dep;
                    //////////////////////////////////나중에 다시 확인///////////////////////////////////////
                    parentWidget->train_list();
                }
                else if (flag==1){
                    QMessageBox::information(this, "예매", "오는 승차권 예약이 완료되었습니다.");
                    flag = 2;
                    // 예매페이지
                }

            }

        });
        qhbl2->addWidget(qpb1);
        qhbl2->addWidget(qpb2);
        qhbl2->addWidget(qpb3);
        qvbl->addWidget(qpb4);
        // qpb 버튼을 수평 가운데에 배치
        qhbl->addStretch(); // 왼쪽 여백
        qhbl->addWidget(qpb); // 가운데 배치
        qhbl->addStretch(); // 오른쪽 여백
        frameLayout->addLayout(qhbl);
        frameLayout->addLayout(qhbl2);
        frameLayout->addLayout(qvbl);

        widgetLayout_ani2->addStretch();
        widgetContainer_ani2->show();
        animation = new QPropertyAnimation(widgetContainer_ani2, "geometry");
        animation->setDuration(500);
        animation->setStartValue(QRect(0, 640, 360, 120));
        animation->setEndValue(QRect(0, 520, 360, 120));
        animation->start();

    }
}


// 이전날 버튼 눌렀을 때 이벤트(어제날짜 부터는 안 나오도록)
void trainList::on_pushButton_preDate_clicked()
{
    if(day_dif>0){
        currentDateTime = currentDateTime.addDays(-1);
        year = currentDateTime.date().year();
        month = currentDateTime.date().month();
        day = currentDateTime.date().day();
        QDate date1 = QDate(year, month, day);
        QDate date2 = QDate(QDateTime::currentDateTime().date().year(),
                            QDateTime::currentDateTime().date().month(),
                            QDateTime::currentDateTime().date().day());
        day_dif = date2.daysTo(date1);
        qDebug() << QString::number(day_dif);
        if (day_dif>=0){
            dayOfWeek = currentDateTime.toString("dddd");
            int index = dow_eng.indexOf(dayOfWeek);
            yoil = dow_kor[index];
            ui->label_sel_date->setText(QString("%1년 %2월 %3일(%4)").arg(year).arg(month).arg(day).arg(yoil));
            if(month == QDateTime::currentDateTime().date().month() && day == QDateTime::currentDateTime().date().day()){
                hour = QDateTime::currentDateTime().time().hour();
                minute = QDateTime::currentDateTime().time().minute();
                qDebug() << QString::number(hour) + ":" + QString::number(minute);
            }else{
                hour = 0;
                minute = 0;
                qDebug() << QString::number(hour) + ":" + QString::number(minute);
            }
            QStringList findday_1 = dateMessage.split(",");
            int pre_day = findday_1[5].toInt() - 1;
            findday_1[5] = QString::number(pre_day);
            findday_1[6] = "4";
            dateMessage = findday_1.join(",");
            tcpSocket->write(dateMessage.toUtf8());
        }
    }
}

// 다음날 버튼 누를 시 이벤트(오늘 이후 30일까지 가능하도록)
void trainList::on_pushButton_nextDate_clicked()
{
    if(day_dif<=30){
        currentDateTime = currentDateTime.addDays(1);
        year = currentDateTime.date().year();
        month = currentDateTime.date().month();
        day = currentDateTime.date().day();
        QDate date1 = QDate(year, month, day);
        QDate date2 = QDate(QDateTime::currentDateTime().date().year(),
                            QDateTime::currentDateTime().date().month(),
                            QDateTime::currentDateTime().date().day());
        day_dif = date2.daysTo(date1);
        qDebug() << QString::number(day_dif);
        if (day_dif<=31){
            dayOfWeek = currentDateTime.toString("dddd");
            int index = dow_eng.indexOf(dayOfWeek);
            yoil = dow_kor[index];
            ui->label_sel_date->setText(QString("%1년 %2월 %3일(%4)").arg(year).arg(month).arg(day).arg(yoil));
            if(month == QDateTime::currentDateTime().date().month() && day == QDateTime::currentDateTime().date().day()){
                hour = QDateTime::currentDateTime().time().hour();
                minute = QDateTime::currentDateTime().time().minute();
                qDebug() << QString::number(hour) + ":" + QString::number(minute);
            }else{
                hour = 0;
                minute = 0;
                qDebug() << QString::number(hour) + ":" + QString::number(minute);
            }
            QStringList findday_2 = dateMessage.split(",");
            int next_day = findday_2[5].toInt() + 1;
            findday_2[5] = QString::number(next_day);
            findday_2[6] = "4";
            dateMessage = findday_2.join(",");
            tcpSocket->write(dateMessage.toUtf8());
        }
    }
}

// traintime(열차 시각) 클래스에 구조체로 데이터를 보내고
// 열차 시각 페이지 열기
void trainList::showTrainTime(){
    traintime->show();
}

// trainfeepage(운임 정보) 클래스에 열차번호, 일반실/특실 요금을 보내고
// 운임 정보 페이지 띄움
void trainList::showTrainFeePage(){
    trainfeepage->show();
}

void trainList::showseat()
{
    connect(this, &trainList::goseat, parentWidget, &Widget::showseat);
    QFrame *trainlistFrame = findChild<QFrame *>("trainList");
    if (trainlistFrame) {
        trainlistFrame->hide();
    }
    emit goseat(all_custom);
    disconnect(this, &trainList::goseat, parentWidget, &Widget::showseat);
}

// 하단 메뉴바 관련 함수
void trainList::setUnderButton()
{
    QFrame *underbuttonframe = findChild<QFrame *>("frame");
    if(underbuttonframe) {
        QVBoxLayout *vbl = new QVBoxLayout(underbuttonframe);
        QHBoxLayout *hbl = new QHBoxLayout;
        vbl->setContentsMargins(0, 0, 0, 0);

        QPushButton *train = new QPushButton;
        QVBoxLayout *trainlayout = new QVBoxLayout(train);
        QLabel *trainimglabel = new QLabel;
        QLabel *trainbook = new QLabel("승차권예매");
        QHBoxLayout *hbl1 = new QHBoxLayout;
        QHBoxLayout *hbl2 = new QHBoxLayout;

        train->setFixedSize(60, 60);
        train->setStyleSheet("border: none; background: transparent;");

        QPixmap trainimg(":resources/train.png");
        trainimglabel->setStyleSheet("border: none; background: transparent;");
        trainimglabel->setFixedSize(30, 30);
        trainimglabel->setPixmap(trainimg);
        trainimglabel->setScaledContents(true);

        trainbook->setAlignment(Qt::AlignCenter);
        trainbook->setFixedSize(50, 15);
        trainbook->setStyleSheet("border: none; font-size: 8pt;");

        hbl1->addWidget(trainimglabel);
        hbl1->setAlignment(Qt::AlignCenter);
        trainlayout->addLayout(hbl1);
        hbl2->addWidget(trainbook);
        hbl2->setAlignment(Qt::AlignCenter);
        trainlayout->addLayout(hbl2);

        QPushButton *ticket = new QPushButton;
        QVBoxLayout *ticketlayout = new QVBoxLayout(ticket);
        QLabel *ticketimglabel = new QLabel;
        QLabel *ticketbook = new QLabel("승차권확인");
        QHBoxLayout *hbl3 = new QHBoxLayout;
        QHBoxLayout *hbl4 = new QHBoxLayout;

        ticket->setFixedSize(60, 60);
        ticket->setStyleSheet("border: none; background: transparent;");

        QPixmap ticketimg(":resources/tickets.png");
        ticketimglabel->setStyleSheet("border: none; background: transparent;");
        ticketimglabel->setFixedSize(30, 30);
        ticketimglabel->setPixmap(ticketimg);
        ticketimglabel->setScaledContents(true);

        ticketbook->setAlignment(Qt::AlignCenter);
        ticketbook->setFixedSize(50, 15);
        ticketbook->setStyleSheet("border: none; font-size: 8pt;");

        hbl3->addWidget(ticketimglabel);
        hbl3->setAlignment(Qt::AlignCenter);
        ticketlayout->addLayout(hbl3);
        hbl4->addWidget(ticketbook);
        hbl4->setAlignment(Qt::AlignCenter);
        ticketlayout->addLayout(hbl4);

        hbl->addWidget(train);
        hbl->addWidget(ticket);
        hbl->setContentsMargins(0, 0, 0, 0);
        vbl->addLayout(hbl);
        vbl->addStretch();

        connect(train, &QPushButton::clicked, [=]() {
            train->setStyleSheet("border: none; background: transparent; border-top: 2px solid rgb(116, 122, 232);");
            ticket->setStyleSheet("border: none; background: transparent;");
            //            승차권 예매 화면으로 이동
            //            hide();
            //            this->show();
        });

        connect(ticket, &QPushButton::clicked, [=]() {
            ticket->setStyleSheet("border: none; background: transparent; border-top: 2px solid rgb(116, 122, 232);");
            train->setStyleSheet("border: none; background: transparent;");
            //            승차권확인 화면으로 이동
            //            showticket();
        });
    }
}

QHostAddress trainList::getMyIP()
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

void trainList::getsignal()
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

void trainList::getTcpSocket(QTcpSocket *tcpsocket)
{
    tcpSocket = tcpsocket;
}

void trainList::getMessage(QString message)
{
    dateMessage = message;
}

void trainList::getuserinfo(QString id, QString password, QString name, QString phonenum, QString email)
{
    ID = id;
    PW = password;
    NAME = name;
    HP = phonenum;
    EM = email;
}

void trainList::backbook()
{
    connect(this, &trainList::gobook, bookWidget, &Book::show);
    QFrame *trainlistFrame = findChild<QFrame *>("trainList");
    if (trainlistFrame) {
        trainlistFrame->hide();
    }
    emit gobook();
    disconnect(this, &trainList::gobook, bookWidget, &Book::show);
}

void trainList::getcustomNum(int num)
{
    all_custom = num;
}
