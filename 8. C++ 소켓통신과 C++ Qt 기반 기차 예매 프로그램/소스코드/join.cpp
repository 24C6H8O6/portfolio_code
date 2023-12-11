#include "join.h"
#include "ui_join.h"
#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include "login.h"
#include "widget.h"

Join::Join(QWidget *parent) :
    QWidget(parent), ui(new Ui::Join)
{
    ui->setupUi(this);
    ui->em_dir->setEnabled(false);

    setTabOrder(ui->input_id, ui->join_idchk);
    setTabOrder(ui->join_idchk, ui->input_name);
    setTabOrder(ui->input_name, ui->input_nick);
    setTabOrder(ui->input_nick, ui->input_pw);
    setTabOrder(ui->input_pw, ui->input_pw2);
    setTabOrder(ui->input_pw2, ui->input_hp1);
    setTabOrder(ui->input_hp1, ui->input_hp2);
    setTabOrder(ui->input_hp2, ui->join_hpchk);
    setTabOrder(ui->join_hpchk, ui->input_email);
    setTabOrder(ui->input_email, ui->em_sel);
    setTabOrder(ui->em_dir, ui->join_emchk);
    setTabOrder(ui->join_emchk, ui->join_okay);
    setTabOrder(ui->join_okay, ui->join_exit);

    // 유효성 검사
    connect(ui->input_id, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_name, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_nick, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_pw, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_pw2, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_hp1, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_hp2, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->input_email, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->em_dir, SIGNAL(editingFinished()),
            this,     SLOT(validchk()));
    connect(ui->em_sel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(emailselect(int)));

    // 다시 입력하려고 했을때 중복 확인 문구 초기화 + 유효성 문구 초기화
    connect(ui->input_id, SIGNAL(textChanged(QString)),
            this,     SLOT(Reset()));
    connect(ui->input_hp1, SIGNAL(textChanged(QString)),
            this,     SLOT(Reset()));
    connect(ui->input_hp2, SIGNAL(textChanged(QString)),
            this,     SLOT(Reset()));
    connect(ui->input_email, SIGNAL(textChanged(QString)),
            this,     SLOT(Reset()));
    connect(ui->em_dir, SIGNAL(textChanged(QString)),
            this,     SLOT(Reset()));

    // 중복 확인
    connect(ui->join_idchk, SIGNAL(clicked()),
            this,     SLOT(dupliButton()));
    connect(ui->join_hpchk, SIGNAL(clicked()),
            this,     SLOT(dupliButton()));
    connect(ui->join_emchk, SIGNAL(clicked()),
            this,     SLOT(dupliButton()));

    connect(ui->join_okay, SIGNAL(clicked()),
            this,     SLOT(sendButton()));

    // 뒤로가기
    connect(ui->join_exit, SIGNAL(clicked()),
            this,     SLOT(showlogin()));

    // ex3. 통신을 위한 initialize() 실행
    initialize();
}

void Join::initialize()
{
    tcpSocket = new QTcpSocket(this);
}

QHostAddress Join::getMyIP()
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

void Join::sendMessage(const QString &message)
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

