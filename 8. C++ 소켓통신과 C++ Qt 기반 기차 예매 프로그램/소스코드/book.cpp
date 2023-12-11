#include "book.h"
#include "ui_book.h"
#include <QLineEdit>
#include <QMouseEvent>
#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>
#include <QDateTime>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMessageBox>
#include <QCalendarWidget>
#include <QToolButton>
#include <QTableView>
#include "map.h"
#include "screen.h"
#include "widget.h"
#include "trainlist.h"

Book::Book(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Book)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    // 현재 날짜 시간 설정
    currentDate_1 = QDate::currentDate();
    QDateTime currentdate = QDateTime::currentDateTime();
    int dayofweekidx = currentdate.date().dayOfWeek();
    datetime_1 = currentdate.toString("yyyy년 MM월 dd일 (" + dayofweek[dayofweekidx-1] + ") hh:mm");
    datetime_2 = currentdate.toString("yyyy년 MM월 dd일 (" + dayofweek[dayofweekidx-1] + ") hh:mm");
    datetime_3 = currentdate.toString("yyyy년 MM월 dd일 (" + dayofweek[dayofweekidx-1] + ") hh:mm");

    ui->stackedWidget->setCurrentIndex(0);

    ui->onewaybtn->setStyleSheet("QPushButton { border: none; border-bottom: 2px solid rgb(116, 122, 232); }");

    connect(ui->onewaybtn, SIGNAL(clicked()), this, SLOT(switchscreen()));
    connect(ui->roundtripbtn, SIGNAL(clicked()), this, SLOT(switchscreen()));

    mainLayout_1 = new QVBoxLayout(ui->oneway);
    mainLayout_2 = new QVBoxLayout(ui->roundtrip);

    setmenubuttons(0);

    widgetContainer_8 = new QWidget(this);
    widgetLayout_8 = new QVBoxLayout(widgetContainer_8);

    widgetContainer_8->hide();

    connect(ui->showmyinfobtn, SIGNAL(clicked()), this, SLOT(showmyinfo()));

    setSearchTrainButton();
    connect(ui->searchtrainbtn, SIGNAL(clicked()), this, SLOT(showtrainlist()));
    setUnderButton();
    connect(ui->outbutton, SIGNAL(clicked()), this, SLOT(showscreen()));

    initialize();
}

void Book::initialize()
{
    tcpSocket = new QTcpSocket(this);
}

void Book::setSearchTrainButton()
{
    if(num==0) {
        ui->searchtrainbtn->setEnabled(false);
        ui->searchtrainbtn->setStyleSheet("border: none;"
                                          "color: rgb(166, 186, 255);"
                                          "background-color: rgb(226, 226, 226);");
    } else if(num>0) {
        ui->searchtrainbtn->setEnabled(true);
        ui->searchtrainbtn->setStyleSheet("border: none;"
                                          "color: rgb(116, 122, 232);"
                                          "background-color: rgb(204, 255, 244);");
    }
}

