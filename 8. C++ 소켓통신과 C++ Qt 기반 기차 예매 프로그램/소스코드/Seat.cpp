#include "Seat.h"
#include "ui_Seat.h"
#include <QGraphicsOpacityEffect>
#include <QButtonGroup>
#include <QComboBox>
#include <QHostAddress>
#include <QtNetwork>
#include <QMouseEvent>
#include "widget.h"

Seat::Seat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Seat)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    ui->selectbutton->setVisible(false);
    ui->selectnum->move(0, 50);

    //테스트
//    QVector<QString> reservedSeat1 = {"15C","1A","2D"};
//    QVector<QString> reservedSeat2 = {"1C","1B","2D", "6C", "15A"};
//    QVector<QString> reservedSeat3 = {"15C","1A","2D"};
//    QVector<QString> reservedSeat4 = {"15C","1A","2D"};
//    QVector<QString> reservedSeat5 = {"15C","1A","2D"};
//    QVector<QString> reservedSeat6 = {"15C","1A","2D"};
//    reservedSeats.append(reservedSeat1);
//    reservedSeats.append(reservedSeat2);
//    reservedSeats.append(reservedSeat3);
//    reservedSeats.append(reservedSeat4);
//    reservedSeats.append(reservedSeat5);
//    reservedSeats.append(reservedSeat6);



    // 좌석 텍스트 세팅
    for(QAbstractButton *button : ui->seatButtonGroup->buttons()) {
        QPushButton *seatButton = qobject_cast<QPushButton *>(button);
        if(seatButton) {
            QString seatText = seatButton->objectName().remove("seatbutton_");
            QLabel *seatLabel = findChild<QLabel *>(QString("seat_%1").arg(seatText));
            if (seatLabel) {
                seatLabel->setText(seatText);
                seatLabel->setStyleSheet("font-size: 11pt; font: bold;");
                seatLabel->setAlignment(Qt::AlignCenter);
            }
        }
    }

    setReservedSeat(0);

    // 호차 선택, 상단 텍스트 변경 연결

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(getcomboboxtext(int)));
    connect(ui->seatButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(changeButtonStyle(QAbstractButton*)));
    connect(ui->selectbutton, &QPushButton::clicked, this, &Seat::reserveComplete);
    connect(ui->backbtn, &QPushButton::clicked, this, &Seat::hide);

//    qDebug() << reservedSeats;
    tcpSocket = new QTcpSocket(this);

}

void Seat::setReservedSeat(int index)
{
    // 호차마다 Label 초기화
    for(QAbstractButton *button : ui->seatButtonGroup->buttons()) {
        QPushButton *seatButton = qobject_cast<QPushButton *>(button);
        if(seatButton) {
            seatButton->setDisabled(false);
            QString seatText = seatButton->objectName().remove("seatbutton_");
            QLabel *seatLabel = findChild<QLabel *>(QString("seat_%1").arg(seatText));
            if (seatLabel) {
                seatLabel->setText(seatText);
                seatLabel->setStyleSheet("font-size: 11pt; font: bold;");
                seatLabel->setAlignment(Qt::AlignCenter);
            } else {
                qWarning() << "Label for seat" << seatText << "not found!";
            }
        }
    }

    // vector reservedSeat 예약된 좌석 배경 변경 및 버튼 비활성화
    if (index<reservedSeats.size()) {
        for (int i = 0; i < reservedSeats[index].size(); i++) {
            QString ReservSeatLabel = "seat_" + reservedSeats[index][i];
            QString ReservSeatButton = "seatbutton_"+ reservedSeats[index][i];

            QPushButton *seatPushButton = findChild<QPushButton *>(ReservSeatButton);
            if (seatPushButton) {
                seatPushButton->setDisabled(true);
            }

            QLabel *seatLabel = findChild<QLabel *>(ReservSeatLabel);
            if (seatLabel) {
                seatLabel->setStyleSheet("background-color: rgb(200, 200, 200); color: gray; font: bold; font-size: 11pt;");
            }
        }
    }
}

// 콤보박스에서 선택된 아이템 가져와서 label에 입력
void Seat::getcomboboxtext(int index) {
    if (index != -1) {
        QString selectcombobox = ui->comboBox->itemText(index);
        ui->label->setText(selectcombobox);
        setReservedSeat(index);
        num = chk;
        ui->label_4->setText("남은 인원 : " + QString::number(num) + "명" );
        ui->selectnum->clear();
        ui->selectnum->setText("선택 좌석");
        ui->selectbutton->setStyleSheet("background-color: rgb(136, 138, 133);");
        ui->selectbutton->setVisible(false);
        ui->selectnum->move(0, 50);
    }
}

