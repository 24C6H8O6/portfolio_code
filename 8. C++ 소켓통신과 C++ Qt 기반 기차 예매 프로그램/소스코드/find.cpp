#include "find.h"
#include "ui_find.h"
#include <QTimer>
#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include "login.h"
#include "widget.h"

Find::Find(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Find)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->em_dir->setEnabled(false);
    ui->input_id->setEnabled(false);
    ui->input_id_2->setEnabled(false);
    ui->label_33->setVisible(false);
    ui->label_34->setVisible(false);

    connect(ui->em_sel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(emailselect(int)));

    // 입력한 정보 유효성 검사
    connect(ui->input_id, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_id_2, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_hp1, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_hp2, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_email, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->em_dir, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));

    // 다음화면
    connect(ui->find_id, SIGNAL(clicked()),
            this,     SLOT(changeButton()));
    connect(ui->find_pw, SIGNAL(clicked()),
            this,     SLOT(changeButton()));
    connect(ui->findbyhp, SIGNAL(clicked()),
            this,     SLOT(changeButton()));
    connect(ui->findbyem, SIGNAL(clicked()),
            this,     SLOT(changeButton()));
    // 뒤로가기
    connect(ui->back_1, SIGNAL(clicked()),
            this,     SLOT(showlogin()));
    connect(ui->back_5, SIGNAL(clicked()),
            this,     SLOT(showlogin()));
    connect(ui->back_6, SIGNAL(clicked()),
            this,     SLOT(showlogin()));

    connect(ui->back_2, SIGNAL(clicked()),
            this,     SLOT(changeButton()));
    connect(ui->back_3, SIGNAL(clicked()),
            this,     SLOT(changeButton()));
    connect(ui->back_4, SIGNAL(clicked()),
            this,     SLOT(changeButton()));
    // 비번찾기로 넘어가기
    connect(ui->find_pw_2, SIGNAL(clicked()),
            this,     SLOT(changeButton()));

    // 찾기 버튼 눌렀을 때
    connect(ui->find_info1, SIGNAL(clicked()),
            this,     SLOT(sendButton()));
    connect(ui->find_info2, SIGNAL(clicked()),
            this,     SLOT(sendButton()));

    initialize();
}

void Find::initialize()
{
    tcpSocket = new QTcpSocket(this);
}

QHostAddress Find::getMyIP()
{
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            myAddress = ipAddressesList.at(i);
            break;
        }
    }

    if (myAddress.toString().isEmpty())
        myAddress = QHostAddress(QHostAddress::LocalHost);

    return myAddress;
}

void Find::sendMessage(const QString &message)
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        // 메시지 전송
        QByteArray messageData = message.toUtf8();
        tcpSocket->write(messageData);
    }
}

void Find::sendButton()
{
    QPushButton *findbtn = qobject_cast<QPushButton*>(sender());

    if(findbtn == ui->find_info1) {
        QString message;
        QString id;
        QString hp1chk = ui->input_hp1->text();
        QString hp2chk = ui->input_hp2->text();
        QString hp = "010" + ui->input_hp1->text() + ui->input_hp2->text();
        if(ui->find_title_2->text().startsWith("아이디")) {
            message = "아이디HP: " + hp;
            if (!hp1chk.isEmpty() && !hp2chk.isEmpty()) {
                sendMessage(message);
            } else {
                if (hp1chk.isEmpty() || hp2chk.isEmpty()) {
                    ui->textEdit_3->setText("HP를 입력해주세요");
                }
            }
        } else{
            id = ui->input_id->text();
            message = "비번HP: " + id + "," + hp;
            if (!id.isEmpty() && !hp1chk.isEmpty() && !hp2chk.isEmpty()) {
                sendMessage(message);
            } else {
                if (id.isEmpty()){
                    ui->textEdit_3->setText("ID를 입력해주세요");
                } else if (hp1chk.isEmpty() || hp2chk.isEmpty()) {
                    ui->textEdit_3->setText("HP를 입력해주세요");
                }
            }
        }
    } else {
        QString message;
        QString id;
        QString emchk1 = ui->input_email->text();
        QString emchk2;
        QString em;
        if (ui->em_sel->currentIndex() == ui->em_sel->count() - 1) {
            // "직접입력" 옵션이 선택된 경우
            em = ui->input_email->text() + "@" + ui->em_dir->text();
            emchk2 = ui->em_dir->text();
        } else {
            // 다른 옵션이 선택된 경우
            em = ui->input_email->text() + "@" + ui->em_sel->currentText();
            emchk2 = ui->em_sel->currentText();
        }
        if(ui->find_title_3->text().startsWith("아이디")){
            message = "아이디EM: " + em;
            if (!emchk1.isEmpty() && !emchk2.isEmpty()) {
                sendMessage(message);
            } else {
                if (emchk1.isEmpty() || emchk2.isEmpty()) {
                    ui->textEdit_4->setText("Email를 입력해주세요");
                }
            }
        } else{
            id = ui->input_id_2->text();
            message = "비번EM: " + id + "," + em;
            if (!id.isEmpty() && !emchk1.isEmpty() && !emchk2.isEmpty()) {
                sendMessage(message);
            } else {
                if (id.isEmpty()){
                    ui->textEdit_4->setText("ID를 입력해주세요");
                } else if (emchk1.isEmpty() || emchk2.isEmpty()) {
                    ui->textEdit_4->setText("Email를 입력해주세요");
                }
            }
        }
    }
}