void Book::setUnderButton()
{
    QFrame *underbuttonframe = findChild<QFrame *>("showmenu");
    if(underbuttonframe) {
        QVBoxLayout *vbl = new QVBoxLayout(underbuttonframe);
        QHBoxLayout *hbl = new QHBoxLayout;

        QPushButton *train = new QPushButton;
        QVBoxLayout *trainlayout = new QVBoxLayout(train);
        QLabel *trainimglabel = new QLabel;
        QLabel *trainbook = new QLabel("승차권예매");
        QHBoxLayout *hbl1 = new QHBoxLayout;
        QHBoxLayout *hbl2 = new QHBoxLayout;

        train->setFixedSize(60, 60);
        train->setStyleSheet("border: none; background: transparent;");

        QPixmap trainimg(":/resources/train.png");
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

        QPixmap ticketimg(":/resources/tickets.png");
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
        vbl->setContentsMargins(0, 0, 0, 0);
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

void Book::setmenubuttons(int idx)
{
    if(idx == 0) {
        // 편도 레이아웃 생성
        // 출발지, 도착지
        chooseArea_1 = new QFrame;
        chooseArea_1->setFixedSize(340, 80);
        chooseArea_1->setStyleSheet("QFrame {"
                                    "border: none;}");
        QVBoxLayout *choosearea = new QVBoxLayout(chooseArea_1);
        QHBoxLayout *titlelayout = new QHBoxLayout;
        QHBoxLayout *buttonLayout = new QHBoxLayout;

        QSpacerItem *spacer_1 = new QSpacerItem(60, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        QSpacerItem *spacer_2 = new QSpacerItem(60, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        QLabel *departurearea = new QLabel("출발");
        QPushButton *change = new QPushButton;
        QLabel *arrivalarea = new QLabel("도착");

        departurearea->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");

        arrivalarea->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");

        QIcon changearrowimg(":/resources/refresh.png");
        change->setFixedSize(20, 20);
        change->setStyleSheet("background: transparent; border: none");
        change->setIcon(changearrowimg);
        change->setIconSize(QSize(20, 20));

        titlelayout->addItem(spacer_1);
        titlelayout->addWidget(departurearea);
        titlelayout->addWidget(change);
        titlelayout->addItem(spacer_2);
        titlelayout->addWidget(arrivalarea);


        departure = new QPushButton;
        QLabel *goarrow = new QLabel;
        arrival = new QPushButton;

        QPixmap goarrowimg(":/resources/arrow-right.png");
        goarrow->setStyleSheet("background: transparent;");
        goarrow->setFixedSize(20, 20);
        goarrow->setPixmap(goarrowimg);
        goarrow->setScaledContents(true);

        departure->setFixedSize(100, 40);
        departure->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold;");
        departure->setText(station1);

        arrival->setFixedSize(100, 40);
        arrival->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold;");
        arrival->setText(station2);

        buttonLayout->addWidget(departure);
        buttonLayout->addWidget(goarrow);
        buttonLayout->addWidget(arrival);

        QHBoxLayout *centerlayout = new QHBoxLayout;
        QPushButton *showmapbtn = new QPushButton;
        showmapbtn->setText("KTX역 선택 지도");
        showmapbtn->setFixedSize(300, 30);
        showmapbtn->setStyleSheet("QPushButton {"
                               "background-color: rgb(255, 255, 255);"
                               "color: rgb(116, 122, 232);"
                               "border: 2px solid rgb(116, 122, 232);"
                               "border-radius: 8px;}");
        choosearea->addLayout(titlelayout);
        choosearea->addLayout(buttonLayout);
        mainLayout_1->addWidget(chooseArea_1);
        centerlayout->addWidget(showmapbtn);
        centerlayout->setAlignment(Qt::AlignCenter);
        mainLayout_1->addLayout(centerlayout);

        connect(showmapbtn, SIGNAL(clicked()), this, SLOT(showmap()));

        // 위젯을 담을 컨테이너 위젯 생성
        widgetContainer_1 = new QWidget(this);
        widgetLayout_1 = new QVBoxLayout(widgetContainer_1);

        widgetContainer_1->hide();

        connect(change, &QPushButton::clicked, [=]() {
            QString temp;
            temp = station1;
            station1 = station2;
            station2 = temp;
            departure->setText(station1);
            arrival->setText(station2);
        });

        // 출발일
        toggleButton_2 = new QPushButton;
        QVBoxLayout *lvl2 = new QVBoxLayout(toggleButton_2);
        QHBoxLayout *lbl2_1 = new QHBoxLayout;
        QHBoxLayout *lbl2_2 = new QHBoxLayout;
        QHBoxLayout *lbl2_3 = new QHBoxLayout;

        QLabel *departuredate = new QLabel("출발일");
        date_1 = new QLabel;
        date_1->setText(datetime_1);

        arrow_2 = new QLabel;
        departuredate->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");
        date_1->setStyleSheet("background: transparent;");
        arrowimg_2.load(":/resources/down-arrow_1.png");
        arrow_2->setStyleSheet("background: transparent;");
        arrow_2->setFixedSize(20, 20);
        arrow_2->setPixmap(arrowimg_2);
        arrow_2->setScaledContents(true);

        QFont font2_1;
        font2_1.setPointSize(10);
        departuredate->setFont(font2_1);
        QFont font2_2;
        font2_2.setPointSize(13);
        date_1->setFont(font2_2);

        lbl2_1->addWidget(departuredate);
        lbl2_1->setAlignment(Qt::AlignCenter);

        lbl2_2->addWidget(date_1);
        lbl2_2->setAlignment(Qt::AlignCenter);

        lbl2_3->addWidget(arrow_2);
        lbl2_2->setAlignment(Qt::AlignCenter);

        lvl2->addLayout(lbl2_1);
        lvl2->addLayout(lbl2_2);
        lvl2->addLayout(lbl2_3);
        toggleButton_2->setFixedSize(340, 90);
        toggleButton_2->setStyleSheet("QPushButton {"
                                      "border: none;"
                                      "border-top: 1px solid rgb(226, 226, 226);}");
        mainLayout_1->addWidget(toggleButton_2);

        widgetContainer_2 = new QWidget(this);
        widgetLayout_2 = new QVBoxLayout(widgetContainer_2);
        widgetContainer_2->hide();
        mainLayout_1->addWidget(widgetContainer_2);

        // 승객 인원수
        toggleButton_3 = new QPushButton;
        QVBoxLayout *lvl3 = new QVBoxLayout(toggleButton_3);
        QHBoxLayout *lbl3_1 = new QHBoxLayout;
        QHBoxLayout *lbl3_2 = new QHBoxLayout;
        QHBoxLayout *lbl3_3 = new QHBoxLayout;

        QLabel *customnumtitle = new QLabel("승객 연령 및 좌석수");
        customnum_1 = new QLabel(QString("총 %1명").arg(num));
        arrow_3 = new QLabel;
        customnumtitle->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");
        customnum_1->setStyleSheet("background: transparent;");
        arrowimg_3.load(":/resources/down-arrow_1.png");
        arrow_3->setStyleSheet("background: transparent;");
        arrow_3->setFixedSize(20, 20);
        arrow_3->setPixmap(arrowimg_3);
        arrow_3->setScaledContents(true);

        QFont font3_1;
        font3_1.setPointSize(10);
        customnumtitle->setFont(font3_1);
        QFont font3_2;
        font3_2.setPointSize(13);
        customnum_1->setFont(font3_2);

        lbl3_1->addWidget(customnumtitle);
        lbl3_1->setAlignment(Qt::AlignCenter);

        lbl3_2->addWidget(customnum_1);
        lbl3_2->setAlignment(Qt::AlignCenter);

        lbl3_3->addWidget(arrow_3);
        lbl3_2->setAlignment(Qt::AlignCenter);

        lvl3->addLayout(lbl3_1);
        lvl3->addLayout(lbl3_2);
        lvl3->addLayout(lbl3_3);

        toggleButton_3->setFixedSize(340, 90);
        toggleButton_3->setStyleSheet("QPushButton {"
                                      "border: none;"
                                      "border-top: 1px solid rgb(226, 226, 226);}");
        mainLayout_1->addWidget(toggleButton_3);
        widgetContainer_3 = new QWidget(this);
        widgetLayout_3 = new QVBoxLayout(widgetContainer_3);
        widgetContainer_3->hide();
        mainLayout_1->addWidget(widgetContainer_3);

        mainLayout_1->addStretch();


        // 버튼 클릭 시 토글 동작 설정
        connect(departure, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
        connect(arrival, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
        connect(toggleButton_2, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
        connect(toggleButton_3, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
    } else if(idx == 1) {
        // 왕복 레이아웃 생성
        // 출발지, 도착지
        chooseArea_4 = new QFrame;
        chooseArea_4->setFixedSize(340, 80);
        chooseArea_4->setStyleSheet("QFrame {"
                                    "border: none;}");
        QVBoxLayout *choosearea = new QVBoxLayout(chooseArea_4);
        QHBoxLayout *titlelayout = new QHBoxLayout;
        QHBoxLayout *buttonLayout = new QHBoxLayout;

        QSpacerItem *spacer_1 = new QSpacerItem(60, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        QSpacerItem *spacer_2 = new QSpacerItem(60, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        QLabel *departurearea = new QLabel("출발");
        QPushButton *change = new QPushButton;
        QLabel *arrivalarea = new QLabel("도착");

        departurearea->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");

        arrivalarea->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");

        QIcon changearrowimg(":/resources/refresh.png");
        change->setFixedSize(20, 20);
        change->setStyleSheet("background: transparent; border: none");
        change->setIcon(changearrowimg);
        change->setIconSize(QSize(20, 20));

        titlelayout->addItem(spacer_1);
        titlelayout->addWidget(departurearea);
        titlelayout->addWidget(change);
        titlelayout->addItem(spacer_2);
        titlelayout->addWidget(arrivalarea);


        departure2 = new QPushButton;
        QLabel *goarrow = new QLabel;
        arrival2 = new QPushButton;

        QPixmap goarrowimg(":/resources/exchange.png");
        goarrow->setStyleSheet("background: transparent;");
        goarrow->setFixedSize(20, 20);
        goarrow->setPixmap(goarrowimg);
        goarrow->setScaledContents(true);

        departure2->setFixedSize(100, 40);
        departure2->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold;");
        departure2->setText(station1);

        arrival2->setFixedSize(100, 40);
        arrival2->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold;");
        arrival2->setText(station2);

        buttonLayout->addWidget(departure2);
        buttonLayout->addWidget(goarrow);
        buttonLayout->addWidget(arrival2);

        QHBoxLayout *centerlayout = new QHBoxLayout;
        QPushButton *showmapbtn_2 = new QPushButton;
        showmapbtn_2->setText("KTX역 선택 지도");
        showmapbtn_2->setFixedSize(300, 30);
        showmapbtn_2->setStyleSheet("QPushButton {"
                               "background-color: rgb(255, 255, 255);"
                               "color: rgb(116, 122, 232);"
                               "border: 2px solid rgb(116, 122, 232);"
                               "border-radius: 8px;}");
        choosearea->addLayout(titlelayout);
        choosearea->addLayout(buttonLayout);
        mainLayout_2->addWidget(chooseArea_4);
        centerlayout->addWidget(showmapbtn_2);
        centerlayout->setAlignment(Qt::AlignCenter);
        mainLayout_2->addLayout(centerlayout);

        connect(showmapbtn_2, SIGNAL(clicked()), this, SLOT(showmap()));

        widgetContainer_4 = new QWidget(this);
        widgetLayout_4 = new QVBoxLayout(widgetContainer_4);
        widgetContainer_4->hide();

        connect(change, &QPushButton::clicked, [=]() {
            QString temp;
            temp = station1;
            station1 = station2;
            station2 = temp;
            departure2->setText(station1);
            arrival2->setText(station2);
        });


        // 가는날
        toggleButton_5 = new QPushButton;
        QVBoxLayout *lvl5 = new QVBoxLayout(toggleButton_5);
        QHBoxLayout *lbl5_1 = new QHBoxLayout;
        QHBoxLayout *lbl5_2 = new QHBoxLayout;
        QHBoxLayout *lbl5_3 = new QHBoxLayout;

        QLabel *departuredate_2 = new QLabel("가는날");
        date_2 = new QLabel;
        date_2->setText(datetime_2);

        arrow_5 = new QLabel;
        departuredate_2->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");
        date_2->setStyleSheet("background: transparent;");
        arrowimg_5.load(":/resources/down-arrow_1.png");
        arrow_5->setStyleSheet("background: transparent;");
        arrow_5->setFixedSize(20, 20);
        arrow_5->setPixmap(arrowimg_5);
        arrow_5->setScaledContents(true);

        QFont font5_1;
        font5_1.setPointSize(10);
        departuredate_2->setFont(font5_1);
        QFont font5_2;
        font5_2.setPointSize(13);
        date_2->setFont(font5_2);

        lbl5_1->addWidget(departuredate_2);
        lbl5_1->setAlignment(Qt::AlignCenter);

        lbl5_2->addWidget(date_2);
        lbl5_2->setAlignment(Qt::AlignCenter);

        lbl5_3->addWidget(arrow_5);
        lbl5_2->setAlignment(Qt::AlignCenter);

        lvl5->addLayout(lbl5_1);
        lvl5->addLayout(lbl5_2);
        lvl5->addLayout(lbl5_3);
        toggleButton_5->setFixedSize(340, 90);
        toggleButton_5->setStyleSheet("QPushButton {"
                                      "border: none;"
                                      "border-top: 1px solid rgb(226, 226, 226);}");
        mainLayout_2->addWidget(toggleButton_5);
        widgetContainer_5 = new QWidget(this);
        widgetLayout_5 = new QVBoxLayout(widgetContainer_5);
        widgetContainer_5->hide();
        mainLayout_2->addWidget(widgetContainer_5);

        // 오는날
        toggleButton_6 = new QPushButton;
        QVBoxLayout *lvl6 = new QVBoxLayout(toggleButton_6);
        QHBoxLayout *lbl6_1 = new QHBoxLayout;
        QHBoxLayout *lbl6_2 = new QHBoxLayout;
        QHBoxLayout *lbl6_3 = new QHBoxLayout;

        QLabel *departuredate_3 = new QLabel("오는날");
        date_3 = new QLabel;
        date_3->setText(datetime_3);

        arrow_6 = new QLabel;
        departuredate_3->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");
        date_3->setStyleSheet("background: transparent;");
        arrowimg_6.load(":/resources/down-arrow_1.png");
        arrow_6->setStyleSheet("background: transparent;");
        arrow_6->setFixedSize(20, 20);
        arrow_6->setPixmap(arrowimg_6);
        arrow_6->setScaledContents(true);

        QFont font2_1;
        font2_1.setPointSize(10);
        departuredate_3->setFont(font2_1);
        QFont font2_2;
        font2_2.setPointSize(13);
        date_3->setFont(font2_2);

        lbl6_1->addWidget(departuredate_3);
        lbl6_1->setAlignment(Qt::AlignCenter);

        lbl6_2->addWidget(date_3);
        lbl6_2->setAlignment(Qt::AlignCenter);

        lbl6_3->addWidget(arrow_6);
        lbl6_2->setAlignment(Qt::AlignCenter);

        lvl6->addLayout(lbl6_1);
        lvl6->addLayout(lbl6_2);
        lvl6->addLayout(lbl6_3);
        toggleButton_6->setFixedSize(340, 90);
        toggleButton_6->setStyleSheet("QPushButton {"
                                      "border: none;"
                                      "border-top: 1px solid rgb(226, 226, 226);}");
        mainLayout_2->addWidget(toggleButton_6);
        widgetContainer_6 = new QWidget(this);
        widgetLayout_6 = new QVBoxLayout(widgetContainer_6);
        widgetContainer_6->hide();
        mainLayout_2->addWidget(widgetContainer_6);

        // 승객 인원수
        toggleButton_7 = new QPushButton;
        QVBoxLayout *lvl7 = new QVBoxLayout(toggleButton_7);
        QHBoxLayout *lbl7_1 = new QHBoxLayout;
        QHBoxLayout *lbl7_2 = new QHBoxLayout;
        QHBoxLayout *lbl7_3 = new QHBoxLayout;

        QLabel *customnumtitle = new QLabel("승객 연령 및 좌석수");
        customnum_2 = new QLabel(QString("총 %1명").arg(num));
        arrow_7 = new QLabel;
        customnumtitle->setStyleSheet("color: rgb(116, 122, 232); background: transparent;");
        customnum_2->setStyleSheet("background: transparent;");
        arrowimg_7.load(":/resources/down-arrow_1.png");
        arrow_7->setStyleSheet("background: transparent;");
        arrow_7->setFixedSize(20, 20);
        arrow_7->setPixmap(arrowimg_7);
        arrow_7->setScaledContents(true);

        QFont font3_1;
        font3_1.setPointSize(10);
        customnumtitle->setFont(font3_1);
        QFont font3_2;
        font3_2.setPointSize(13);
        customnum_2->setFont(font3_2);

        lbl7_1->addWidget(customnumtitle);
        lbl7_1->setAlignment(Qt::AlignCenter);

        lbl7_2->addWidget(customnum_2);
        lbl7_2->setAlignment(Qt::AlignCenter);

        lbl7_3->addWidget(arrow_7);
        lbl7_2->setAlignment(Qt::AlignCenter);

        lvl7->addLayout(lbl7_1);
        lvl7->addLayout(lbl7_2);
        lvl7->addLayout(lbl7_3);
        toggleButton_7->setFixedSize(340, 90);
        toggleButton_7->setStyleSheet("QPushButton {"
                                      "border: none;"
                                      "border-top: 1px solid rgb(226, 226, 226);}");
        mainLayout_2->addWidget(toggleButton_7);
        widgetContainer_7 = new QWidget(this);
        widgetLayout_7 = new QVBoxLayout(widgetContainer_7);
        widgetContainer_7->hide();
        mainLayout_2->addWidget(widgetContainer_7);

        mainLayout_2->addStretch();

        connect(departure2, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
        connect(arrival2, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
        connect(toggleButton_5, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
        connect(toggleButton_6, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
        connect(toggleButton_7, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
    }

}

void Book::switchscreen()
{
    int idx;
    QPushButton *switchButton = qobject_cast<QPushButton *>(sender());
    if(switchButton == ui->onewaybtn) {

        ui->onewaybtn->setStyleSheet("QPushButton { border: none; border-bottom: 2px solid rgb(116, 122, 232); }");
        ui->roundtripbtn->setStyleSheet("QPushButton { border:none; color: gray;}");
        if(mainLayout_1) {
            QLayoutItem* item;
            while ((item = mainLayout_1->takeAt(0))) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                }
                delete item;
            }
            delete mainLayout_1;
        }

        ui->stackedWidget->setCurrentIndex(0);
        mainLayout_1 = new QVBoxLayout(ui->oneway);
        idx = ui->stackedWidget->currentIndex();

    } else if(switchButton == ui->roundtripbtn) {

        ui->roundtripbtn->setStyleSheet("QPushButton { border: none; border-bottom: 2px solid rgb(116, 122, 232); }");
        ui->onewaybtn->setStyleSheet("QPushButton { border:none; color: gray;}");
        if(mainLayout_2) {
            QLayoutItem* item;
            while ((item = mainLayout_2->takeAt(0))) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                }
                delete item;
            }
            delete mainLayout_2;
        }

        ui->stackedWidget->setCurrentIndex(1);
        mainLayout_2 = new QVBoxLayout(ui->roundtrip);
        idx = ui->stackedWidget->currentIndex();

    }
    setmenubuttons(idx);
}

void Book::showmyinfo()
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

        connect(logoutbutton, &QPushButton::clicked, this, &Book::showlogin);

        animation = new QPropertyAnimation(widgetContainer_8, "geometry");
        animation->setDuration(300);


        animation->setStartValue(QRect(360, 0, 330, 700));
        animation->setEndValue(QRect(30, 0, 330, 700));
        animation->start();
    }
}

void Book::toggleExpandingWidget()
{
    QPushButton *toggleButton = qobject_cast<QPushButton *>(sender());
    if (toggleButton == downwidget_1) {
        departure->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold;");
        arrival->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold;");
        ui->onewaybtn->setEnabled(true);
        ui->roundtripbtn->setEnabled(true);
        departure->setEnabled(true);
        arrival->setEnabled(true);

        animation = new QPropertyAnimation(widgetContainer_1, "geometry");

        animation->setDuration(300);

        animation->setStartValue(QRect(0, 190, 360, 500));
        animation->setEndValue(QRect(0, 700, 360, 500));
        connect(animation, &QPropertyAnimation::finished, widgetContainer_1, &QWidget::hide);
        animation->start();

        QLayoutItem* item2;
        while ((item2 = widgetLayout_1->takeAt(0))) {
            if (QWidget* widget2 = item2->widget()) {
                widget2->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
            }
            delete item2;
        }
    } else if (toggleButton == departure || toggleButton == arrival){
        if(toggleButton == departure) {
            departure->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold; text-decoration: underline;");
        } else if (toggleButton == arrival) {
            arrival->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold; text-decoration: underline;");
        }
        ui->onewaybtn->setEnabled(false);
        ui->roundtripbtn->setEnabled(false);
        departure->setEnabled(false);
        arrival->setEnabled(false);

        QScrollArea *frame = new QScrollArea(widgetContainer_1);
        frame -> setFixedSize(360, 500);
        QVBoxLayout *vbl = new QVBoxLayout(frame);
        frame->setStyleSheet("QFrame {"
                             "background-color: rgb(204, 255, 244);"
                             "border-radius: 10px;"
                             "border: 1px solid rgb(85, 170, 255);"
                             "border-bottom: none;"
                             "}");

        downwidget_1 = new QPushButton;
        QLineEdit *search = new QLineEdit;
        search->setPlaceholderText("역 명을 입력해주세요.");

        QLabel *downarrow = new QLabel;
        QPixmap downarrowimg(":/resources/down-arrow_1.png");

        downarrow->setStyleSheet("background: transparent;");
        downarrow->setFixedSize(20, 20);
        downarrow->setPixmap(downarrowimg);
        downarrow->setScaledContents(true);

        downwidget_1->setFixedSize(350, 30);
        downwidget_1->setStyleSheet("background: transparent; border: none;");

        QHBoxLayout *centerlayout= new QHBoxLayout(downwidget_1);
        centerlayout->addWidget(downarrow);
        centerlayout->setAlignment(Qt::AlignCenter);

        vbl->addWidget(downwidget_1);
        vbl->addWidget(search);

        connect(search,  &QLineEdit::editingFinished, [=]() {
            if(station.contains(search->text())) {
                if(toggleButton==departure) {
                    station1 = search->text();
                } else if(toggleButton==arrival) {
                    station2 = search->text();
                }
                toggleButton->setText(search->text());
            }
            search->clear();
        });

        QLabel *stationtitle = new QLabel("주요역");
        stationtitle->setStyleSheet("font-size: 12px; color: rgb(116, 122, 232); border: none; border-bottom: 1px solid rgb(226, 226, 226); background: transparent;");

        vbl->addWidget(stationtitle);

        int cnt = 0;
        QHBoxLayout *hbl = new QHBoxLayout;
        for(QString &stationname : station) {
            QPushButton *button = new QPushButton("   " + stationname);
            button->setFixedSize(175, 30);
            button->setStyleSheet("background: transparent; border: none; border-bottom: 1px solid rgb(226, 226, 226); text-align: left;");
            hbl->addWidget(button);
            cnt++;
            if(cnt == 2) {
                button->setStyleSheet("background: transparent; border: none; border-bottom: 1px solid rgb(226, 226, 226); border-left: 1px solid rgb(226, 226, 226); text-align: left;");
                cnt = 0;
                vbl->addLayout(hbl);
                hbl = new QHBoxLayout;
            }
            connect(button, &QPushButton::clicked, [=]() {
                if(toggleButton==departure) {
                    station1 = stationname;
                } else if(toggleButton==arrival) {
                    station2 = stationname;
                }
                toggleButton->setText(stationname);
            });
        }
        if (cnt == 1) {
            hbl->addStretch();
            vbl->addLayout(hbl);
        }
        vbl->addStretch();

        widgetLayout_1->addStretch();
        widgetContainer_1->show();

        animation = new QPropertyAnimation(widgetContainer_1, "geometry");
        animation->setDuration(300);


        animation->setStartValue(QRect(0, 700, 360, 500));
        animation->setEndValue(QRect(0, 190, 360, 500));
        animation->start();

        connect(downwidget_1, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));

    } else if (toggleButton == toggleButton_2) {
        if (widgetContainer_2->isVisible()) {
            toggleButton_2->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_2.load(":/resources/down-arrow_1.png");
            arrow_2->setStyleSheet("background: transparent;");
            arrow_2->setFixedSize(20, 20);
            arrow_2->setPixmap(arrowimg_2);
            arrow_2->setScaledContents(true);

            radiobuttons_1.clear();
            dayIdx_1.clear();

            QLayoutItem* item;
            while ((item = widgetLayout_2->takeAt(0))) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                }
                delete item;
            }
            widgetContainer_2->hide();
        } else {
            toggleButton_2->setStyleSheet("QPushButton {"
                                         "border: none;"
                                         "background-color: rgb(204, 255, 244);"
                                         "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_2.load(":/resources/up-arrow_1.png");
            arrow_2->setStyleSheet("background: transparent;");
            arrow_2->setFixedSize(20, 20);
            arrow_2->setPixmap(arrowimg_2);
            arrow_2->setScaledContents(true);

            setDateButton_1();

            widgetContainer_2->show();

        }
    } else if (toggleButton == toggleButton_3) {
        if (widgetContainer_3->isVisible()) {
            toggleButton_3->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_3.load(":/resources/down-arrow_1.png");
            arrow_3->setStyleSheet("background: transparent;");
            arrow_3->setFixedSize(20, 20);
            arrow_3->setPixmap(arrowimg_3);
            arrow_3->setScaledContents(true);

            QLayoutItem* item;
            while ((item = widgetLayout_3->takeAt(0))) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                }
                delete item;
            }
            widgetContainer_3->hide();
        } else {
            toggleButton_3->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "background-color: rgb(204, 255, 244);"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_3.load(":/resources/up-arrow_1.png");
            arrow_3->setStyleSheet("background: transparent;");
            arrow_3->setFixedSize(20, 20);
            arrow_3->setPixmap(arrowimg_3);
            arrow_3->setScaledContents(true);

            setCustomNum_1();

            widgetContainer_3->show();
        }
    } else if (toggleButton == downwidget_2) {
        departure2->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold;");
        arrival2->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold;");

        ui->onewaybtn->setEnabled(true);
        ui->roundtripbtn->setEnabled(true);
        departure2->setEnabled(true);
        arrival2->setEnabled(true);

        animation = new QPropertyAnimation(widgetContainer_4, "geometry");
        animation->setDuration(300);

        animation->setStartValue(QRect(0, 190, 360, 500));
        animation->setEndValue(QRect(0, 700, 360, 500));
        connect(animation, &QPropertyAnimation::finished, widgetContainer_4, &QWidget::hide);
        animation->start();

        QLayoutItem* item2;
        while ((item2 = widgetLayout_4->takeAt(0))) {
            if (QWidget* widget2 = item2->widget()) {
                widget2->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
            }
            delete item2;
        }
    } else if (toggleButton == departure2 || toggleButton == arrival2) {
        if(toggleButton == departure2) {
            departure2->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold; text-decoration: underline;");
        } else if (toggleButton == arrival2) {
            arrival2->setStyleSheet("background: transparent; border: none; font-size: 25px; font-weight: bold; text-decoration: underline;");
        }

        ui->onewaybtn->setEnabled(false);
        ui->roundtripbtn->setEnabled(false);
        departure2->setEnabled(false);
        arrival2->setEnabled(false);

        QFrame *frame = new QFrame(widgetContainer_4);
        frame -> setFixedSize(360, 500);
        QVBoxLayout *vbl = new QVBoxLayout(frame);
        frame->setStyleSheet("QFrame {"
                             "background-color: rgb(204, 255, 244);"
                             "border-radius: 10px;"
                             "border: 1px solid rgb(85, 170, 255)"
                             "}");

        downwidget_2 = new QPushButton;
        QLineEdit *search = new QLineEdit;
        search->setPlaceholderText("역 명을 입력해주세요.");

        QLabel *downarrow = new QLabel;
        QPixmap downarrowimg(":/resources/down-arrow_1.png");

        downarrow->setStyleSheet("background: transparent;");
        downarrow->setFixedSize(20, 20);
        downarrow->setPixmap(downarrowimg);
        downarrow->setScaledContents(true);

        downwidget_2->setFixedSize(350, 30);
        downwidget_2->setStyleSheet("background: transparent; border: none");

        QHBoxLayout *centerlayout= new QHBoxLayout(downwidget_2);
        centerlayout->addWidget(downarrow);
        centerlayout->setAlignment(Qt::AlignCenter);

        vbl->addWidget(downwidget_2);
        vbl->addWidget(search);

        connect(search,  &QLineEdit::editingFinished, [=]() {
            if(station.contains(search->text())) {
                if(toggleButton==departure2) {
                    station1 = search->text();
                } else if(toggleButton==arrival2) {
                    station2 = search->text();
                }
                toggleButton->setText(search->text());
            }
            search->clear();
        });

        QLabel *stationtitle = new QLabel("주요역");
        stationtitle->setStyleSheet("font-size: 12px; color: rgb(116, 122, 232); border: none; border-bottom: 1px solid rgb(226, 226, 226); background: transparent;");

        vbl->addWidget(stationtitle);

        int cnt = 0;
        QHBoxLayout *hbl = new QHBoxLayout;
        for(QString &stationname : station) {
            QPushButton *button = new QPushButton("   " + stationname);
            button->setFixedSize(175, 30);
            button->setStyleSheet("background: transparent; border: none; border-bottom: 1px solid rgb(226, 226, 226); text-align: left;");
            hbl->addWidget(button);
            cnt++;
            if(cnt == 2) {
                button->setStyleSheet("background: transparent; border: none; border-bottom: 1px solid rgb(226, 226, 226); border-left: 1px solid rgb(226, 226, 226); text-align: left;");
                cnt = 0;
                vbl->addLayout(hbl);
                hbl = new QHBoxLayout;
            }
            connect(button, &QPushButton::clicked, [=]() {
                if(toggleButton==departure2) {
                    station1 = stationname;
                } else if(toggleButton==arrival2) {
                    station2 = stationname;
                }
                toggleButton->setText(stationname);
            });
        }
        if (cnt == 1) {
            hbl->addStretch();
            vbl->addLayout(hbl);
        }
        vbl->addStretch();

        widgetLayout_4->addStretch();
        widgetContainer_4->show();

        animation = new QPropertyAnimation(widgetContainer_4, "geometry");
        animation->setDuration(300);


        animation->setStartValue(QRect(0, 700, 360, 500));
        animation->setEndValue(QRect(0, 190, 360, 500));
        animation->start();

        connect(downwidget_2, SIGNAL(clicked()), this, SLOT(toggleExpandingWidget()));
    } else if (toggleButton == toggleButton_5) {
        if (widgetContainer_5->isVisible()) {
            toggleButton_5->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_5.load(":/resources/down-arrow_1.png");
            arrow_5->setStyleSheet("background: transparent;");
            arrow_5->setFixedSize(20, 20);
            arrow_5->setPixmap(arrowimg_5);
            arrow_5->setScaledContents(true);

            radiobuttons_2.clear();
            dayIdx_2.clear();

            QLayoutItem* item;
            while ((item = widgetLayout_5->takeAt(0))) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                }
                delete item;
            }
            widgetContainer_5->hide();
        } else {
            toggleButton_5->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "background-color: rgb(204, 255, 244);"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_5.load(":/resources/up-arrow_1.png");
            arrow_5->setStyleSheet("background: transparent;");
            arrow_5->setFixedSize(20, 20);
            arrow_5->setPixmap(arrowimg_5);
            arrow_5->setScaledContents(true);

            setDateButton_2();

            widgetContainer_5->show();
        }
    } else if (toggleButton == toggleButton_6) {
        if (widgetContainer_6->isVisible()) {
            toggleButton_6->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_6.load(":/resources/down-arrow_1.png");
            arrow_6->setStyleSheet("background: transparent;");
            arrow_6->setFixedSize(20, 20);
            arrow_6->setPixmap(arrowimg_6);
            arrow_6->setScaledContents(true);

            radiobuttons_3.clear();
            dayIdx_3.clear();

            QLayoutItem* item;
            while ((item = widgetLayout_6->takeAt(0))) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                }
                delete item;
            }
            widgetContainer_6->hide();
        } else {
            toggleButton_6->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "background-color: rgb(204, 255, 244);"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_6.load(":/resources/up-arrow_1.png");
            arrow_6->setStyleSheet("background: transparent;");
            arrow_6->setFixedSize(20, 20);
            arrow_6->setPixmap(arrowimg_6);
            arrow_6->setScaledContents(true);

            setDateButton_3();

            widgetContainer_6->show();
        }
    } else if (toggleButton == toggleButton_7) {
        if (widgetContainer_7->isVisible()) {
            toggleButton_7->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_7.load(":/resources/down-arrow_1.png");
            arrow_7->setStyleSheet("background: transparent;");
            arrow_7->setFixedSize(20, 20);
            arrow_7->setPixmap(arrowimg_7);
            arrow_7->setScaledContents(true);

            QLayoutItem* item;
            while ((item = widgetLayout_7->takeAt(0))) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater(); // 위젯을 삭제하고 메모리 누수 방지
                }
                delete item;
            }
            widgetContainer_7->hide();
        } else {
            toggleButton_7->setStyleSheet("QPushButton {"
                                          "border: none;"
                                          "background-color: rgb(204, 255, 244);"
                                          "border-top: 1px solid rgb(226, 226, 226);}");
            arrowimg_7.load(":/resources/up-arrow_1.png");
            arrow_7->setStyleSheet("background: transparent;");
            arrow_7->setFixedSize(20, 20);
            arrow_7->setPixmap(arrowimg_7);
            arrow_7->setScaledContents(true);

            setCustomNum_2();

            widgetContainer_7->show();
        }
    }
}

