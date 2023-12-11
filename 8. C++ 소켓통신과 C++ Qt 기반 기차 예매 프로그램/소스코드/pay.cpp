#include "pay.h"
#include "ui_pay.h"
#include "book.h"

Pay::Pay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Pay)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    int price1 = 50000;
    QLocale locale(QLocale::English);
    QString prices = locale.toString(price1) + "원";
    ui->label_20_j->setText(prices);
    ui->jpaybutton_4->setEnabled(false);

    connect(ui->paybutton, &QPushButton::clicked, this, &Pay::setScreen);
    connect(ui->jpaybutton_2, &QPushButton::clicked, this, &Pay::payment);
    connect(ui->jpaybutton_4, &QPushButton::clicked, this, &Pay::setScreen1);
    connect(ui->button, &QPushButton::clicked, this, &Pay::payment);
    connect(ui->button1, &QPushButton::clicked, this, &Pay::payment_card);
    connect(ui->hidebutton, &QPushButton::clicked, this, &Pay::hide);
    connect(ui->radioButton_0, &QRadioButton::toggled, this, &Pay::test);
    connect(ui->radioButton_1, &QRadioButton::toggled, this, &Pay::test);
    connect(ui->radioButton12, &QRadioButton::toggled, this, &Pay::test);
    connect(ui->radioButton_3, &QRadioButton::toggled, this, &Pay::test);
    connect(ui->radioButton_4, &QRadioButton::toggled, this, &Pay::test);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Pay::lastpay);
    connect(ui->jpaybutton_4, &QPushButton::clicked, this, &Pay::out);

    connect(ui->backbtn, &QPushButton::clicked, this, &Pay::backmenu);

    //승욱오빠
    QDateTime today_fulltime = QDateTime::currentDateTime();
    QString today_time = today_fulltime.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_todaytime->setText(today_time);

    // 선택한 좌석을 seats 라는 벡터에 담았을 떄
    // frame 안에 vertical을 horizontal로 3개 담고
    // vertical에 라벨 2개를 1:4 비율로 담는다
    // 3번째에는 QR코드를 넣는다.
    // 그 밑에는 label 1개
    // 그 밑에는 label 2개가 horizontal로 들어있는 frame1개
    // 이 모든 걸 groupbox애 담아 verticalLayout_13에 담기
    seats = {"1A", "3B"};
    QVector<QString> hochars = {"1123", "1234"} ;
    for (int i=0; i<seats.size(); i++){
        ui->verticalLayout_11->setContentsMargins(0, 0, 0, 0);

        QHBoxLayout *ticketlayout = new QHBoxLayout;
        QHBoxLayout *ticketlayout_2 = new QHBoxLayout;
        ticketlayout->setContentsMargins(0, 0, 0, 0);
        QLabel *ql1 = new QLabel("타는곳번호");
        QLabel *ql2 = new QLabel();
        QLabel *ql3 = new QLabel("호차번호");
        QLabel *ql4 = new QLabel(hochars[i]);
        QLabel *ql5 = new QLabel("좌석번호");
        QLabel *ql6 = new QLabel(seats[i]);
        QLabel *receipt = new QLabel("운임영수증");
        QLabel *QRlabel = new QLabel;
        QPixmap img(":/new/prefix1/train_station.png");
        QPixmap QRimg(":/new/prefix1/QR.png"); //qr 이미지 넣기

        ql1->setStyleSheet("background-color: lightgray; border-bottom: 2px solid gray; border-left: 2px solid gray;");
        ql3->setStyleSheet("background-color: lightgray; border-bottom: 2px solid gray; border-left: 2px solid gray;");
        ql5->setStyleSheet("background-color: lightgray; border-bottom: 2px solid gray; border-left: 2px solid gray;");
        receipt->setStyleSheet("background-color: lightgray; border-bottom: 2px solid gray; border-left: 2px solid gray;");

        ql1->setAlignment(Qt::AlignCenter);
        ql3->setAlignment(Qt::AlignCenter);
        ql5->setAlignment(Qt::AlignCenter);
        receipt->setAlignment(Qt::AlignCenter);

        ql1->setFixedSize(80, 20);
        ql3->setFixedSize(80, 20);
        ql5->setFixedSize(80, 20);
        receipt->setFixedSize(80, 20);

        ticketlayout->addWidget(ql1);
        ticketlayout->addWidget(ql3);
        ticketlayout->addWidget(ql5);
        ticketlayout->addWidget(receipt);

        ui->verticalLayout_11->addLayout(ticketlayout);

        ql2->setStyleSheet("background-color: white; border-bottom: 1px solid gray;");
        ql4->setStyleSheet("background-color: white; color: blue; border-left: 1px solid gray; border-bottom: 1px solid gray;font-size: 15pt");
        ql6->setStyleSheet("background-color: white; color: blue; border-left: 1px solid gray; border-bottom: 1px solid gray;font-size: 15pt");
        QRlabel->setStyleSheet("background-color: white; border-left: 1px solid gray; border-bottom: 1px solid gray;");

        ql2->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        ql4->setAlignment(Qt::AlignCenter);
        ql6->setAlignment(Qt::AlignCenter);
        QRlabel->setAlignment(Qt::AlignCenter);

        ql2->setFixedSize(80, 80);
        ql4->setFixedSize(80, 80);
        ql6->setFixedSize(80, 80);
        QRlabel->setFixedSize(80, 80);

        // 타는 곳 번호(15분 전에 표시됩니다 아이콘)
        ql2->setPixmap(img);
//        ql2->setScaledContents(true);
        // QR코드 이미지
        QRlabel->setPixmap(QRimg);
//        QRlabel->setScaledContents(true);

        ticketlayout_2->addWidget(ql2);
        ticketlayout_2->addWidget(ql4);
        ticketlayout_2->addWidget(ql6);
        ticketlayout_2->addWidget(QRlabel);

        ui->verticalLayout_11->addLayout(ticketlayout_2);

        QLabel *ticketinfo = new QLabel;
        ticketinfo->setFixedSize(320,100);
        ticketinfo->setStyleSheet("background-color: white;");

        ui->verticalLayout_11->addWidget(ticketinfo);

    }
    ui->verticalLayout_11->addStretch();

}

