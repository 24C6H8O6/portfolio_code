#include "login.h"
#include "ui_login.h"
#include "ui_join.h"
#include "ui_find.h"
#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include "screen.h"
#include "join.h"
#include "find.h"
#include "widget.h"

// ex2. 위젯 생성자 실행
Login::Login(QWidget *parent) :
    QWidget(parent), ui(new Ui::Login)
{
    ui->setupUi(this);

    // ex2.1 Connect, Socket Signal, class Slot 설정
    //       버튼을 클릭하면, connectButton() 함수를 실행한다.

    connect(ui->exit, SIGNAL(clicked()),
            this,     SLOT(disconnectButton()));
    connect(ui->user_login, SIGNAL(clicked()),
            this,     SLOT(sendButton()));
    connect(ui->input_id, SIGNAL(returnPressed()),
            this,     SLOT(sendButton()));
    connect(ui->input_pw, SIGNAL(returnPressed()),
            this,     SLOT(sendButton()));
    connect(ui->joining, SIGNAL(clicked()),
            this,     SLOT(showjoin()));
    connect(ui->find_id, SIGNAL(clicked()),
            this,     SLOT(showfind()));
    // ex3. 통신을 위한 initialize() 실행

    initialize();
}

void Login::initialize()
{
    tcpSocket = new QTcpSocket(this);
}

// ex7. 시그널 ex4.2 이벤트가 발생하면, 소캣 연결을 종료 정보를 표시한다.

QHostAddress Login::getMyIP()
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

void Login::sendMessage(const QString &message)
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        // 메시지 전송
        QByteArray messageData = message.toUtf8();
        tcpSocket->write(messageData);
        tcpSocket->flush();
    }else {
        ui->textEdit->append("서버에 연결되지 않았습니다.");
    }
}

void Login::sendButton()
{
    QString message = "login: ";
    QString id = ui->input_id->text();
    QString pw = ui->input_pw->text();
    message = message + id + "," + pw;
    sendMessage(message);
    ui->input_pw->clear();
}

void Login::showjoin()
{
    connect(this, SIGNAL(gojoin()), parentWidget, SLOT(showjoin()));
    QFrame *loginFrame = findChild<QFrame *>("Login");
    if (loginFrame) {
        loginFrame->hide();
    }
    emit gojoin();
}

void Login::showfind()
{
    connect(this, SIGNAL(gofind()), parentWidget, SLOT(showfind()));
    QFrame *loginFrame = findChild<QFrame *>("Login");
    if (loginFrame) {
        loginFrame->hide();
    }
    emit gofind();
}

void Login::getsignal(const QString &id, const QString &password, const QString &dep, const QString &arv)
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
    ID = id;
    PW = password;
    departure = dep;
    arrival = arv;

    ui->input_id->setText(ID);
    ui->input_pw->setText(PW);
    if(!ID.isEmpty() && !PW.isEmpty()) {
        sendButton();
    } else {
        this->show();
    }
}

void Login::getTcpSocket(QTcpSocket *tcpsocket)
{
    tcpSocket = tcpsocket;
}

void Login::disconnectButton()
{
    qDebug() << Q_FUNC_INFO << "서버로부터 접속 종료.";
    screenWidget = new Screen(this);
    tcpSocket->disconnectFromHost();
    QFrame *loginFrame = findChild<QFrame *>("Login");
    if (loginFrame) {
        loginFrame->hide();
    }
    screenWidget->show();
}

Login::~Login()
{
    delete ui;
}