void Book::setCustomNum_1()
{
    QLayout* existingLayout = widgetContainer_3->layout();
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete existingLayout;
    }

    widgetLayout_3 = new QVBoxLayout(widgetContainer_3);
    QLabel *expandingWidget;
    QHBoxLayout *centerlayout = new QHBoxLayout;
    expandingWidget = new QLabel("최소 1명 ─ 최대 9명");
    expandingWidget->setStyleSheet("color: gray;");
    QFont font_3;
    font_3.setPointSize(10);
    expandingWidget->setFont(font_3);
    centerlayout->addWidget(expandingWidget);
    centerlayout->setAlignment(Qt::AlignCenter);
    widgetLayout_3->addLayout(centerlayout);

    QFrame *adultframe = new QFrame;
    QFrame *childframe = new QFrame;
    QFrame *elderframe = new QFrame;

    adultframe->setStyleSheet("background: transparent; border: none;");
    childframe->setStyleSheet("background: transparent; border: none;");
    elderframe->setStyleSheet("background: transparent; border: none;");

    QHBoxLayout *adultlayout = new QHBoxLayout(adultframe);
    QHBoxLayout *childlayout = new QHBoxLayout(childframe);
    QHBoxLayout *elderlayout = new QHBoxLayout(elderframe);

    QLabel *adulttitle = new QLabel("어른(만 13세 이상)");
    QLabel *childtitle = new QLabel("어린이(만 6-12세 이상)");
    QLabel *eldertitle = new QLabel("경로(만 65세 이상)");

    adulttitle->setFixedSize(190, 30);
    adulttitle->setStyleSheet("background: transparent; border: none; font-size: 15px;");
    childtitle->setFixedSize(190, 30);
    childtitle->setStyleSheet("background: transparent; border: none; font-size: 15px;");
    eldertitle->setFixedSize(190, 30);
    eldertitle->setStyleSheet("background: transparent; border: none; font-size: 15px;");

    QPushButton *adultminus = new QPushButton;
    QPushButton *childminus = new QPushButton;
    QPushButton *elderminus = new QPushButton;

    QVBoxLayout *adultvbl_1 = new QVBoxLayout(adultminus);
    QVBoxLayout *childvbl_1 = new QVBoxLayout(childminus);
    QVBoxLayout *eldervbl_1 = new QVBoxLayout(elderminus);

    adultminus->setFixedSize(30, 30);
    childminus->setFixedSize(30, 30);
    elderminus->setFixedSize(30, 30);

    QPixmap minusimg_1;
    QPixmap minusimg_2;
    QPixmap minusimg_3;

    if(adultnum<=0) {
        minusimg_1.load(":/resources/minus (1).png");
        adultminus->setEnabled(false);
    } else {
        minusimg_1.load(":/resources/minus.png");
    }
    if(childnum<=0) {
        minusimg_2.load(":/resources/minus (1).png");
        childminus->setEnabled(false);
    } else {
        minusimg_2.load(":/resources/minus.png");
    }
    if(eldernum<=0) {
        minusimg_3.load(":/resources/minus (1).png");
        elderminus->setEnabled(false);
    } else {
        minusimg_3.load(":/resources/minus.png");
    }

    QLabel *adultlabel_1 = new QLabel;
    QLabel *childlabel_1 = new QLabel;
    QLabel *elderlabel_1 = new QLabel;

    adultlabel_1->setStyleSheet("border: none; background: transparent;");
    adultlabel_1->setFixedSize(20, 20);
    adultlabel_1->setPixmap(minusimg_1);
    adultlabel_1->setScaledContents(true);

    childlabel_1->setStyleSheet("border: none; background: transparent;");
    childlabel_1->setFixedSize(20, 20);
    childlabel_1->setPixmap(minusimg_2);
    childlabel_1->setScaledContents(true);

    elderlabel_1->setStyleSheet("border: none; background: transparent;");
    elderlabel_1->setFixedSize(20, 20);
    elderlabel_1->setPixmap(minusimg_3);
    elderlabel_1->setScaledContents(true);

    adultvbl_1->addWidget(adultlabel_1);
    childvbl_1->addWidget(childlabel_1);
    eldervbl_1->addWidget(elderlabel_1);


    QLabel *adultnumber = new QLabel;
    QLabel *childnumber = new QLabel;
    QLabel *eldernumber = new QLabel;

    adultnumber->setAlignment(Qt::AlignCenter);
    childnumber->setAlignment(Qt::AlignCenter);
    eldernumber->setAlignment(Qt::AlignCenter);

    adultnumber->setFixedSize(30, 30);
    adultnumber->setText(QString::number(adultnum));
    adultnumber->setStyleSheet("background: transparent; border: none; font-size: 15px;");
    childnumber->setFixedSize(30, 30);
    childnumber->setText(QString::number(childnum));
    childnumber->setStyleSheet("background: transparent; border: none; font-size: 15px;");
    eldernumber->setFixedSize(30, 30);
    eldernumber->setText(QString::number(eldernum));
    eldernumber->setStyleSheet("background: transparent; border: none; font-size: 15px;");

    QPushButton *adultplus = new QPushButton;
    QPushButton *childplus = new QPushButton;
    QPushButton *elderplus = new QPushButton;

    QVBoxLayout *adultvbl_2 = new QVBoxLayout(adultplus);
    QVBoxLayout *childvbl_2 = new QVBoxLayout(childplus);
    QVBoxLayout *eldervbl_2 = new QVBoxLayout(elderplus);


    adultplus->setFixedSize(30, 30);
    childplus->setFixedSize(30, 30);
    elderplus->setFixedSize(30, 30);

    QPixmap plusimg;

    if(num>=9) {
        adultplus->setEnabled(false);
        childplus->setEnabled(false);
        elderplus->setEnabled(false);
        plusimg.load(":/resources/add (1).png");
    } else {
        plusimg.load(":/resources/add.png");
    }

    QLabel *adultlabel_2 = new QLabel;
    QLabel *childlabel_2 = new QLabel;
    QLabel *elderlabel_2 = new QLabel;

    adultlabel_2->setStyleSheet("border: none; background: transparent;");
    adultlabel_2->setFixedSize(20, 20);
    adultlabel_2->setPixmap(plusimg);
    adultlabel_2->setScaledContents(true);

    childlabel_2->setStyleSheet("border: none; background: transparent;");
    childlabel_2->setFixedSize(20, 20);
    childlabel_2->setPixmap(plusimg);
    childlabel_2->setScaledContents(true);

    elderlabel_2->setStyleSheet("border: none; background: transparent;");
    elderlabel_2->setFixedSize(20, 20);
    elderlabel_2->setPixmap(plusimg);
    elderlabel_2->setScaledContents(true);

    adultvbl_2->addWidget(adultlabel_2);
    childvbl_2->addWidget(childlabel_2);
    eldervbl_2->addWidget(elderlabel_2);

    adultlayout->addWidget(adulttitle);
    adultlayout->addWidget(adultminus);
    adultlayout->addWidget(adultnumber);
    adultlayout->addWidget(adultplus);
    adultlayout->addStretch();

    childlayout->addWidget(childtitle);
    childlayout->addWidget(childminus);
    childlayout->addWidget(childnumber);
    childlayout->addWidget(childplus);
    childlayout->addStretch();

    elderlayout->addWidget(eldertitle);
    elderlayout->addWidget(elderminus);
    elderlayout->addWidget(eldernumber);
    elderlayout->addWidget(elderplus);
    elderlayout->addStretch();

    widgetLayout_3->addWidget(adultframe);
    widgetLayout_3->addWidget(childframe);
    widgetLayout_3->addWidget(elderframe);

    widgetLayout_3->addStretch();

    // 마이너스 버튼
    connect(adultminus, &QPushButton::clicked, [=]() {
        adultnum--;
        num--;
        adultnumber->setText(QString::number(adultnum));
        customnum_1->setText(QString("총 %1명").arg(num));
        setCustomNum_1();
        setSearchTrainButton();
    });
    connect(childminus, &QPushButton::clicked, [=]() {
        childnum--;
        num--;
        childnumber->setText(QString::number(childnum));
        customnum_1->setText(QString("총 %1명").arg(num));
        setCustomNum_1();
        setSearchTrainButton();
    });
    connect(elderminus, &QPushButton::clicked, [=]() {
        eldernum--;
        num--;
        eldernumber->setText(QString::number(eldernum));
        customnum_1->setText(QString("총 %1명").arg(num));
        setCustomNum_1();
        setSearchTrainButton();
    });

    // 플러스 버튼
    connect(adultplus, &QPushButton::clicked, [=]() {
        adultnum++;
        num++;
        adultnumber->setText(QString::number(adultnum));
        customnum_1->setText(QString("총 %1명").arg(num));
        setCustomNum_1();
        setSearchTrainButton();
    });
    connect(childplus, &QPushButton::clicked, [=]() {
        childnum++;
        num++;
        childnumber->setText(QString::number(childnum));
        customnum_1->setText(QString("총 %1명").arg(num));
        setCustomNum_1();
        setSearchTrainButton();
    });
    connect(elderplus, &QPushButton::clicked, [=]() {
        eldernum++;
        num++;
        eldernumber->setText(QString::number(eldernum));
        customnum_1->setText(QString("총 %1명").arg(num));
        setCustomNum_1();
        setSearchTrainButton();
    });
}