void Join::sendButton()
{
    QString idchk = ui->id_chk->toPlainText();
    QString hpchk = ui->hp_chk->toPlainText();
    QString emchk = ui->em_chk->toPlainText();
    if(emchk.startsWith("이미")) {
        ui->textEdit->setText("Email 중복확인을 해주세요");
    } else {
        em_dupchk = true;
    }
    if(hpchk.startsWith("이미")) {
        ui->textEdit->setText("HP 중복확인을 해주세요");
    } else {
        hp_dupchk = true;
    }
    if (idchk.startsWith("이미")) {
        ui->textEdit->setText("ID 중복확인을 해주세요");
    } else {
        id_dupchk = true;
    }
    if(id_dupchk && hp_dupchk && em_dupchk) {
        if(passwordchk()) {
            QString id = ui->input_id->text().trimmed();
            QString name = ui->input_name->text().trimmed();
            QString nick = ui->input_nick->text().trimmed();
            QString pw = ui->input_pw->text().trimmed();
            QString hp1chk = ui->input_hp1->text().trimmed();
            QString hp2chk = ui->input_hp2->text().trimmed();
            QString hp = "010" + ui->input_hp1->text() + ui->input_hp2->text();
            QString emchk1 = ui->input_email->text().trimmed();
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
            QString message = "join: " + id + "," + name + "," + nick + "," + pw + "," + hp + "," + em;
            if (!id.isEmpty() && !name.isEmpty() && !nick.isEmpty() && !pw.isEmpty() && !hp1chk.isEmpty() && !hp2chk.isEmpty() && !emchk1.isEmpty() && !emchk2.isEmpty()) {
                sendMessage(message);
            } else {
                if (id.isEmpty()){
                    ui->textEdit->setText("ID를 입력해주세요");
                } else if (name.isEmpty()) {
                    ui->textEdit->setText("이름을 입력해주세요");
                } else if (nick.isEmpty()) {
                    ui->input_nick->setText(ui->input_name->text());
                    if (!id.isEmpty() && !name.isEmpty() && !pw.isEmpty() && !hp1chk.isEmpty() && !hp2chk.isEmpty() && !emchk1.isEmpty() && !emchk2.isEmpty()) {
                        sendMessage(message);
                    } else {
                        if (id.isEmpty()){
                            ui->textEdit->setText("ID를 입력해주세요");
                        } else if (name.isEmpty()) {
                            ui->textEdit->setText("이름을 입력해주세요");
                        } else if (pw.isEmpty()) {
                            ui->textEdit->setText("pw를 입력해주세요");
                        } else if (hp1chk.isEmpty() || hp2chk.isEmpty()) {
                            ui->textEdit->setText("HP를 입력해주세요");
                        } else if (emchk1.isEmpty() || emchk2.isEmpty()) {
                            ui->textEdit->setText("Email을 입력해주세요");
                        }
                    }
                } else if (pw.isEmpty()) {
                    ui->textEdit->setText("pw를 입력해주세요");
                } else if (hp1chk.isEmpty() || hp2chk.isEmpty()) {
                    ui->textEdit->setText("HP를 입력해주세요");
                } else if (emchk1.isEmpty() || emchk2.isEmpty()) {
                    ui->textEdit->setText("Email을 입력해주세요");
                }
            }
        } else {
            ui->textEdit->setText("pw확인을 해주세요");
        }
    }
}

void Join::Reset()
{
    QLineEdit *validchk = qobject_cast<QLineEdit*>(sender());
    if (validchk == ui->input_id) {
        ui->id_chk->clear();
    } else if (validchk == ui->input_hp1 || validchk == ui->input_hp2) {
        ui->hp_chk->clear();
    } else if (validchk == ui->input_email || validchk == ui->em_dir) {
        ui->em_chk->clear();
    }
}

void Join::dupliButton()
{
    QString message;
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton == ui->join_idchk) {
        QString id = "ID중복: " + ui->input_id->text();
        message = id;
    } else if (clickedButton == ui->join_hpchk) {
        QString hp = "HP중복: 010" + ui->input_hp1->text() + ui->input_hp2->text();
        message = hp;
    } else if (clickedButton == ui->join_emchk) {
        QString em;
        if (ui->em_sel->currentIndex() == ui->em_sel->count() - 1) {
            // "직접입력" 옵션이 선택된 경우
            em = "EM중복: " + ui->input_email->text() + "@" + ui->em_dir->text();
        } else {
            // 다른 옵션이 선택된 경우
            em = "EM중복: " + ui->input_email->text() + "@" + ui->em_sel->currentText();
        }
        message = em;
    } else {
        message = "Unknown button clicked";
    }

    sendMessage(message);
}