// 하단 메뉴바 관련 함수
void Pay::setUnderButton()
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

        QPixmap trainimg(":/new/prefix1/train.png");
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

        QPixmap ticketimg(":/new/prefix1/tickets.png");
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

//주옥

void Pay::setScreen()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Pay::setScreen1()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Pay::test(bool checked)
{
    QVector <int> sale = {70,90,20,20,50};
    int price = 50000;
    QRadioButton *salebutton = qobject_cast<QRadioButton *>(sender());
    if(salebutton) {
        QString buttonname = salebutton->objectName();
        buttonname.remove("radioButton_");
        price = price - (price * sale[buttonname.toInt()] / 100);

        // 원에 , 표시
        QLocale locale(QLocale::English);
        QString pricestr = locale.toString(price);
        pricestr = pricestr + "원";
        ui->label_20_j->setText(pricestr);
    }
}

//승차권확인
void Pay::payment()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->button->setStyleSheet("QPushButton { border: none; border-bottom: 2px solid rgb(116, 122, 232); }");
    ui->button1->setStyleSheet("QPushButton { border: none;}");

}

void Pay::payment_card()
{
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->button->setStyleSheet("QPushButton { border: none; }");
    ui->button1->setStyleSheet("QPushButton { border: none; border-bottom: 2px solid rgb(116, 122, 232); }");
}

void Pay::lastpay()
{
    ui->jpaybutton_4->setStyleSheet("background-color: rgb(204, 255, 244); color: rgb(116, 122, 232); font: 13pt; border:none;");
    ui->jpaybutton_4->setEnabled(true);
}

void Pay::out()
{

}

void Pay::backmenu()
{

}

Pay::~Pay()
{
    delete ui;
}