void Book::setCustomNum_2()
{
    QLayout* existingLayout = widgetContainer_7->layout();
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete existingLayout;
    }

    widgetLayout_7 = new QVBoxLayout(widgetContainer_7);
    QLabel *expandingWidget;
    QHBoxLayout *centerlayout = new QHBoxLayout;
    expandingWidget = new QLabel("최소 1명 ─ 최대 9명");
    expandingWidget->setStyleSheet("color: gray;");
    QFont font_3;
    font_3.setPointSize(10);
    expandingWidget->setFont(font_3);
    centerlayout->addWidget(expandingWidget);
    centerlayout->setAlignment(Qt::AlignCenter);
    widgetLayout_7->addLayout(centerlayout);

    QFrame *adultframe = new QFrame;
    QFrame *childframe = new QFrame;
    QFrame *elderframe = new QFrame;

    adultframe->setStyleSheet("background: transparent; border: none;");
    childframe->setStyleSheet("background: transparent; border: none;");
    elderframe->setStyleSheet("background: transparent; border: none;");

    QHBoxLayout *adultlayout = new QHBoxLayout(adultframe);
    QHBoxLayout *childlayout = new QHBoxLayout(childframe);
    QHBoxLayout *elderlayout = new QHBoxLayout(elderframe);

    QLabel *adulttitle = new QLabel("어른(만 13세 이상)");
    QLabel *childtitle = new QLabel("어린이(만 6-12세 이상)");
    QLabel *eldertitle = new QLabel("경로(만 65세 이상)");

    adulttitle->setFixedSize(190, 30);
    adulttitle->setStyleSheet("background: transparent; border: none; font-size: 15px;");
    childtitle->setFixedSize(190, 30);
    childtitle->setStyleSheet("background: transparent; border: none; font-size: 15px;");
    eldertitle->setFixedSize(190, 30);
    eldertitle->setStyleSheet("background: transparent; border: none; font-size: 15px;");

    QPushButton *adultminus = new QPushButton;
    QPushButton *childminus = new QPushButton;
    QPushButton *elderminus = new QPushButton;

    QVBoxLayout *adultvbl_1 = new QVBoxLayout(adultminus);
    QVBoxLayout *childvbl_1 = new QVBoxLayout(childminus);
    QVBoxLayout *eldervbl_1 = new QVBoxLayout(elderminus);

    adultminus->setFixedSize(30, 30);
    childminus->setFixedSize(30, 30);
    elderminus->setFixedSize(30, 30);

    QPixmap minusimg_1;
    QPixmap minusimg_2;
    QPixmap minusimg_3;

    if(adultnum<=0) {
        minusimg_1.load(":/resources/minus (1).png");
        adultminus->setEnabled(false);
    } else {
        minusimg_1.load(":/resources/minus.png");
    }
    if(childnum<=0) {
        minusimg_2.load(":/resources/minus (1).png");
        childminus->setEnabled(false);
    } else {
        minusimg_2.load(":/resources/minus.png");
    }
    if(eldernum<=0) {
        minusimg_3.load(":/resources/minus (1).png");
        elderminus->setEnabled(false);
    } else {
        minusimg_3.load(":/resources/minus.png");
    }

    QLabel *adultlabel_1 = new QLabel;
    QLabel *childlabel_1 = new QLabel;
    QLabel *elderlabel_1 = new QLabel;

    adultlabel_1->setStyleSheet("border: none; background: transparent;");
    adultlabel_1->setFixedSize(20, 20);
    adultlabel_1->setPixmap(minusimg_1);
    adultlabel_1->setScaledContents(true);

    childlabel_1->setStyleSheet("border: none; background: transparent;");
    childlabel_1->setFixedSize(20, 20);
    childlabel_1->setPixmap(minusimg_2);
    childlabel_1->setScaledContents(true);

    elderlabel_1->setStyleSheet("border: none; background: transparent;");
    elderlabel_1->setFixedSize(20, 20);
    elderlabel_1->setPixmap(minusimg_3);
    elderlabel_1->setScaledContents(true);

    adultvbl_1->addWidget(adultlabel_1);
    childvbl_1->addWidget(childlabel_1);
    eldervbl_1->addWidget(elderlabel_1);

    QLabel *adultnumber = new QLabel;
    QLabel *childnumber = new QLabel;
    QLabel *eldernumber = new QLabel;

    adultnumber->setAlignment(Qt::AlignCenter);
    childnumber->setAlignment(Qt::AlignCenter);
    eldernumber->setAlignment(Qt::AlignCenter);

    adultnumber->setFixedSize(30, 30);
    adultnumber->setText(QString::number(adultnum));
    adultnumber->setStyleSheet("background: transparent; border: none; font-size: 15px;");
    childnumber->setFixedSize(30, 30);
    childnumber->setText(QString::number(childnum));
    childnumber->setStyleSheet("background: transparent; border: none; font-size: 15px;");
    eldernumber->setFixedSize(30, 30);
    eldernumber->setText(QString::number(eldernum));
    eldernumber->setStyleSheet("background: transparent; border: none; font-size: 15px;");

    QPushButton *adultplus = new QPushButton;
    QPushButton *childplus = new QPushButton;
    QPushButton *elderplus = new QPushButton;

    QVBoxLayout *adultvbl_2 = new QVBoxLayout(adultplus);
    QVBoxLayout *childvbl_2 = new QVBoxLayout(childplus);
    QVBoxLayout *eldervbl_2 = new QVBoxLayout(elderplus);

    adultplus->setFixedSize(30, 30);
    childplus->setFixedSize(30, 30);
    elderplus->setFixedSize(30, 30);

    QPixmap plusimg;

    if(num>=9) {
        adultplus->setEnabled(false);
        childplus->setEnabled(false);
        elderplus->setEnabled(false);
        plusimg.load(":/resources/add (1).png");
    } else {
        plusimg.load(":/resources/add.png");
    }

    QLabel *adultlabel_2 = new QLabel;
    QLabel *childlabel_2 = new QLabel;
    QLabel *elderlabel_2 = new QLabel;

    adultlabel_2->setStyleSheet("border: none; background: transparent;");
    adultlabel_2->setFixedSize(20, 20);
    adultlabel_2->setPixmap(plusimg);
    adultlabel_2->setScaledContents(true);

    childlabel_2->setStyleSheet("border: none; background: transparent;");
    childlabel_2->setFixedSize(20, 20);
    childlabel_2->setPixmap(plusimg);
    childlabel_2->setScaledContents(true);

    elderlabel_2->setStyleSheet("border: none; background: transparent;");
    elderlabel_2->setFixedSize(20, 20);
    elderlabel_2->setPixmap(plusimg);
    elderlabel_2->setScaledContents(true);

    adultvbl_2->addWidget(adultlabel_2);
    childvbl_2->addWidget(childlabel_2);
    eldervbl_2->addWidget(elderlabel_2);

    adultlayout->addWidget(adulttitle);
    adultlayout->addWidget(adultminus);
    adultlayout->addWidget(adultnumber);
    adultlayout->addWidget(adultplus);
    adultlayout->addStretch();

    childlayout->addWidget(childtitle);
    childlayout->addWidget(childminus);
    childlayout->addWidget(childnumber);
    childlayout->addWidget(childplus);
    childlayout->addStretch();

    elderlayout->addWidget(eldertitle);
    elderlayout->addWidget(elderminus);
    elderlayout->addWidget(eldernumber);
    elderlayout->addWidget(elderplus);
    elderlayout->addStretch();

    widgetLayout_7->addWidget(adultframe);
    widgetLayout_7->addWidget(childframe);
    widgetLayout_7->addWidget(elderframe);

    widgetLayout_7->addStretch();

    // 마이너스 버튼
    connect(adultminus, &QPushButton::clicked, [=]() {
        adultnum--;
        num--;
        adultnumber->setText(QString::number(adultnum));
        customnum_2->setText(QString("총 %1명").arg(num));
        setCustomNum_2();
        setSearchTrainButton();
    });
    connect(childminus, &QPushButton::clicked, [=]() {
        childnum--;
        num--;
        childnumber->setText(QString::number(childnum));
        customnum_2->setText(QString("총 %1명").arg(num));
        setCustomNum_2();
        setSearchTrainButton();
    });
    connect(elderminus, &QPushButton::clicked, [=]() {
        eldernum--;
        num--;
        eldernumber->setText(QString::number(eldernum));
        customnum_2->setText(QString("총 %1명").arg(num));
        setCustomNum_2();
        setSearchTrainButton();
    });

    // 플러스 버튼
    connect(adultplus, &QPushButton::clicked, [=]() {
        adultnum++;
        num++;
        adultnumber->setText(QString::number(adultnum));
        customnum_2->setText(QString("총 %1명").arg(num));
        setCustomNum_2();
        setSearchTrainButton();
    });
    connect(childplus, &QPushButton::clicked, [=]() {
        childnum++;
        num++;
        childnumber->setText(QString::number(childnum));
        customnum_2->setText(QString("총 %1명").arg(num));
        setCustomNum_2();
        setSearchTrainButton();
    });
    connect(elderplus, &QPushButton::clicked, [=]() {
        eldernum++;
        num++;
        eldernumber->setText(QString::number(eldernum));
        customnum_2->setText(QString("총 %1명").arg(num));
        setCustomNum_2();
        setSearchTrainButton();
    });
}