void Join::validchk()
{
    QString pw="";
    QLineEdit *inputinfo = qobject_cast<QLineEdit*>(sender());
    if (inputinfo == ui->input_id) {
        QString id = ui->input_id->text();
        if(id.length() < 6 || !IDchk(id)) {
            ui->input_id->setText("사용할 수 없는 ID입니다.");
            QTimer::singleShot(1000, [this]() {
                ui->input_id->clear();
            });
        }
    } else if (inputinfo == ui->input_name) {
        QString name = ui->input_name->text();
        if(name.length() == 0 || !namechk(name)) {
            ui->input_name->setText("다시 입력해주세요");
            QTimer::singleShot(1000, [this]() {
                ui->input_name->clear();
            });
        } else {
            ui->input_nick->setText(name);
        }
    } else if (inputinfo == ui->input_nick) {
        QString nick = ui->input_nick->text();
        if(!Specialchk(nick)) {
            ui->input_nick->clear();
        }
    }  else if (inputinfo == ui->input_pw) {
        pw = ui->input_pw->text();
        if(pw.length()<8 || !containsMixed(pw)) {
            ui->input_pw->setText("사용할 수 없는 PW입니다.");
            ui->input_pw->setEchoMode(QLineEdit::Normal);
            QTimer::singleShot(1000, [this]() {
            ui->input_pw->clear();
            ui->input_pw->setEchoMode(QLineEdit::PasswordEchoOnEdit);
            });
        }
    } else if (inputinfo == ui->input_pw2) {
        pw = ui->input_pw->text();
        QString pw2 = ui->input_pw2->text();
        if(pw2 != pw) {
            ui->input_pw2->setText("비밀번호가 일치하지 않습니다.");
            ui->input_pw2->setEchoMode(QLineEdit::Normal);
            QTimer::singleShot(1000, [this]() {
                ui->input_pw2->clear();
                ui->input_pw2->setEchoMode(QLineEdit::PasswordEchoOnEdit);
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

QString Join::emailselect(int index)
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
        ui->em_chk->clear();
        ui->em_dir->clear();
    }
    return email;
}

bool Join::containsOnlyDigits(const QString &str)
{
    for (const QChar &ch : str) {
        if (!ch.isDigit()) {
            return false;
        }
    }
    return true;
}

bool Join::IDchk(const QString &str)
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

bool Join::containsMixed(const QString &str)
{
    int cnt_a=0;
    int cnt_n=0;
    for (const QChar &ch : str) {
        ushort unicode = ch.unicode();
        if ((unicode >= 'A' && unicode <= 'Z') ||
            (unicode >= 'a' && unicode <= 'z')) {
            cnt_a++;
            continue;
        } else if((unicode >= '0' && unicode <= '9')) {
            cnt_n++;
            continue;
        } else {
            return false; // 영문자나 숫자가 아닌 문자가 있으면 false 반환
        }
    }
    if(cnt_a>0 && cnt_n>0) {
        return true;
    } else {
        return false;
    }
}

bool Join::passwordchk()
{
    QString pw = ui->input_pw->text();
    QString pwchk = ui->input_pw2->text();
    if(pw == pwchk){
        return true;
    }
    return false;
}

bool Join::hangulchk(ushort unicode)
{
    // 한글 음절의 시작 유니코드와 끝 유니코드
    const ushort first = 0xAC00;
    const ushort last = 0xD7A3;

    return (unicode >= first && unicode <= last);
}

bool Join::namechk(const QString &name)
{
    for (const QChar &ch : name) {
        // 유니코드 범위로 한글 문자인지 확인
        if (!ch.isLetter() || ch.unicode() < 0xAC00 || ch.unicode() > 0xD7A3) {
            return false;
        }
        // 완성된 한글인지 확인
        if (!hangulchk(ch.unicode())) {
            return false;
        }
    }
    return true;
}

bool Join::Specialchk(const QString &email)
{
    for (const QChar &ch : email) {
        if (!ch.isLetterOrNumber()) {
            return false; // 특수 문자가 있다면 false 반환
        }
    }
    return true; // 특수 문자가 없다면 true 반환
}

void Join::getsignal()
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

void Join::getTcpSocket(QTcpSocket *tcpsocket)
{
    tcpSocket = tcpsocket;
}

void Join::showlogin()
{
    connect(this, SIGNAL(gologin()), parentWidget, SLOT(showlogin()));
    QFrame *joinFrame = findChild<QFrame *>("Join");
    if (joinFrame) {
        joinFrame->hide();
    }
    emit gologin();
}

void Join::disconnectButton()
{
    qDebug() << Q_FUNC_INFO << "서버로부터 접속 종료.";
    tcpSocket->disconnectFromHost();
}

Join::~Join()
{
    delete ui;
}