void Find::changeButton()
{
    QPushButton *changebtn = qobject_cast<QPushButton*>(sender());
    qDebug() << qobject_cast<QPushButton*>(sender());
    if(changebtn == ui->back_2) {
        ui->stackedWidget->setCurrentIndex(0);
    } else if(changebtn == ui->back_3) {
        ui->stackedWidget->setCurrentIndex(1);
    } else if(changebtn == ui->back_4) {
        ui->stackedWidget->setCurrentIndex(1);
    } else if(changebtn == ui->find_pw_2) {
        ui->stackedWidget->setCurrentIndex(1);
        setPwfind();
    } else if(changebtn == ui->find_id) {
        setIDfind();
        ui->stackedWidget->setCurrentIndex(1);
    } else if(changebtn == ui->find_pw) {
        setPwfind();
        ui->stackedWidget->setCurrentIndex(1);
    } else if(changebtn == ui->findbyhp) {
        ui->input_id->clear();
        ui->input_hp1->clear();
        ui->input_hp2->clear();
        ui->stackedWidget->setCurrentIndex(2);
    } else if(changebtn == ui->findbyem) {
        ui->input_id_2->clear();
        ui->input_email->clear();
        ui->em_dir->clear();
        ui->em_sel->currentIndexChanged(0);
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void Find::setIDfind()
{
    ui->find_title_1->setText("아이디 찾기");
    ui->find_title_2->setText("아이디 찾기");
    ui->find_title_3->setText("아이디 찾기");
    ui->input_id->setEnabled(false);
    ui->input_id_2->setEnabled(false);
    ui->label_33->setVisible(false);
    ui->label_34->setVisible(false);
    ui->input_id->setStyleSheet("background-color: rgb(255, 194, 34); border: none;");
    ui->input_id_2->setStyleSheet("background-color: rgb(255, 194, 34); border: none;");
}

void Find::setPwfind()
{
    ui->find_title_1->setText("비밀번호 찾기");
    ui->find_title_2->setText("비밀번호 찾기");
    ui->find_title_3->setText("비밀번호 찾기");
    ui->input_id->setEnabled(true);
    ui->input_id_2->setEnabled(true);
    ui->label_33->setVisible(true);
    ui->label_34->setVisible(true);
    ui->input_id->setStyleSheet("color: rgb(0, 0, 0);"
                                "background-color: rgb(255, 255, 255);"
                                "border-radius: 7px;");
    ui->input_id_2->setStyleSheet("color: rgb(0, 0, 0);"
                                  "background-color: rgb(255, 255, 255);"
                                  "border-radius: 7px;");
}

void Find::validchk()
{
    QLineEdit *inputinfo = qobject_cast<QLineEdit*>(sender());
    if (inputinfo == ui->input_id) {
        QString id = ui->input_id->text();
        if(id.length() < 6 || !IDchk(id)) {
            ui->input_id->setText("다시 입력해주세요");
            QTimer::singleShot(1000, [this]() {
                ui->input_id->clear();
            });
        }
    } else if (inputinfo == ui->input_id_2) {
        QString id = ui->input_id_2->text();
        if(id.length() < 6 || !IDchk(id)) {
            ui->input_id_2->setText("다시 입력해주세요");
            QTimer::singleShot(1000, [this]() {
                ui->input_id_2->clear();
            });
        }
    } else if (inputinfo == ui->input_hp1) {
        QString hp1 = ui->input_hp1->text();
        if(hp1.length()!=4 || !containsOnlyDigits(hp1)) {
            ui->input_hp1->clear();
        }
    } else if (inputinfo == ui->input_hp2) {
        QString hp2 = ui->input_hp2->text();
        if(hp2.length()!=4 || !containsOnlyDigits(hp2)) {
            ui->input_hp2->clear();
        }
    } else if (inputinfo == ui->input_email) {
        QString em1 = ui->input_email->text();
        if(!Specialchk(em1) || !IDchk(em1)) {
            ui->input_email->clear();
        }
    } else if (inputinfo == ui->em_dir) {
        QString em = ui->em_dir->text().trimmed();
        if((!em.endsWith(".com") && !em.endsWith(".net")) || em.length() < 5) {
            ui->em_dir->clear();
        }
    }
}

bool Find::IDchk(const QString &str)
{
    for (const QChar &ch : str) {
        ushort unicode = ch.unicode();
        if ((unicode >= 'A' && unicode <= 'Z') ||
            (unicode >= 'a' && unicode <= 'z') ||
            (unicode >= '0' && unicode <= '9')) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

QString Find::emailselect(int index)
{
    QString email;
    if (index == ui->em_sel->count() - 1) {
        ui->em_dir->setEnabled(true);
        ui->em_dir->setStyleSheet("background-color: white");
        email = ui->em_dir->text();
    } else {
        ui->em_dir->setEnabled(false);
        ui->em_dir->setStyleSheet("background-color: transparent");
        email = ui->em_sel->currentText();
        ui->em_dir->clear();
    }
    return email;
}

bool Find::containsOnlyDigits(const QString &str)
{
    for (const QChar &ch : str) {
        if (!ch.isDigit()) {
            return false;
        }
    }
    return true;
}

bool Find::Specialchk(const QString &email)
{
    for (const QChar &ch : email) {
        if (!ch.isLetterOrNumber()) {
            return false; // 특수 문자가 있다면 false 반환
        }
    }
    return true; // 특수 문자가 없다면 true 반환
}

void Find::getsignal()
{
    qDebug() << tcpSocket;
    qDebug() << tcpSocket->isOpen() << ": join";

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

void Find::getTcpSocket(QTcpSocket *tcpsocket)
{
    tcpSocket = tcpsocket;
}

void Find::showlogin()
{
    connect(this, SIGNAL(gologin()), parentWidget, SLOT(showlogin()));
    QFrame *findFrame = findChild<QFrame *>("Find");
    if (findFrame) {
        findFrame->hide();
    }
    emit gologin();
}

Find::~Find()
{
    delete ui;
}