void Book::updateCalendar()
{
    QDate startOfMonth(currentDate_1.year(), currentDate_1.month(), 1);
    int startDayOfWeek = startOfMonth.dayOfWeek();

    startDayOfWeek = startDayOfWeek % 7 + 1;

    labelMonthYear->setText(currentDate_1.toString("yyyy년 MM월"));

    QDate buttonDate = startOfMonth.addDays(-startDayOfWeek + 1);

    QDate today = QDate::currentDate(); // 현재 날짜
    QDate endDate = today.addDays(31);

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            QPushButton *daybutton = calendarButtons[i][j];
            daybutton->setText(QString::number(buttonDate.day()));

            // 오늘 날짜와 31일 후 사이에 있고, 같은 달인 경우에만 버튼 활성화
            if (buttonDate >= today && buttonDate <= endDate && buttonDate.month() == currentDate_1.month()) {
                daybutton->setEnabled(true);
                if(j == 0) {
                    daybutton->setStyleSheet("border: none; color: red;");
                } else if(j == 6) {
                    daybutton->setStyleSheet("border: none; color: blue;");
                } else {
                    daybutton->setStyleSheet("border: none;");
                }
            } else {
                daybutton->setEnabled(false);
                if(j == 0) {
                    daybutton->setStyleSheet("border: none; color: rgb(255, 200, 200);");
                } else if(j == 6) {
                    daybutton->setStyleSheet("border: none; color: lightblue;");
                } else {
                    daybutton->setStyleSheet("border: none; color: gray;");
                }
            }

            buttonDate = buttonDate.addDays(1);
        }
    }
}


void Book::prevMonth()
{
    currentDate_1 = currentDate_1.addMonths(-1);
    updateCalendar();
    checkButtonEnable();
}

void Book::nextMonth()
{
    currentDate_1 = currentDate_1.addMonths(1);
    updateCalendar();
    checkButtonEnable();
}

void Book::setCalender(int chk)
{
    calendarButtons.clear();
    QWidget *currentWidget = QApplication::activeWindow(); // 현재 활성 창 얻기
    if (currentWidget) {
        int x = currentWidget->geometry().x() + 10; // x 좌표 설정
        int y = currentWidget->geometry().y() + 150; // y 좌표 설정
        newWidget = new QWidget;
        newWidget->setFixedSize(340, 400);

        layout = new QVBoxLayout(newWidget);
//        layout->setContentsMargins(0, 0, 0, 0);

        prevButton = new QPushButton("이전달");
        nextButton = new QPushButton("다음달");

        prevButton->setFixedSize(50, 30);
        nextButton->setFixedSize(50, 30);

        labelMonthYear = new QLabel;
        labelMonthYear->setAlignment(Qt::AlignCenter);

        QHBoxLayout *hbl = new QHBoxLayout;

        hbl->addWidget(prevButton);
        hbl->addWidget(labelMonthYear);
        hbl->addWidget(nextButton);
        layout->addLayout(hbl);

        QHBoxLayout *hbl_2 = new QHBoxLayout;
        hbl_2->setContentsMargins(0, 0, 0, 0);
        hbl_2->setSpacing(0);
        for(int i=0; i<7; i++) {
            QLabel *weekday = new QLabel;
            weekday->setFixedSize(48, 48);
            int idx=i-1;
            if(idx==-1) {
                idx = 6;
            }
            weekday->setText(dayofweek[idx]);
            weekday->setAlignment(Qt::AlignCenter);
            if(i == 0) {
                weekday->setStyleSheet("border: none; color: red;");
            } else if(i == 6) {
                weekday->setStyleSheet("border: none; color: blue;");
            } else {
                weekday->setStyleSheet("border: none;");
            }
            hbl_2->addWidget(weekday);
        }
        layout->addLayout(hbl_2);

        for (int i = 0; i < 6; i++) {
            QHBoxLayout *hbl_3 = new QHBoxLayout;
            hbl_3->setContentsMargins(0, 0, 0, 0);
            hbl_3->setSpacing(0);
            QVector<QPushButton *> temp;
            for (int j = 0; j < 7; j++) {
                QPushButton *daybutton = new QPushButton;
                daybutton->setFixedSize(48, 48);
                hbl_3->addWidget(daybutton);
                temp.append(daybutton);
                connect(daybutton, &QPushButton::clicked, [=]() {
                    QDateTime currentDateTime = QDateTime::currentDateTime();

                    // 클릭된 버튼에서 날짜 정보 얻기
                    int day = daybutton->text().toInt(); // 버튼의 일자 (1부터 31까지)
                    int month = currentDate_1.month();   // 현재 월
                    int year = currentDate_1.year();     // 현재 년도

                    // 현재 시간을 설정한 후 날짜를 QDateTime 형식으로 생성
                    QDateTime selectedDateTime = currentDateTime;
                    selectedDateTime.setDate(QDate(year, month, day));

                    // 요일을 인덱스로 가져오기
                    int dayOfWeekIndex = selectedDateTime.date().dayOfWeek()-1; // 0부터 시작하도록 조정

                    // 시간을 포함하여 원하는 형식으로 포맷
                    selectDate = selectedDateTime.toString("yyyy년 MM월 dd일 (" + dayofweek[dayOfWeekIndex] + ") hh:mm");

                    updateCalendar();
                    checkButtonEnable();
                    daybutton->setStyleSheet("border: none; color: white; background-color: rgb(139, 145, 255)");
                });
            }
            calendarButtons.append(temp);
            layout->addLayout(hbl_3);
        }

        QHBoxLayout *hbl_4 = new QHBoxLayout;
        hbl_4->setContentsMargins(0, 0, 0, 0);
        hbl_4->setSpacing(0);

        QPushButton *noButton = new QPushButton;
        QPushButton *okButton = new QPushButton;

        noButton->setFixedSize(160, 30);
        okButton->setFixedSize(160, 30);

        noButton->setText("취소");
        okButton->setText("확인");

        noButton->setStyleSheet("color: blue; background-color: rgb(139, 145, 255)");
        okButton->setStyleSheet("color: blue; background-color: rgb(139, 145, 255)");

        hbl_4->addWidget(noButton);
        hbl_4->addWidget(okButton);

        layout->addLayout(hbl_4);
        layout->addStretch();
        updateCalendar();
        checkButtonEnable();

        connect(prevButton, &QPushButton::clicked, this, &Book::prevMonth);
        connect(nextButton, &QPushButton::clicked, this, &Book::nextMonth);

        connect(noButton, &QPushButton::clicked, [=](){
            newWidget->close();
        });
        connect(okButton, &QPushButton::clicked, [=](){
            if(chk == 1) {
                datetime_1 = selectDate;
                date_1->setText(datetime_1);
            } else if (chk==2) {
                datetime_2 = selectDate;
                // 오는날과 비교
                QString goday = datetime_2.mid(10, 2);
                QString gomon = datetime_2.mid(6, 2);
                QString comeday = datetime_3.mid(10, 2);
                QString comemon = datetime_3.mid(6, 2);
                if(comeday.toInt() < goday.toInt() && comemon.toInt() <= gomon.toInt()) {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "이용안내", "이용하시는 날짜를 다시한번 확인하여<br>주시기 바랍니다.", QMessageBox::Ok);
                    if (reply == QMessageBox::Ok) {
                        datetime_3=datetime_2;
                        date_3->setText(datetime_3);
                    }
                }
                date_2->setText(datetime_2);
            } else if (chk==3) {
                datetime_3 = selectDate;
                // 가는날과 비교
                QString goday = datetime_2.mid(10, 2);
                QString gomon = datetime_2.mid(6, 2);
                QString comeday = datetime_3.mid(10, 2);
                QString comemon = datetime_3.mid(6, 2);
                if(comeday.toInt() <= goday.toInt() && comemon.toInt() <= gomon.toInt()) {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "이용안내", "이용하시는 날짜를 다시한번 확인하여<br>주시기 바랍니다.", QMessageBox::Ok);
                    if (reply == QMessageBox::Ok) {
                        datetime_2=datetime_3;
                        date_2->setText(datetime_2);
                    }
                }
                date_3->setText(datetime_3);
            }
            newWidget->close();
        });

        newWidget->move(x, y);
        newWidget->show();
    }
}

void Book::checkButtonEnable()
{
    QDate endOfMonth(currentDate_1.year(), currentDate_1.month(), currentDate_1.daysInMonth());
    QDate today = QDate::currentDate();
    QDate endDate = today.addDays(31);

    bool enablePrev = (currentDate_1.year() > today.year() || (currentDate_1.year() == today.year() && currentDate_1.month() > today.month()));
    bool enableNext = (endOfMonth <= endDate);

    // 이전 달 버튼 설정
    prevButton->setEnabled(enablePrev);
    if (enablePrev) {
        prevButton->setStyleSheet("QPushButton {"
                                  "background-color: rgb(255, 255, 255);"
                                  "color: rgb(116, 122, 232);"
                                  "border: 1px solid rgb(116, 122, 232);"
                                  "border-radius: 8px;}");  // 활성화된 버튼의 기본 스타일
    } else {
        prevButton->setStyleSheet("QPushButton {"
                                  "background-color: lightgray;"
                                  "color: gray;"
                                  "border: 1px solid gray;"
                                  "border-radius: 8px;}");  // 비활성화된 버튼의 스타일
    }

    // 다음 달 버튼 설정
    nextButton->setEnabled(enableNext);
    if (enableNext) {
        nextButton->setStyleSheet("QPushButton {"
                                  "background-color: rgb(255, 255, 255);"
                                  "color: rgb(116, 122, 232);"
                                  "border: 1px solid rgb(116, 122, 232);"
                                  "border-radius: 8px;}");  // 활성화된 버튼의 기본 스타일
    } else {
        nextButton->setStyleSheet("QPushButton {"
                                  "background-color: lightgray;"
                                  "color: gray;"
                                  "border: 1px solid gray;"
                                  "border-radius: 8px;}");  // 비활성화된 버튼의 스타일
    }
}