//버튼 바꾸기
void Seat::changeButtonStyle(QAbstractButton *button) {
    qDebug()<< num << ":seat";
    if (button) {
        QString seatName = button->objectName();
        seatName.remove("button");
        QLabel *seatLabel = findChild<QLabel *>(seatName);
        // 버튼 취소
        ui->selectbutton->setStyleSheet("background-color: rgb(136, 138, 133);");

        if (seatLabel && num >= 0) {
            ui->selectbutton->setEnabled(false);
            if (seatLabel->styleSheet().contains("background-color: rgb(116,122,232);")) {
                seatLabel->setStyleSheet("background-color: rgb(255,255,255); font: bold;");
                num++; //취소하면 인원 수 다시 증가
                ui->label_4->setText("남은 인원 : " + QString::number(num) + "명" );

                QString text = ui->selectnum->text();
                text.remove("선택 좌석<br>");
                QStringList textlist = text.split(",");
                seatName.remove("seat_"); //seat_10B -> 10B만 남게하기
                seatName = ui->label->text() + " " + seatName;

                QString labelText = ui->selectnum->text();
                labelText.remove("선택 좌석<br>");
                QStringList labelTextlist = labelText.split(",");

                for (int i = 0; i < textlist.size(); i++) {
                    QString part = textlist.at(i);
                    if(part == seatName){
                        labelTextlist.removeAll(part);
                        labelText = labelTextlist.join(",");
                        if (num != chk) {
                            ui->selectnum->setText("선택 좌석<br>" + labelText);
                        } else {
                            ui->selectnum->setText("선택 좌석");
                        }
                        if(num==chk){
                            ui->selectbutton->setVisible(false);
                            ui->selectnum->move(0, 50);
                        }
                    }
                }
            }
            // 버튼 클릭
            else if(num>0) {
                ui->selectbutton->setEnabled(false);
                ui->selectbutton->setVisible(true);
                ui->selectnum->move(0, 0);
                seatLabel->setStyleSheet("background-color: rgb(116,122,232); color: white; font: bold; font-size: 11pt;");

                QString pretext = ui->selectnum->text(); //이전 텍스트
                QString selectSeats;
                if(num != chk) {
                    selectSeats = "," + ui->label->text() +" " + seatLabel->objectName(); //좌석이름
                } else {
                    selectSeats = "<br>" + ui->label->text() +" " + seatLabel->objectName();
                }
                selectSeats.remove("seat_");
                pretext = pretext+selectSeats;
                ui->selectnum->setText(pretext);
                num--;

                ui->label_4->setText("남은 인원 : " + QString::number(num) + "명" );
                 if(num == 0){
                    ui->selectbutton->setStyleSheet("background-color: rgb(243, 147, 235);");
                    ui->selectbutton->setEnabled(true);
                 }
                 if(num==chk){
                    ui->selectbutton->setVisible(false);
                    ui->selectnum->move(0, 50);
                 }
            }
        }
    }
}

void Seat::reserveComplete()
{
    QString complete = ui->selectnum->text();
    complete.remove("선택 좌석<br>");
    QStringList seats = complete.split(",");
    QStringList cars = seats[0].split(" ");
    QString car = cars[0];
    complete.remove(car+" ");
    QString seatdata = car+ "," + complete;
    qDebug() << seatdata;

    hide();
}

void Seat::getuserinfo(QString id, QString password, QString name, QString phonenum, QString email)
{
    ID = id;
    PW = password;
    NAME = name;
    HP = phonenum;
    EM = email;
}

void Seat::showmyinfo()
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

        connect(logoutbutton, &QPushButton::clicked, this, &Seat::showlogin);

        animation = new QPropertyAnimation(widgetContainer_8, "geometry");
        animation->setDuration(300);


        animation->setStartValue(QRect(360, 0, 330, 700));
        animation->setEndValue(QRect(30, 0, 330, 700));
        animation->start();
    }
}

bool Seat::eventFilter(QObject *obj, QEvent *event)
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

void Seat::showlogin()
{
    connect(this, &Seat::logout, parentWidget, &Widget::backlogin);
    QFrame *SeatFrame = findChild<QFrame *>("Seat");
    if (SeatFrame) {
        SeatFrame->hide();
    }
    emit logout();
    QString message = "logout: ";
    tcpSocket->write(message.toUtf8());
    disconnect(this, &Seat::logout, parentWidget, &Widget::backlogin);
}

void Seat::getsignal(const int &custom)
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
    num = custom;
    chk = num;
    ui->label_4->setText("남은 인원 : " + QString::number(num) + "명" );
    this->show();
}

void Seat::getTcpSocket(QTcpSocket *tcpsocket)
{
    tcpSocket = tcpsocket;
}

QHostAddress Seat::getMyIP()
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

Seat::~Seat()
{
    delete ui;
}