void Book::setDateButton_1()
{
    QLayout* existingLayout = widgetContainer_2->layout();
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete existingLayout;
        radiobuttons_1.clear();
        dayIdx_1.clear();
        radiobuttons_4.clear();
    }
    // 달력 보여주는 버튼 생성
    widgetLayout_2 = new QVBoxLayout(widgetContainer_2);
    QHBoxLayout *centerlayout = new QHBoxLayout;
    goCalendar_1 = new QPushButton("달력에서 날짜 선택");
    goCalendar_1->setFixedSize(300, 30);
    goCalendar_1->setStyleSheet("QPushButton {"
                          "background-color: rgb(255, 255, 255);"
                          "color: rgb(116, 122, 232);"
                          "border: 2px solid rgb(116, 122, 232);"
                          "border-radius: 8px;}");

    centerlayout->addWidget(goCalendar_1);
    centerlayout->setAlignment(Qt::AlignCenter);
    widgetLayout_2->addLayout(centerlayout);

    connect(goCalendar_1, &QPushButton::clicked, [=](){
        setCalender(1);
    });

    // 스크롤 영역에 30일 이후까지 날짜 시간 선택할 수 있도록 생성
    // 날짜 스크롤 설정
    QScrollArea *bookdate = new QScrollArea;
    QVBoxLayout *vbl = new QVBoxLayout;
    QHBoxLayout *bookdatelayout = new QHBoxLayout;
    QWidget *scrollWidget = new QWidget;

    bookdate->setFixedSize(320, 120);
    scrollWidget->setFixedSize(1540, 100);

    bookdate->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    bookdate->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    bookdate->setStyleSheet("border: none;");

    QDateTime currentdate = QDateTime::currentDateTime();
    int dayofweekidx = currentdate.date().dayOfWeek();

    QString lastday = datetime_1.mid(10, 2);
    QString lastmon = datetime_1.mid(6, 2);

    for(int i=0; i<31; i++) {
        QDateTime futuredatetime = QDateTime::currentDateTime().addDays(i);
        QString futureday = futuredatetime.toString("dd");
        QString futuremon = futuredatetime.toString("MM");

        QRadioButton *radiobutton = new QRadioButton;
        radiobutton->setCheckable(true);
        QVBoxLayout *buttonlayout = new QVBoxLayout(radiobutton);
        radiobutton->setFixedSize(50, 80);

        QLabel *scrolldayofweek = new QLabel;
        scrolldayofweek->setFixedSize(40, 40);
        if(futuredatetime == currentdate) {
            scrolldayofweek->setText("오늘");
        } else {
            scrolldayofweek->setText(dayofweek[dayofweekidx-1]);
        }
        buttonlayout->addWidget(scrolldayofweek);

        QLabel *daynum = new QLabel;
        QString futuredate;
        if (futureday == "01") {
            QString futuremonth = futuredatetime.toString("MM/");
            futuredate = "<div style='text-align: left; font-size: 10px;'>"+ futuremonth +"</div>"
                                                                                            "<div style='text-align: left; font-size: 14px;'>"+ futureday +"</div>";
        } else {
            futuredate = futureday;
        }
        daynum->setFixedSize(40, 40);
        daynum->setText(futuredate);
        buttonlayout->addWidget(daynum);
        bookdatelayout->addWidget(radiobutton);

        if(lastday == futureday && lastmon == futuremon) {
            radiobutton->setChecked(true);
        }

        if(radiobutton->isChecked()) {
            radiobutton->setStyleSheet("font-size: 14px; color: white; border: none; background: transparent; background-color: rgb(116, 122, 232); text-align: center;");
        } else if(!radiobutton->isChecked()) {
            if(futuredatetime == currentdate) {
                radiobutton->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
            } else {
                radiobutton->setStyleSheet("font-size: 14px; color: rgb(116, 122, 232); border: none; background: transparent; text-align: center;");
                if(dayofweekidx==6) {
                    radiobutton->setStyleSheet("font-size: 14px; color: blue; border: none; background: transparent; text-align: center;");
                } else if(dayofweekidx==7) {
                    radiobutton->setStyleSheet("font-size: 14px; color: red; border: none; background: transparent; text-align: center;");
                }
            }
        }

        radiobuttons_1.append(radiobutton);
        dayIdx_1.append(dayofweekidx);

        connect(radiobutton, &QRadioButton::clicked, [=]() {
            datetime_1 = currentdate.toString("yyyy년 " + futuremon + "월 " + futureday + "일 (" + dayofweek[dayofweekidx-1] + ") 00:00");
            date_1->setText(datetime_1);
            changeButtonStyle_1();
            if (radiobutton->isChecked()) {
                radiobutton->setStyleSheet("font-size: 14px; color: white; border: none; background: transparent; background-color: rgb(116, 122, 232); text-align: center;");
            }
            setDateButton_1();
        });

        dayofweekidx++;
        if(dayofweekidx==8) {
            dayofweekidx=1;
        }
    }
    vbl->addLayout(bookdatelayout);
    vbl->addStretch();
    scrollWidget->setLayout(vbl);
    bookdate->setWidget(scrollWidget);
    bookdate->setWidgetResizable(true);
    widgetLayout_2->addWidget(bookdate);
    showTimeScroll_1();
    widgetLayout_2->addWidget(booktime_1);
    widgetLayout_2->addStretch();
}

void Book::showTimeScroll_1()
{
    // 기존의 scrollWidget_1을 삭제
    QDateTime currentdate = QDateTime::currentDateTime();
    // 시간 스크롤 설정
    booktime_1 = new QScrollArea;
    QWidget *scrollWidget_1 = new QWidget;

    QVBoxLayout *vbl_2 = new QVBoxLayout;
    QHBoxLayout *booktimelayout = new QHBoxLayout;

    booktime_1->setFixedSize(320, 120);
    scrollWidget_1->setFixedSize(1240, 100);

    booktime_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    booktime_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    booktime_1->setStyleSheet("border: none; background-color: rgb(226, 226, 226);");

    QString nowday = currentdate.toString("dd");
    QString nowmon = currentdate.toString("MM");
    QString nowhour = currentdate.toString("hh");
    QString depday = datetime_1.mid(10, 2);
    QString depmon = datetime_1.mid(6, 2);
    QString dephour = datetime_1.mid(18, 2);
    QString depweek = datetime_1.mid(15, 1);

    for(int i=0; i<24; i++) {
        QString bookhour;
        if(i<10) {
            bookhour = "0" + QString::number(i);
        } else {
            bookhour = QString::number(i);
        }

        QRadioButton *radiobutton_2 = new QRadioButton;
        QVBoxLayout *buttonlayout_2 = new QVBoxLayout(radiobutton_2);
        radiobutton_2->setCheckable(true);
        radiobutton_2->setFixedSize(50, 80);

        QLabel *buttonhour = new QLabel;
        QString buttonhourstring = "<span style='font-size: 14px;'>" + bookhour + "</span>"
                                                                                  "<span style='font-size: 10px;'>시</span>";
        buttonhour->setFixedSize(50, 80);
        buttonhour->setText(buttonhourstring);
        buttonlayout_2->addWidget(buttonhour);
        booktimelayout->addWidget(radiobutton_2);

        if(nowday == depday && nowmon == depmon) {
            if(bookhour == dephour) {
                radiobutton_2->setChecked(true);
            }
        } else {
            if(bookhour == "00") {
                radiobutton_2->setChecked(true);
            }
        }

        if(radiobutton_2->isChecked()) {
            radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
        } else if(!radiobutton_2->isChecked()) {
            radiobutton_2->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
        }
        if(nowday == depday && nowmon == depmon) {
            if(nowhour.toInt() > bookhour.toInt()) {
                radiobutton_2->setCheckable(false);
            }
        }

        radiobuttons_4.append(radiobutton_2);

        connect(radiobutton_2, &QRadioButton::clicked, [=]() {
            if(nowday == depday && nowmon == depmon) {
                if(nowhour.toInt() >= bookhour.toInt()) {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "이용안내", "현재 시간 이전은 입력이 불가능합니다.", QMessageBox::Ok);
                    if (reply == QMessageBox::Ok) {
                        datetime_1 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") hh:mm");
                        date_1->setText(datetime_1);
                    }
                } else {
                    datetime_1 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") "+ bookhour +":00");
                    date_1->setText(datetime_1);
                    changehourButton_1();
                    if (radiobutton_2->isChecked()) {
                        radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
                    }
                    showTimeScroll_1();
                }
            } else {
                datetime_1 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") "+ bookhour +":00");
                date_1->setText(datetime_1);
                changehourButton_1();
                if (radiobutton_2->isChecked()) {
                    radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
                }
                showTimeScroll_1();
            }
        });
    }
    vbl_2->addLayout(booktimelayout);
    vbl_2->addStretch();
    scrollWidget_1->setLayout(vbl_2);
    booktime_1->setWidget(scrollWidget_1);
    booktime_1->setWidgetResizable(true);
}

void Book::setDateButton_2()
{
    QLayout* existingLayout = widgetContainer_5->layout();
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete existingLayout;
        radiobuttons_2.clear();
        dayIdx_2.clear();
        radiobuttons_5.clear();
    }
    // 달력 보여주는 버튼 생성
    widgetLayout_5 = new QVBoxLayout(widgetContainer_5);
    QHBoxLayout *centerlayout = new QHBoxLayout;
    goCalendar_2 = new QPushButton("달력에서 날짜 선택");
    goCalendar_2->setFixedSize(300, 30);
    goCalendar_2->setStyleSheet("QPushButton {"
                          "background-color: rgb(255, 255, 255);"
                          "color: rgb(116, 122, 232);"
                          "border: 2px solid rgb(116, 122, 232);"
                          "border-radius: 8px;}");

    centerlayout->addWidget(goCalendar_2);
    centerlayout->setAlignment(Qt::AlignCenter);
    widgetLayout_5->addLayout(centerlayout);

    connect(goCalendar_2, &QPushButton::clicked, [=](){
        setCalender(2);
    });

    // 스크롤 영역에 30일 이후까지 날짜 시간 선택할 수 있도록 생성
    // 날짜 스크롤 설정
    QScrollArea *bookdate = new QScrollArea;
    QVBoxLayout *vbl = new QVBoxLayout;
    QHBoxLayout *bookdatelayout = new QHBoxLayout;
    QWidget *scrollWidget = new QWidget;

    bookdate->setFixedSize(320, 120);
    scrollWidget->setFixedSize(1540, 100);

    bookdate->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    bookdate->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    bookdate->setStyleSheet("border: none;");

    QDateTime currentdate = QDateTime::currentDateTime();
    int dayofweekidx = currentdate.date().dayOfWeek();

    QString lastday = datetime_2.mid(10, 2);
    QString lastmon = datetime_2.mid(6, 2);

    for(int i=0; i<31; i++) {
        QDateTime futuredatetime = QDateTime::currentDateTime().addDays(i);
        QString futureday = futuredatetime.toString("dd");
        QString futuremon = futuredatetime.toString("MM");

        QRadioButton *radiobutton = new QRadioButton;
        radiobutton->setCheckable(true);
        QVBoxLayout *buttonlayout = new QVBoxLayout(radiobutton);
        radiobutton->setFixedSize(50, 80);

        QLabel *scrolldayofweek = new QLabel;
        scrolldayofweek->setFixedSize(40, 40);
        if(futuredatetime == currentdate) {
            scrolldayofweek->setText("오늘");
        } else {
            scrolldayofweek->setText(dayofweek[dayofweekidx-1]);
        }
        buttonlayout->addWidget(scrolldayofweek);

        QLabel *daynum = new QLabel;
        QString futuredate;
        if (futureday == "01") {
            QString futuremonth = futuredatetime.toString("MM/");
            futuredate = "<div style='text-align: left; font-size: 10px;'>"+ futuremonth +"</div>"
                                                                                            "<div style='text-align: left; font-size: 14px;'>"+ futureday +"</div>";
        } else {
            futuredate = futureday;
        }
        daynum->setFixedSize(40, 40);
        daynum->setText(futuredate);
        buttonlayout->addWidget(daynum);
        bookdatelayout->addWidget(radiobutton);

        if(lastday == futureday && lastmon == futuremon) {
            radiobutton->setChecked(true);
        }

        if(radiobutton->isChecked()) {
            radiobutton->setStyleSheet("font-size: 14px; color: white; border: none; background: transparent; background-color: rgb(116, 122, 232); text-align: center;");
        } else if(!radiobutton->isChecked()) {
            if(futuredatetime == currentdate) {
                radiobutton->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
            } else {
                radiobutton->setStyleSheet("font-size: 14px; color: rgb(116, 122, 232); border: none; background: transparent; text-align: center;");
                if(dayofweekidx==6) {
                    radiobutton->setStyleSheet("font-size: 14px; color: blue; border: none; background: transparent; text-align: center;");
                } else if(dayofweekidx==7) {
                    radiobutton->setStyleSheet("font-size: 14px; color: red; border: none; background: transparent; text-align: center;");
                }
            }
        }

        radiobuttons_2.append(radiobutton);
        dayIdx_2.append(dayofweekidx);

        connect(radiobutton, &QRadioButton::clicked, [=]() {
            datetime_2 = currentdate.toString("yyyy년 " + futuremon + "월 " + futureday + "일 (" + dayofweek[dayofweekidx-1] + ") hh:mm");
            // 오는날과 비교
            QString goday = datetime_2.mid(10, 2);
            QString gomon = datetime_2.mid(6, 2);
            QString comeday = datetime_3.mid(10, 2);
            QString comemon = datetime_3.mid(6, 2);
            if(comeday.toInt() < goday.toInt() && comemon.toInt() <= gomon.toInt()) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::warning(this, "이용안내", "이용하시는 날짜를 다시한번 확인하여<br>주시기 바랍니다.", QMessageBox::Ok);
                if (reply == QMessageBox::Ok) {
                    datetime_3=datetime_2;
                    date_3->setText(datetime_3);
                }
            }
            date_2->setText(datetime_2);
            changeButtonStyle_2();
            if (radiobutton->isChecked()) {
                radiobutton->setStyleSheet("font-size: 14px; color: white; border: none; background: transparent; background-color: rgb(116, 122, 232); text-align: center;");
            }
            setDateButton_2();
        });

        dayofweekidx++;
        if(dayofweekidx==8) {
            dayofweekidx=1;
        }
    }
    vbl->addLayout(bookdatelayout);
    vbl->addStretch();
    scrollWidget->setLayout(vbl);
    bookdate->setWidget(scrollWidget);
    bookdate->setWidgetResizable(true);
    widgetLayout_5->addWidget(bookdate);
    showTimeScroll_2();
    widgetLayout_5->addWidget(booktime_2);
    widgetLayout_5->addStretch();
}

void Book::showTimeScroll_2()
{
    // 기존의 scrollWidget_1을 삭제
    QDateTime currentdate = QDateTime::currentDateTime();
    // 시간 스크롤 설정
    booktime_2 = new QScrollArea;
    QWidget *scrollWidget_2 = new QWidget;

    QVBoxLayout *vbl_2 = new QVBoxLayout;
    QHBoxLayout *booktimelayout = new QHBoxLayout;

    booktime_2->setFixedSize(320, 120);
    scrollWidget_2->setFixedSize(1240, 100);

    booktime_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    booktime_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    booktime_2->setStyleSheet("border: none; background-color: rgb(226, 226, 226);");

    QString nowday = currentdate.toString("dd");
    QString nowmon = currentdate.toString("MM");
    QString nowhour = currentdate.toString("hh");
    QString depday = datetime_2.mid(10, 2);
    QString depmon = datetime_2.mid(6, 2);
    QString dephour = datetime_2.mid(18, 2);
    QString depweek = datetime_2.mid(15, 1);

    QString arvday = datetime_3.mid(10, 2);
    QString arvmon = datetime_3.mid(6, 2);
    QString arvhour = datetime_3.mid(18, 2);

    for(int i=0; i<24; i++) {
        QString bookhour;
        if(i<10) {
            bookhour = "0" + QString::number(i);
        } else {
            bookhour = QString::number(i);
        }

        QRadioButton *radiobutton_2 = new QRadioButton;
        QVBoxLayout *buttonlayout_2 = new QVBoxLayout(radiobutton_2);
        radiobutton_2->setCheckable(true);
        radiobutton_2->setFixedSize(50, 80);

        QLabel *buttonhour = new QLabel;
        QString buttonhourstring = "<span style='font-size: 14px;'>" + bookhour + "</span>"
                                                                                  "<span style='font-size: 10px;'>시</span>";
        buttonhour->setFixedSize(50, 80);
        buttonhour->setText(buttonhourstring);
        buttonlayout_2->addWidget(buttonhour);
        booktimelayout->addWidget(radiobutton_2);

        if(nowday == depday && nowmon == depmon) {
            if(bookhour == dephour) {
                radiobutton_2->setChecked(true);
            }
        } else {
            if(bookhour == "00") {
                radiobutton_2->setChecked(true);
            }
        }

        if(radiobutton_2->isChecked()) {
            radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
        } else if(!radiobutton_2->isChecked()) {
            radiobutton_2->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
        }
        if(nowday == depday && nowmon == depmon) {
            if(nowhour.toInt() > bookhour.toInt()) {
                radiobutton_2->setCheckable(false);
            }
        }

        radiobuttons_5.append(radiobutton_2);

        connect(radiobutton_2, &QRadioButton::clicked, [=]() {
            if(nowday == depday && nowmon == depmon) {
                if(nowhour.toInt() >= bookhour.toInt()) {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "이용안내", "현재 시간 이전은 입력이 불가능합니다.", QMessageBox::Ok);
                    if (reply == QMessageBox::Ok) {
                        datetime_2 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") hh:mm");
                        date_2->setText(datetime_2);
                    }
                } else {
                    datetime_2 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") "+ bookhour +":00");
                    date_2->setText(datetime_2);
                    changehourButton_2();
                    if (radiobutton_2->isChecked()) {
                        radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
                    }
                    showTimeScroll_2();
                }
            } else if(arvday == depday && arvmon == depmon){
                if(bookhour >= arvhour){
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "이용안내", "이용하시는 날짜를 다시한번 확인하여<br>주시기 바랍니다.", QMessageBox::Ok);
                    if (reply == QMessageBox::Ok) {
                        datetime_2 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") " + bookhour + ":00");
                        date_2->setText(datetime_2);
                        datetime_3 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") " + bookhour + ":00");
                        date_3->setText(datetime_3);
                    }
                } else {
                    datetime_2 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") " + bookhour + ":00");
                    date_2->setText(datetime_2);
                }
                changehourButton_2();
                if (radiobutton_2->isChecked()) {
                    radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
                }
                showTimeScroll_2();
            } else {
                datetime_2 = currentdate.toString("yyyy년 " + depmon + "월 " + depday + "일 (" + depweek + ") "+ bookhour +":00");
                date_2->setText(datetime_2);
                changehourButton_2();
                if (radiobutton_2->isChecked()) {
                    radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
                }
                showTimeScroll_2();
            }
        });
    }
    vbl_2->addLayout(booktimelayout);
    vbl_2->addStretch();
    scrollWidget_2->setLayout(vbl_2);
    booktime_2->setWidget(scrollWidget_2);
    booktime_2->setWidgetResizable(true);
}

void Book::setDateButton_3()
{
    QLayout* existingLayout = widgetContainer_6->layout();
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete existingLayout;
        radiobuttons_3.clear();
        dayIdx_3.clear();
        radiobuttons_6.clear();
    }
    // 달력 보여주는 버튼 생성
    widgetLayout_6 = new QVBoxLayout(widgetContainer_6);
    QHBoxLayout *centerlayout = new QHBoxLayout;

    goCalendar_3 = new QPushButton("달력에서 날짜 선택");
    goCalendar_3->setFixedSize(300, 30);
    goCalendar_3->setStyleSheet("QPushButton {"
                          "background-color: rgb(255, 255, 255);"
                          "color: rgb(116, 122, 232);"
                          "border: 2px solid rgb(116, 122, 232);"
                          "border-radius: 8px;}");

    centerlayout->addWidget(goCalendar_3);
    centerlayout->setAlignment(Qt::AlignCenter);
    widgetLayout_6->addLayout(centerlayout);

    connect(goCalendar_3, &QPushButton::clicked, [=](){
        setCalender(3);
    });

    // 스크롤 영역에 30일 이후까지 날짜 시간 선택할 수 있도록 생성
    // 날짜 스크롤 설정
    QScrollArea *bookdate = new QScrollArea;
    QVBoxLayout *vbl = new QVBoxLayout;
    QHBoxLayout *bookdatelayout = new QHBoxLayout;
    QWidget *scrollWidget = new QWidget;

    bookdate->setFixedSize(320, 120);
    scrollWidget->setFixedSize(1540, 100);

    bookdate->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    bookdate->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    bookdate->setStyleSheet("border: none;");

    QDateTime currentdate = QDateTime::currentDateTime();
    int dayofweekidx = currentdate.date().dayOfWeek();

    QString lastday = datetime_3.mid(10, 2);
    QString lastmon = datetime_3.mid(6, 2);

    for(int i=0; i<31; i++) {
        QDateTime futuredatetime = QDateTime::currentDateTime().addDays(i);
        QString futureday = futuredatetime.toString("dd");
        QString futuremon = futuredatetime.toString("MM");

        QRadioButton *radiobutton = new QRadioButton;
        radiobutton->setCheckable(true);
        QVBoxLayout *buttonlayout = new QVBoxLayout(radiobutton);
        radiobutton->setFixedSize(50, 80);

        QLabel *scrolldayofweek = new QLabel;
        scrolldayofweek->setFixedSize(40, 40);
        if(futuredatetime == currentdate) {
            scrolldayofweek->setText("오늘");
        } else {
            scrolldayofweek->setText(dayofweek[dayofweekidx-1]);
        }
        buttonlayout->addWidget(scrolldayofweek);

        QLabel *daynum = new QLabel;
        QString futuredate;
        if (futureday == "01") {
            QString futuremonth = futuredatetime.toString("MM/");
            futuredate = "<div style='text-align: left; font-size: 10px;'>"+ futuremonth +"</div>"
                                                                                            "<div style='text-align: left; font-size: 14px;'>"+ futureday +"</div>";
        } else {
            futuredate = futureday;
        }
        daynum->setFixedSize(40, 40);
        daynum->setText(futuredate);
        buttonlayout->addWidget(daynum);
        bookdatelayout->addWidget(radiobutton);

        if(lastday == futureday && lastmon == futuremon) {
            radiobutton->setChecked(true);
        }

        if(radiobutton->isChecked()) {
            radiobutton->setStyleSheet("font-size: 14px; color: white; border: none; background: transparent; background-color: rgb(116, 122, 232); text-align: center;");
        } else if(!radiobutton->isChecked()) {
            if(futuredatetime == currentdate) {
                radiobutton->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
            } else {
                radiobutton->setStyleSheet("font-size: 14px; color: rgb(116, 122, 232); border: none; background: transparent; text-align: center;");
                if(dayofweekidx==6) {
                    radiobutton->setStyleSheet("font-size: 14px; color: blue; border: none; background: transparent; text-align: center;");
                } else if(dayofweekidx==7) {
                    radiobutton->setStyleSheet("font-size: 14px; color: red; border: none; background: transparent; text-align: center;");
                }
            }
        }

        radiobuttons_3.append(radiobutton);
        dayIdx_3.append(dayofweekidx);

        connect(radiobutton, &QRadioButton::clicked, [=]() {
            datetime_3 = currentdate.toString("yyyy년 " + futuremon + "월 " + futureday + "일 (" + dayofweek[dayofweekidx-1] + ") hh:mm");
            // 가는날과 비교
            QString goday = datetime_2.mid(10, 2);
            QString gomon = datetime_2.mid(6, 2);
            QString comeday = datetime_3.mid(10, 2);
            QString comemon = datetime_3.mid(6, 2);
            if(comeday.toInt() <= goday.toInt() && comemon.toInt() <= gomon.toInt()) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::warning(this, "이용안내", "이용하시는 날짜를 다시한번 확인하여<br>주시기 바랍니다.", QMessageBox::Ok);
                if (reply == QMessageBox::Ok) {
                    datetime_2=datetime_3;
                    date_2->setText(datetime_2);
                }
            }
            date_3->setText(datetime_3);
            changeButtonStyle_3();
            if (radiobutton->isChecked()) {
                radiobutton->setStyleSheet("font-size: 14px; color: white; border: none; background: transparent; background-color: rgb(116, 122, 232); text-align: center;");
            }
            setDateButton_3();
        });

        dayofweekidx++;
        if(dayofweekidx==8) {
            dayofweekidx=1;
        }
    }
    vbl->addLayout(bookdatelayout);
    vbl->addStretch();
    scrollWidget->setLayout(vbl);
    bookdate->setWidget(scrollWidget);
    bookdate->setWidgetResizable(true);
    widgetLayout_6->addWidget(bookdate);
    showTimeScroll_3();
    widgetLayout_6->addWidget(booktime_3);
    widgetLayout_6->addStretch();
}

void Book::showTimeScroll_3()
{
    // 기존의 scrollWidget_1을 삭제
    QDateTime currentdate = QDateTime::currentDateTime();
    // 시간 스크롤 설정
    booktime_3 = new QScrollArea;
    QWidget *scrollWidget_3 = new QWidget;

    ///////////////////////////////안드로이드에서 터치로 스크롤 움직이게 하는 코드//////////////////////////////////////
//    #include <QScroller>

//    // ...

//    QScrollArea *scrollArea = new QScrollArea(this);
//    QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


    QVBoxLayout *vbl_2 = new QVBoxLayout;
    QHBoxLayout *booktimelayout = new QHBoxLayout;

    booktime_3->setFixedSize(320, 120);
    scrollWidget_3->setFixedSize(1240, 100);

    booktime_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    booktime_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    booktime_3->setStyleSheet("border: none; background-color: rgb(226, 226, 226);");

    QString nowday = currentdate.toString("dd");
    QString nowmon = currentdate.toString("MM");
    QString nowhour = currentdate.toString("hh");
    QString depday = datetime_2.mid(10, 2);
    QString depmon = datetime_2.mid(6, 2);
    QString dephour = datetime_2.mid(18, 2);

    QString arvday = datetime_3.mid(10, 2);
    QString arvmon = datetime_3.mid(6, 2);
    QString arvhour = datetime_3.mid(18, 2);
    QString arvweek = datetime_3.mid(15, 1);

    for(int i=0; i<24; i++) {
        QString bookhour;
        if(i<10) {
            bookhour = "0" + QString::number(i);
        } else {
            bookhour = QString::number(i);
        }

        QRadioButton *radiobutton_2 = new QRadioButton;
        QVBoxLayout *buttonlayout_2 = new QVBoxLayout(radiobutton_2);
        radiobutton_2->setCheckable(true);
        radiobutton_2->setFixedSize(50, 80);

        QLabel *buttonhour = new QLabel;
        QString buttonhourstring = "<span style='font-size: 14px;'>" + bookhour + "</span>"
                                                                                  "<span style='font-size: 10px;'>시</span>";
        buttonhour->setFixedSize(50, 80);
        buttonhour->setText(buttonhourstring);
        buttonlayout_2->addWidget(buttonhour);
        booktimelayout->addWidget(radiobutton_2);

        if(nowday == arvday && nowmon == arvmon) {
            if(bookhour == arvhour) {
                radiobutton_2->setChecked(true);
            }
        } else {
            if(bookhour == "00") {
                radiobutton_2->setChecked(true);
            }
        }

        if(radiobutton_2->isChecked()) {
            radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
        } else if(!radiobutton_2->isChecked()) {
            radiobutton_2->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
        }
        if(nowday == arvday && nowmon == arvmon) {
            if(nowhour.toInt() > bookhour.toInt()) {
                radiobutton_2->setCheckable(false);
            }
        }

        radiobuttons_6.append(radiobutton_2);

        connect(radiobutton_2, &QRadioButton::clicked, [=]() {
            if(nowday == arvday && nowmon == arvmon) {
                if(nowhour.toInt() >= bookhour.toInt()) {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "이용안내", "현재 시간 이전은 입력이 불가능합니다.", QMessageBox::Ok);
                    if (reply == QMessageBox::Ok) {
                        datetime_3 = currentdate.toString("yyyy년 " + arvmon + "월 " + arvday + "일 (" + arvweek + ") hh:mm");
                        date_3->setText(datetime_3);
                    }
                } else {
                    datetime_3 = currentdate.toString("yyyy년 " + arvmon + "월 " + arvday + "일 (" + arvweek + ") "+ bookhour +":00");
                    date_3->setText(datetime_3);
                    changehourButton_3();
                    if (radiobutton_2->isChecked()) {
                        radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
                    }
                    showTimeScroll_3();
                }
            } else if(arvday == depday && arvmon == depmon){
                if(dephour >= bookhour){
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "이용안내", "이용하시는 날짜를 다시한번 확인하여<br>주시기 바랍니다.", QMessageBox::Ok);
                    if (reply == QMessageBox::Ok) {
                        datetime_2 = currentdate.toString("yyyy년 " + arvmon + "월 " + arvday + "일 (" + arvweek + ") " + bookhour + ":00");
                        date_2->setText(datetime_2);
                        datetime_3 = currentdate.toString("yyyy년 " + arvmon + "월 " + arvday + "일 (" + arvweek + ") " + bookhour + ":00");
                        date_3->setText(datetime_3);
                    }
                } else {
                    datetime_3 = currentdate.toString("yyyy년 " + arvmon + "월 " + arvday + "일 (" + arvweek + ") " + bookhour + ":00");
                    date_3->setText(datetime_3);
                }
                changehourButton_3();
                if (radiobutton_2->isChecked()) {
                    radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
                }
                showTimeScroll_3();
            } else {
                datetime_3 = currentdate.toString("yyyy년 " + arvmon + "월 " + arvday + "일 (" + arvweek + ") "+ bookhour +":00");
                date_3->setText(datetime_3);
                changehourButton_3();
                if (radiobutton_2->isChecked()) {
                    radiobutton_2->setStyleSheet("font-size: 14px; color: white; border: none; background-color: rgb(116, 122, 232); text-align: center;");
                }
                showTimeScroll_3();
            }
        });
    }
    vbl_2->addLayout(booktimelayout);
    vbl_2->addStretch();
    scrollWidget_3->setLayout(vbl_2);
    booktime_3->setWidget(scrollWidget_3);
    booktime_3->setWidgetResizable(true);
}

void Book::changehourButton_1()
{
    QRadioButton *button = nullptr;
    for (int i=0; i<radiobuttons_4.size(); i++) {
        button = radiobuttons_4[i];
        button->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
    }
}

void Book::changehourButton_2()
{
    QRadioButton *button = nullptr;
    for (int i=0; i<radiobuttons_5.size(); i++) {
        button = radiobuttons_5[i];
        button->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
    }
}

void Book::changehourButton_3()
{
    QRadioButton *button = nullptr;
    for (int i=0; i<radiobuttons_6.size(); i++) {
        button = radiobuttons_6[i];
        button->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
    }
}

void Book::changeButtonStyle_1()
{
    QRadioButton *button = nullptr;
    for (int i=0; i<radiobuttons_1.size(); i++) {
        if (i==0) {
            button = radiobuttons_1[0];
            button->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
        } else {
            if (dayIdx_1[i] == 6) {
                button = radiobuttons_1[i];
                button->setStyleSheet("font-size: 14px; color: blue; border: none; background: transparent; text-align: center;");
            } else if (dayIdx_1[i] == 7) {
                button = radiobuttons_1[i];
                button->setStyleSheet("font-size: 14px; color: red; border: none; background: transparent; text-align: center;");
            } else {
                button = radiobuttons_1[i];
                button->setStyleSheet("font-size: 14px; color: rgb(116, 122, 232); border: none; background: transparent; text-align: center;");
            }
        }
    }
}

void Book::changeButtonStyle_2()
{
    QRadioButton *button = nullptr;
    for (int i=0; i<radiobuttons_2.size(); i++) {
        if (i==0) {
            button = radiobuttons_2[0];
            button->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
        } else {
            if (dayIdx_2[i] == 6) {
                button = radiobuttons_2[i];
                button->setStyleSheet("font-size: 14px; color: blue; border: none; background: transparent; text-align: center;");
            } else if (dayIdx_2[i] == 7) {
                button = radiobuttons_2[i];
                button->setStyleSheet("font-size: 14px; color: red; border: none; background: transparent; text-align: center;");
            } else {
                button = radiobuttons_2[i];
                button->setStyleSheet("font-size: 14px; color: rgb(116, 122, 232); border: none; background: transparent; text-align: center;");
            }
        }
    }
}

void Book::changeButtonStyle_3()
{
    QRadioButton *button = nullptr;
    for (int i=0; i<radiobuttons_3.size(); i++) {
        if (i==0) {
            button = radiobuttons_3[0];
            button->setStyleSheet("font-size: 14px; border: none; background: transparent; text-align: center;");
        } else {
            if (dayIdx_3[i] == 6) {
                button = radiobuttons_3[i];
                button->setStyleSheet("font-size: 14px; color: blue; border: none; background: transparent; text-align: center;");
            } else if (dayIdx_3[i] == 7) {
                button = radiobuttons_3[i];
                button->setStyleSheet("font-size: 14px; color: red; border: none; background: transparent; text-align: center;");
            } else {
                button = radiobuttons_3[i];
                button->setStyleSheet("font-size: 14px; color: rgb(116, 122, 232); border: none; background: transparent; text-align: center;");
            }
        }
    }
}

bool Book::eventFilter(QObject *obj, QEvent *event)
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

QHostAddress Book::getMyIP()
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

void Book::getsignal(const QString &id, const QString &password, const QString &name, const QString &dep, const QString &arv)
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

    if(!id.isEmpty() && !password.isEmpty() && !name.isEmpty()) {
        ID = id;
        PW = password;
        NAME = name;
    }

    if(!dep.isEmpty() && !arv.isEmpty()) {
        station1 = dep;
        station2 = arv;
        departure->setText(station1);
        arrival->setText(station2);
    }

    this->show();
}

void Book::getTcpSocket(QTcpSocket *tcpsocket)
{
    tcpSocket = tcpsocket;
}

void Book::showmap()
{
    map_1 = new map(this);
    map_1->getway(ui->stackedWidget->currentIndex());
    map_1->getuserinfo(ID, PW, NAME, HP, EM);
    map_1->setWidgetInstance(parentWidget);
    QFrame *bookframe = findChild<QFrame *>("Book");
    if(bookframe) {
        bookframe->hide();
    }
    connect(this, SIGNAL(gomap()), map_1, SLOT(getsignal()));

    emit gomap();
    disconnect(this, SIGNAL(gomap()), map_1, SLOT(getsignal()));
}

void Book::showtrainlist()
{
    if(station1 == station2){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "이용안내", "출발지와 도착지를 다시한번 확인하여<br>주시기 바랍니다.", QMessageBox::Ok);
        if (reply == QMessageBox::Ok) {

        }
    } else {
        QString tripway = QString::number(ui->stackedWidget->currentIndex()); // 0이면 편도, 1이면 왕복
        QString adult = QString::number(adultnum);
        QString child = QString::number(childnum);
        QString elder = QString::number(eldernum);
        QString message;
        if(tripway ==  "0") {
            // 편도
            // 출발일
            QString depyear = datetime_1.mid(0, 4);
            QString depmon = datetime_1.mid(6, 2);
            QString depday = datetime_1.mid(10, 2); // idx = 5
            QString dephour = datetime_1.mid(18, 2);
            QString depweek = datetime_1.mid(15, 1);
            QString deptime = depyear + "," + depmon + "," + depday + "," + dephour + "," + depweek;

            message = "search: " + tripway + "," + station1 + "," + station2 + "," + deptime + "," + adult + "," + child + "," + elder;
        } else if(tripway == "1") {
            // 왕복
            // 가는날
            QString depyear_1 = datetime_2.mid(0, 4);
            QString depmon_1 = datetime_2.mid(6, 2);
            QString depday_1 = datetime_2.mid(10, 2); //idx = 5
            QString dephour_1 = datetime_2.mid(18, 2);
            QString depweek_1 = datetime_2.mid(15, 1);
            QString deptime_1 = depyear_1 + "," + depmon_1 + "," + depday_1 + "," + dephour_1 + "," + depweek_1;
            // 오는날
            QString depyear_2 = datetime_3.mid(0, 4);
            QString depmon_2 = datetime_3.mid(6, 2);
            QString depday_2 = datetime_3.mid(10, 2); // idx = 10
            QString dephour_2 = datetime_3.mid(18, 2);
            QString depweek_2 = datetime_3.mid(15, 1);
            QString deptime_2 = depyear_2 + "," + depmon_2 + "," + depday_2 + "," + dephour_2 + "," + depweek_2;

            message = "search: " + tripway + "," + station1 + "," + station2 + "," + deptime_1 + "," + deptime_2 + "," + adult + "," + child + "," + elder;
        }
        // 서버에 정보 전달
        tcpSocket->write(message.toUtf8());

        // trainlist로 이동
        connect(this, SIGNAL(gotrainlist(QString)), parentWidget, SLOT(showtrainlist(QString)));
        QFrame *bookFrame = findChild<QFrame *>("Book");
        if (bookFrame) {
            bookFrame->hide();
        }
        emit gotrainlist(message);
        disconnect(this, SIGNAL(gotrainlist(QString)), parentWidget, SLOT(showtrainlist(QString)));
    }
}

void Book::getstations(const QString &dep, const QString &arv, const int &idx)
{
    stackedidx= idx;
    mapdeparture = dep;
    maparrival = arv;
    station1 = mapdeparture;
    station2 = maparrival;
    if(stackedidx == 0) {
        departure->setText(station1);
        arrival->setText(station2);
    } else if(stackedidx == 1) {
        departure2->setText(station1);
        arrival2->setText(station2);
    }
    map_1->hide();
    this->show();
}

void Book::showscreen()
{
    connect(this, &Book::sendInfo, parentWidget, &Widget::getuserinfo);
    emit sendInfo(ID, PW, station1, station2);
    disconnect(this, &Book::sendInfo, parentWidget, &Widget::getuserinfo);
    this->hide();
    QString message = "out: ";
    tcpSocket->write(message.toUtf8());
}

void Book::showlogin()
{
    connect(this, &Book::logout, parentWidget, &Widget::backlogin);
    QFrame *bookFrame = findChild<QFrame *>("Book");
    if (bookFrame) {
        bookFrame->hide();
    }
    emit logout();
    QString message = "logout: ";
    tcpSocket->write(message.toUtf8());
    disconnect(this, &Book::logout, parentWidget, &Widget::backlogin);
}

Book::~Book()
{
    delete ui;
    delete mainLayout_1;
}
