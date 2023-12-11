#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QToolButton>
#include <QDateTime>
#include <QKeyEvent>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    connect(ui->connectButton, SIGNAL(clicked()),
            this,              SLOT(connectButton()));
    ui->groupBox_chat->setVisible(false);
    connect(ui->lineEdit,SIGNAL(editingFinished()),
            this,SLOT(joinIdFlag()));
    connect(ui->lineEdit_4,SIGNAL(editingFinished()),
            this,SLOT(joinEmailFlag()));
    connect(ui->lineEdit_5,SIGNAL(editingFinished()),
            this,SLOT(joinPhoneFlag()));
    initialize();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initialize()
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()),
            this,      SLOT(readMessage()));
    connect(tcpSocket, SIGNAL(disconnected()),
            this,      SLOT(disconnected()));
}

void Widget::connectButton()
{
//    serverIP = ui->serverIP->text().trimmed();
//    serverPort = ui->serverPort->text().trimmed();

    // 매번 변경해줘야할 부분 ★★★★
//    serverIP = "172.16.4.74";
    serverIP = "10.10.21.116";
//    serverIP = "192.168.0.2";
    serverPort = "25000";

    QHostAddress serverAddress(serverIP);
    tcpSocket->connectToHost(serverAddress, serverPort.toUShort());
//    ui->textEdit->append("<- 서버에게 연결 요청");
}

void Widget::readMessage()
{
    if (tcpSocket->bytesAvailable() > 0)
    {
        QByteArray readData = tcpSocket->readAll();
//        ui->textEdit->append("-> " + readData);

        QList<QString> responseList = QString::fromUtf8(readData).split("●");
        if (responseList[0] == "LOGIN_SUCCESS")
        {
            userlist.append(loginID);
            ui->stackedWidget->setCurrentIndex(5);
//            ui->usernameEdit->clear();
//            ui->passwordEdit->clear();
        }
        else if (responseList[0] == "LOGOUT_SUCCESS")
        {
            tcpSocket->disconnectFromHost();
            ui->stackedWidget->setCurrentIndex(1);
            qDebug() << "서버가 종료되었습니다.";
        }
        else if (responseList[0] == "existnick"){
            QMessageBox::warning(this, "닉네임 중복", "이미 존재하는 닉네임입니다");
        }
        else if (responseList[0] == "notexistnick"){
            // 우선 값을 불러오기 위해 값을 보내고 스택위젯을 다음페이지로 넘김
            ui->tabWidget->setCurrentIndex(1);
            loginNICK = ui->lineEdit_inputnick->text();
            QString goUserlist = "goUserlist";
            QMessageBox::information(this, "닉네임", "다음 닉네임을 사용합니다.");
            tcpSocket->write(goUserlist.toUtf8());

//            ui->lineEdit_inputnick->clear();
            ui->stackedWidget->setCurrentIndex(6);
        }
        // 0907 추가한 부분(제목 존재 여부 체크)
        else if (responseList[0]=="existtitle"){
            QMessageBox::warning(this, "채팅창 제목 중복", "이미 존재하는 제목입니다");
        }
        else if (responseList[0]=="notexisttitle"){
            // 중복 확인 후 보내기
            QMessageBox::information(this, "채팅창 제목 사용 가능", " 사용가능한 제목입니다");
            QString goChatroom = "gochatroom," + chattitle;
            QByteArray byteArray;
            QDataStream stream(&byteArray, QIODevice::WriteOnly);
            stream << goChatroom << userlist;
            tcpSocket->write(byteArray);
            ui->groupBox_chat->setVisible(false);
        }
        else if (responseList[0]=="receiveText"){
            QString sender = responseList[1];
            QString msg = responseList[2];
            QString currTime = QTime::currentTime().toString("hh:mm");
            QString message2 = sender + ":" + msg + "(" + currTime + ")";
            ui->textBrowser->append(message2);
        }
        else if(responseList[0] == "중복된 아이디입니다."){
            ui->notice_signin->setText("중복된 아이디입니다.");
            QMessageBox::warning(this, "아이디 중복 검사", "아이디 사용 불가");
        }
        else if(responseList[0] == "중복된 핸드폰 번호입니다."){
            ui->notice_signin->setText("중복된 핸드폰 번호입니다.");
            QMessageBox::warning(this, "핸드폰 번호 중복 검사", "핸드폰 번호 사용 불가");
        }
        else if(responseList[0] == "중복된 이메일입니다."){
            ui->notice_signin->setText("중복된 이메일입니다.");
            QMessageBox::warning(this, "이메일 중복 검사", "이메일 사용 불가");
        }
        else if(responseList[0] == "사용가능한 아이디입니다."){
            ui->notice_signin->setText("사용가능한 아이디입니다.");
            QMessageBox::information(this, "아이디 중복 검사", "사용가능한 아이디");
            joinid_flag = 1;
        }
        else if(responseList[0] == "사용가능한 전화번호입니다."){
            ui->notice_signin->setText("사용가능한 전화번호입니다.");
            QMessageBox::information(this, "전화번호 중복 검사", "사용가능한 전화번호");
            joinphone_flag = 1;
        }
        else if(responseList[0] == "사용가능한 이메일입니다."){
            ui->notice_signin->setText("사용가능한 이메일입니다.");
            QMessageBox::information(this, "이메일 중복 검사", "사용가능한 이메일");
            joinemail_flag = 1;
        }
        else if(responseList[0] == "회원가입 완료"){
            QMessageBox::information(this, "회원가입 성공","회원가입이 완료되었습니다.");
            joinid_flag = 0;
            joinemail_flag = 0;
            joinphone_flag = 0;

            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
            ui->lineEdit_4->clear();
            ui->lineEdit_5->clear();
        }
        else if(responseList[0] == "아이디는 :"){
            QString mssg = responseList[1];
            ui->notice_findid->setText("당신의 아이디는 " + mssg);
        }
        else if(responseList[0] == "이메일이 일치하지 않습니다.") {
            ui->notice_findid->setText("이메일이 일치하지 않습니다.");
        }
        else if(responseList[0] == "비밀번호는 :") {
            QString mssg = responseList[1];
            ui->notice_findpw->setText("당신의 비밀번호는 " + mssg);
        }
        else if(responseList[0] == "아이디 또는 전화번호가 일치하지 않습니다.") {
            ui->notice_findpw->setText("아이디 또는 전화번호가 일치하지 않습니다.");
        }
        // 서버에서 내정보 받아오기
        else if(responseList[0] == "mypage"){
            ui->label_mypage_id->setText(responseList[1]);
            ui->label_mypage_pw->setText(responseList[2]);
            ui->label_mypage_name->setText(responseList[3]);
            ui->label_mypage_phone->setText(responseList[4]);
            ui->label_mypage_email->setText(responseList[5]);
            ui->label_mypage_nick->setText(responseList[6]);
        }
        // 벡터 받는 부분
        // 벡터 받았을 때 위의 response로 하는 조건문에 해당되지 않으므로
        // 위의 조건문이 돌아가지 않고 아래의 코드가 실행됨
        QByteArray receivedData = readData;
        QDataStream stream(&receivedData, QIODevice::ReadOnly);
        QString message;
        // 서버에서 벡터 앞에 붙여온 메시지를 먼저 분리
        stream >> message;
        // 분리한 메시지로 유저 리스트 또는 채팅방 리스트에 띄움

        // 유저 리스트
        if (message == "userlist"){
//            QLayoutItem* child;
//            while ((child = ui->verticalLayout_user->takeAt(0)) != nullptr) {
//                delete child->widget();
//                delete child;
//            }
            QVector<QString> names, nicknames;
            stream >> names >> nicknames;
            names.removeOne(loginID);
            nicknames.removeOne(loginNICK);
            idList = names;
            nickList = nicknames;

            int count = qMin(names.size(), nicknames.size());
            qDebug() << count;
            // 받아온 벡터 names, nicknames로 로그인 유저 리스트창의 verticalLayout_user에
            // 토글버튼을 추가해서 버튼 클릭 시 userlist에 유저의 아이디 값이 추가되고
            // 토글버튼 해제 시 userlist에서 유저의 아이디 값이 제거됨
            for (int i = 0; i < count; i++) {
                if (i >= 0 && i < names.size() && i < nicknames.size()) {
                    qDebug() << names[i] + " : " + nicknames[i];

                    // 유저 이름과 닉네임을 합쳐서 텍스트 생성
                    QString itemText = nicknames[i] + "(" + names[i] + ")";

                    // 토글 푸쉬버튼 생성
                    QPushButton* toggleButton = new QPushButton(itemText);
                    toggleButton->setCheckable(true);
                    toggleButton->setFixedHeight(50);
                    ui->verticalLayout_user->addWidget(toggleButton);

                    connect(toggleButton, &QPushButton::toggled, this, [=](bool checked) {
                        if (checked) {
                            // 버튼 누를 시 아이디를 names 벡터에 추가
                            qDebug() << toggleButton->text() + "버튼이 토글됨 (On 상태)";
                            userlist.append(names[i]);
                            qDebug() << userlist;
                            num_user = num_user + 1;
                            qDebug() << num_user;
                        } else {
                            // 버튼 해제 시 아이디를 names 벡터에서 제거
                            qDebug() << toggleButton->text() + "버튼이 토글 해제됨 (Off 상태)";
                            userlist.removeOne(names[i]);
                            qDebug() << userlist;
                            num_user = num_user - 1;
                            qDebug() << num_user;
                        }
                    });
                }else {
                    qDebug() << "유효하지 않은 인덱스에 접근하려고 시도함. 벡터 크기: names-" << names.size() << " nicknames-" << nicknames.size();
                }
            }
        }
        // 채팅방 리스트
        else if (message == "chatroomlist"){
            QVector<QString> new_chatroomlist;
            stream >> new_chatroomlist;
            if (new_chatroomlist.size() > chatroomlist.size()){
                for (int i = 0; i<chatroomlist.size();i++){
                    new_chatroomlist.removeOne(chatroomlist[i]);
                }
                for (int j=0; j<new_chatroomlist.size();j++){
                    chatroomlist.append(new_chatroomlist[j]);
                }
                int count = new_chatroomlist.size();
                for (int i=0; i<count; i++){
                    if(i>=0 && i<count){
                        qDebug() << new_chatroomlist[i];
                        QString itemText = new_chatroomlist[i];
                        QPushButton* toggleButton = new QPushButton(itemText);
                        toggleButton->setCheckable(true);
                        toggleButton->setFixedHeight(50);
                        ui->verticalLayout_chatroom->addWidget(toggleButton);

                        connect(toggleButton, &QPushButton::toggled, this, [=](bool checked) {
                            if (checked) {
                                // 버튼 누를 시 아이디를 names 벡터에 추가
                                qDebug() << toggleButton->text() + "버튼이 토글됨 (On 상태)";
                                roomlist.append(toggleButton->text());
                                qDebug() << roomlist;
                                num_chat = num_chat + 1;
                                qDebug() << num_chat;
                            } else {
                                // 버튼 해제 시 아이디를 names 벡터에서 제거
                                qDebug() << toggleButton->text() + "버튼이 토글 해제됨 (Off 상태)";
                                roomlist.removeOne(toggleButton->text());
                                qDebug() << roomlist;
                                num_chat = num_chat - 1;
                                qDebug() << num_chat;
                            }
                        });
                    }
                }
            }else if (new_chatroomlist.size() == chatroomlist.size()){
                qDebug() << "받아온 리스트와 비교할 리스트가 같아요, 변경 필요 X";
            }else if (new_chatroomlist.size() < chatroomlist.size()){
                for (int i = 0; i<new_chatroomlist.size(); i++){
                    chatroomlist.removeOne(new_chatroomlist[i]);
                }
                for (int j = 0; j<chatroomlist.size(); j++){
                    QString textToDelete = chatroomlist[j];

                    // verticalLayout_user에서 모든 자식 위젯을 검색하고 텍스트와 일치하는 버튼을 삭제
                    for (int i = 0; i < ui->verticalLayout_chatroom->count(); ++i) {
                        QWidget* widget = ui->verticalLayout_chatroom->itemAt(i)->widget();
                        QPushButton* button = qobject_cast<QPushButton*>(widget);

                        if (button && button->text() == textToDelete) {
                            // 버튼을 찾았을 때 삭제
                            ui->verticalLayout_chatroom->removeWidget(button);
                            delete button; // 버튼 삭제
                            break; // 중복된 텍스트를 가진 다른 버튼이 있는 경우 break를 제거하고 모두 삭제할 수 있습니다.
                        }
                    }
                }
                // 기존의 리스트를 새로운 리스트로 변경
                chatroomlist = new_chatroomlist;
            }
        }
        else if (message == "adduserlist"){
            QVector<QString> names, nicknames;
            stream >> names >> nicknames;
            names.removeOne(loginID);
            nicknames.removeOne(loginNICK);
            qDebug() << "기존 리스트 개수" + QString::number(idList.size());
            qDebug() << "새로운 리스트 개수" + QString::number(names.size());
            if (names.size() > idList.size()){
                for (int i = 0; i< idList.size(); i++){
                    names.removeOne(idList[i]);
                    nicknames.removeOne(nickList[i]);
                }
                for (int j = 0; j<names.size(); j++){
                    idList.append(names[j]);
                    nickList.append(nicknames[j]);
                }
                int count = qMin(names.size(), nicknames.size());
                qDebug() << count;
                // 받아온 벡터 names, nicknames로 로그인 유저 리스트창의 verticalLayout_user에
                // 토글버튼을 추가해서 버튼 클릭 시 userlist에 유저의 아이디 값이 추가되고
                // 토글버튼 해제 시 userlist에서 유저의 아이디 값이 제거됨
                for (int i = 0; i < count; i++) {
                    if (i >= 0 && i < names.size() && i < nicknames.size()) {
                        qDebug() << names[i] + " : " + nicknames[i];

                        // 유저 이름과 닉네임을 합쳐서 텍스트 생성
                        QString itemText = nicknames[i] + "(" + names[i] + ")";

                        // 토글 푸쉬버튼 생성
                        QPushButton* toggleButton = new QPushButton(itemText);
                        toggleButton->setCheckable(true);
                        toggleButton->setFixedHeight(50);
                        ui->verticalLayout_user->addWidget(toggleButton);

                        connect(toggleButton, &QPushButton::toggled, this, [=](bool checked) {
                            if (checked) {
                                // 버튼 누를 시 아이디를 names 벡터에 추가
                                qDebug() << toggleButton->text() + "버튼이 토글됨 (On 상태)";
                                userlist.append(names[i]);
                                qDebug() << userlist;
                                num_user = num_user + 1;
                                qDebug() << num_user;
                            } else {
                                // 버튼 해제 시 아이디를 names 벡터에서 제거
                                qDebug() << toggleButton->text() + "버튼이 토글 해제됨 (Off 상태)";
                                userlist.removeOne(names[i]);
                                qDebug() << userlist;
                                num_user = num_user - 1;
                                qDebug() << num_user;
                            }
                        });
                    }else {
                        qDebug() << "유효하지 않은 인덱스에 접근하려고 시도함. 벡터 크기: names-" << names.size() << " nicknames-" << nicknames.size();
                    }
                }

            }else if (names.size() == idList.size()){
                qDebug() << "받아온 리스트와 비교할 리스트가 같아요, 변경 필요 X";
            }
            else if(names.size() < idList.size()){
                for(int i = 0; i<names.size();i++){
                    idList.removeOne(names[i]);
                    nickList.removeOne(nicknames[i]);
                }
                for (int j=0; j<idList.size();j++){
                    QString id = idList[j];
                    QString nick = nickList[j];
                    QString textToDelete = nick + "(" + id + ")";

                    // verticalLayout_user에서 모든 자식 위젯을 검색하고 텍스트와 일치하는 버튼을 삭제
                    for (int i = 0; i < ui->verticalLayout_user->count(); ++i) {
                        QWidget* widget = ui->verticalLayout_user->itemAt(i)->widget();
                        QPushButton* button = qobject_cast<QPushButton*>(widget);

                        if (button && button->text() == textToDelete) {
                            // 버튼을 찾았을 때 삭제
                            ui->verticalLayout_user->removeWidget(button);
                            delete button; // 버튼 삭제
                            break; // 중복된 텍스트를 가진 다른 버튼이 있는 경우 break를 제거하고 모두 삭제할 수 있습니다.
                        }
                    }
                }
                // 기존의 리스트를 새로운 리스트로 변경
                idList = names;
                nickList = nicknames;
            }
        }
        // 0907 추가된 사항
        else if (message == "enterroom"){
            real_members.clear();
            QVector<QString> chatroomlists;
            stream >> chatroomlists;
            qDebug() << "가져온 리스트";
            qDebug() << chatroomlists;

            if (!chatroomlists.contains(loginID)){
                ui->stackedWidget->setCurrentIndex(6);
                QMessageBox::warning(this, "채팅방 선택", "채팅방에 들어갈 권한이 없습니다!!");
            }
            else if (num_chat==1 && chatroomlists.contains(loginID)){
                QMessageBox::information(this, "채팅방", "선택하신 채팅방으로 들어갑니다!!");
                real_chatRoomTitle = chatroomlists[0];
                for (int i = 1; i < chatroomlists.size(); ++i) {
                    real_members.append(chatroomlists[i]);
                }
                qDebug() << "채팅창으로 끌고 갈 채팅 제목과 채팅 구성원 리스트 :";
                qDebug() << real_chatRoomTitle;
                qDebug() << real_members;
                setChatroom();
            }
        }
    }
}

void Widget::f_endstr(QString & Fbt)
{
    QStringList parts = Fbt.split(",");
    QString ID = parts[0];
    QString NICK = parts[1];
    qDebug() << ID << " : " << NICK;
}

void Widget::disconnected()
{
//    ui->textEdit->append("-> 서버로부터 연결 해제");
    qDebug() << Q_FUNC_INFO << "서버로부터 접속 종료.";
}

QHostAddress Widget::getMyIP()
{
    QHostAddress myAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // localhost(127.0.0.1) 가 아닌 것을 사용
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            //qDebug() << ipAddressesList.at(i);
            myAddress = ipAddressesList.at(i);
            break;
        }
    }
    if (myAddress.toString().isEmpty())
        myAddress = QHostAddress(QHostAddress::LocalHost);
    return myAddress;
}

// 로그인 페이지(인덱스 1페이지)
// 로그인 버튼 누를 시 로그인 정보 전송
void Widget::on_login_pushButton_clicked()
{
    loginID = ui->usernameEdit->text();
    loginPW = ui->passwordEdit->text();
    QString loginData = "LOGIN●" + loginID + "●" + loginPW;
    tcpSocket->write(loginData.toUtf8());
    ui->passwordEdit->clear();
}
// 회원가입 페이지 이동
void Widget::on_join_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
// 아이디 찾기 페이지 이동
void Widget::on_idfindButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
// 비밀번호 찾기 페이지 이동
void Widget::on_pwfindButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void Widget::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_back_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_back_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_pushButton_checknick_clicked()
{
    // 닉네임 체크 함수와 더불어 아래 내용 들어갈 것
    QString nickname = "checknick●"+ui->lineEdit_inputnick->text()+"●"+loginID;
    tcpSocket->write(nickname.toUtf8());
}

void Widget::on_pushButton_makechatroom_clicked()
{
    if ((num_user<=4) && (num_user>=1)){
        // 0907 추가 및 변경된 부분
        // (채팅방 만들기 버튼 누를 시 채팅방 입력 받는 그룹박스 열림)
        QMessageBox::information(this, "채팅방 제목 입력", "채팅방 제목을 입력해주세요");
        ui->groupBox_chat->setVisible(true);
    }
    else if (num_user<=0){
        QMessageBox::warning(this, "채팅방 관련", "대화할 상대 최소 1명을 선택해주세요!!");
    }
    else{
        QMessageBox::warning(this, "채팅방 관련", "본인을 제외한 멤버 4명을 선택해주세요!!");
    }
}

void Widget::on_tabWidget_tabBarClicked(int index)
{
    qDebug() << "탭 클릭 이벤트 발생! 인덱스:" << index;
    if (index == 1){
        QString indextext = "index" + QString::number(index);
        qDebug() << indextext;
        tcpSocket->write(indextext.toUtf8());
    }
    else if (index == 0){
        QString indextext = "index" + QString::number(index);
        tcpSocket->write(indextext.toUtf8());
    }
}

void Widget::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        ui->notice_signin->setText("아이디를 입력해주세요.");
        return;
    }else {
        ui->notice_signin->setText("");
    }
    QString USERid = ui->lineEdit->text();
    QByteArray message = "checkID●" + USERid.toUtf8();
    tcpSocket->write(message);
}

void Widget::on_pushButton_3_clicked()
{
    if(ui->lineEdit_4->text().isEmpty()) {
        ui->notice_signin->setText("이메일를 입력해주세요.");
        return;
    }else {
        ui->notice_signin->setText("");
    }
    QString USEid = ui->lineEdit_4->text();
    QByteArray message = "checkML●" + USEid.toUtf8();
    tcpSocket->write(message);
}

void Widget::on_pushButton_4_clicked()
{
    if(ui->lineEdit_5->text().isEmpty()) {
        ui->notice_signin->setText("전화번호를 입력해주세요.");
        return;
    }else {
        ui->notice_signin->setText("");
    }
    QString USEid = ui->lineEdit_5->text();
    QByteArray message = "checkPH●" + USEid.toUtf8();
    tcpSocket->write(message);
}

void Widget::on_pushButton_5_clicked()
{

    QString signID = ui->lineEdit->text();
    QString signPW = ui->lineEdit_2->text();
    QString signNM = ui->lineEdit_3->text();
    QString signML = ui->lineEdit_4->text();
    QString signPH = ui->lineEdit_5->text();
    isIdValid1 = false;
    isPwValid1 = false;
    isEmailValid1 = false;
    isPhoneValid1 = false;

    QRegularExpression idRegex("^[a-zA-Z0-9]{8,20}$");
    if (idRegex.match(signID).hasMatch()) {
        isIdValid1 = true;
    }
    QRegularExpression pwRegex("^[a-zA-Z0-9]{8,20}$");
    if (pwRegex.match(signPW).hasMatch()) {
        isPwValid1 = true;
    }
    QRegularExpression emailRegex("^[A-Za-z0-9]+@[A-Za-z0-9]+\\.[A-Za-z]{2,}$");
    if (emailRegex.match(signML).hasMatch()) {
        isEmailValid1 = true;
    }
    QRegularExpression phoneRegex("^[0-9]{2,}-[0-9]{3,}-[0-9]{4,}$");
    if (phoneRegex.match(signPH).hasMatch()) {
        isPhoneValid1 = true;
    }

    if (!isIdValid1) {
        QMessageBox::warning(this, "회원가입", "아이디를 8~20자이내로 적어주세요");
    }
    else if (!joinid_flag){
        QMessageBox::warning(this, "회원가입", "아이디 중복 확인 요망");
    }
    else if (!isPwValid1) {
        QMessageBox::warning(this, "회원가입", "비밀번호를 8~20자이내로 적어주세요");
    }
    else if (!isEmailValid1) {
        QMessageBox::warning(this, "회원가입", "이메일을 형식에 맞추어 적어주세요");
    }
    else if (!joinemail_flag){
        QMessageBox::warning(this, "회원가입", "이메일 중복 확인 요망");
    }
    else if (!isPhoneValid1) {
        QMessageBox::warning(this, "회원가입", "전화번호를 형식에 맞추어 적어주세요");
    }
    else if (!joinphone_flag){
        QMessageBox::warning(this, "회원가입", "전화번호 중복 확인 요망");
    }
    else{
        QByteArray message = "SIGNIN●" + signID.toUtf8() +"●"+ signPW.toUtf8() +"●"+ signNM.toUtf8() +"●"+ signPH.toUtf8() +"●"+ signML.toUtf8();
        tcpSocket->write(message);
    }

}

void Widget::on_return_findid_clicked()
{
    if(ui->findid_ML->text().isEmpty()) {
        ui->notice_findid->setText("이메일을 입력해주세요.");
        return;
    }else {
        ui->notice_findid->setText("");
    }

    QString findID = ui->findid_ML->text();
    bool isEmailValid = false;

    QRegularExpression emailRegex("^[A-Za-z0-9]+@[A-Za-z0-9]+\\.[A-Za-z]{2,3}$");
    if (emailRegex.match(findID).hasMatch()) {
        isEmailValid = true;
    }else if (!isEmailValid) {
        QMessageBox::warning(this, "아이디찾기", "이메일을 형식에 맞춰 적어주세요");
    }

    QString findid_ML = ui->findid_ML->text();
    QByteArray message = ("findID●" + findid_ML).toUtf8();
    tcpSocket->write(message);
}

void Widget::on_return_findpw_clicked()
{
    if(ui->findpw_ID->text().isEmpty() || ui->findpw_PH->text().isEmpty()) {
        ui->label_9->setText("빈칸을 채워주세요");
        return;
    }else{
        ui->label_9->setText("");
    }

    QString findpw_ID = ui->findpw_ID->text();
    QString findpw_PH = ui->findpw_PH->text();
    bool isIdValid = false;
    bool isPhoneValid = false;
    QRegularExpression idRegex("^[a-zA-Z0-9]{1,20}$");
    if (idRegex.match(findpw_ID).hasMatch()) {
        isIdValid = true;
    }
    QRegularExpression phoneRegex("^[0-9]{2,}-[0-9]{3,}-[0-9]{4,}$");
    if (phoneRegex.match(findpw_PH).hasMatch()) {
        isPhoneValid= true;
    }

    if (!isIdValid) {
        QMessageBox::warning(this, "비밀번호찾기", "아이디를 1~20자 이내로 적어주세요");
    }else if (!isPhoneValid) {
        QMessageBox::warning(this, "비밀번호찾기", "전화번호 형식에 맞춰 적어주세요");
    }

    findpw_ID = ui->findpw_ID->text();
    findpw_PH = ui->findpw_PH->text();
    QByteArray message = "findpw●" + findpw_ID.toUtf8() +"●"+ findpw_PH.toUtf8();
    tcpSocket->write(message);
}
// 수정
void Widget::on_pushButton_9_clicked()
{
    QString msg = ui->chattext->text();
    if (msg==""){
        QMessageBox::warning(this, "채팅창", "채팅창을 반드시 입력 부탁드립니다.");
    }else{
        QString msgsend = "sendtext●" + real_chatRoomTitle + "●" + msg;
                          tcpSocket->write(msgsend.toUtf8());
        ui->chattext->clear();
    }

}

void Widget::showchat()
{
    ui->textBrowser->setPlainText(chatroom);

    if (tcpSocket->bytesAvailable() > 0)
    {
        QByteArray readData = tcpSocket->readAll();
        ui->textBrowser->append(readData);
    }
}

void Widget::setChatroom()
{

    ui->label_23->setText(real_chatRoomTitle);
}

void Widget::on_pushButton_10_clicked()
{
    QString message = "roomout";
    tcpSocket->write(message.toUtf8());
    ui->stackedWidget->setCurrentIndex(6);
}

void Widget::on_pushButton_8_clicked()
{
    QString msgsend = "logout";

    // 로그아웃 시 초기화
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->findid_ML->clear();
    ui->findpw_ID->clear();
    ui->findpw_PH->clear();
    ui->lineEdit_inputnick->clear();
    ui->lineEdit_title->clear();
    ui->textBrowser->clear();
    ui->chattext->clear();

    // new로 생성한 로그인 유저 리스트 클리어
    QLayoutItem *item1;
    while ((item1 = ui->verticalLayout_user->takeAt(0)) != nullptr) {
        if (item1->widget()) {
            // 위젯이 있다면 삭제
            delete item1->widget();
        }
        delete item1;
    }
    userlist.clear();
    num_user = 0;
    // new로 생성한 채팅방 리스트 클리어
    QLayoutItem *item;
    while ((item = ui->verticalLayout_chatroom->takeAt(0)) != nullptr) {
        if (item->widget()) {
            // 위젯이 있다면 삭제
            delete item->widget();
        }
        delete item;
    }
    roomlist.clear();
    num_chat = 0;

    chatroomlist.clear();
    idList.clear();
    nickList.clear();
    loginNICK="";

    tcpSocket->write(msgsend.toUtf8());
}

void Widget::on_pushButton_gochatroom_clicked()
{
//    QString
    if (num_chat==0){
        QMessageBox::warning(this, "채팅방 선택", "반드시 1개의 채팅방을 선택해주세요!!");
    }
    else if (num_chat>1){
        QMessageBox::warning(this, "채팅방 선택", "1개의 채팅방만 선택 가능합니다!!");
    }
    else{
        // roomlist를 보냄
        QString enterroom = "enterroom";
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << enterroom << roomlist;
        tcpSocket->write(byteArray);
        ui->textBrowser->clear();
        ui->stackedWidget->setCurrentIndex(7);
    }
}


void Widget::on_pushButton_applytitle_clicked()
{
        chattitle = ui->lineEdit_title->text();
        QString sendtitle = "chattitle●" + chattitle;
                                //    QString sendtitle = "chattitle●" + chattitle + "●" + loginID;
                                qDebug() << "제목 보냄??";
        tcpSocket->write(sendtitle.toUtf8());
}


void Widget::on_pushButton_cancel_room_clicked()
{
    ui->groupBox_chat->setVisible(false);
}


void Widget::on_check_id_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        ui->notice_signin->setText("아이디를 입력해주세요.");
        return;
    }else {
        ui->notice_signin->setText("");
    }
    QString signID = ui->lineEdit->text();
    QRegularExpression idRegex("^[a-zA-Z0-9]{8,20}$");
    if (idRegex.match(signID).hasMatch()) {
        isIdValid1 = true;
    }
    if (isIdValid1 == false){
        QMessageBox::warning(this, "아이디 형식", "아이디를 8~20자 이내로 적어주세요");
    }else{
        QString USERid = ui->lineEdit->text();
        QByteArray message = "checkID●" + USERid.toUtf8();
                                 tcpSocket->write(message);
    }

}


void Widget::on_check_mail_clicked()
{
    if(ui->lineEdit_4->text().isEmpty()) {
        ui->notice_signin->setText("이메일를 입력해주세요.");
        return;
    }else {
        ui->notice_signin->setText("");
    }
    QString signML = ui->lineEdit_4->text();
    QRegularExpression emailRegex("^[A-Za-z0-9]+@[A-Za-z0-9]+\\.[A-Za-z]{2,}$");
    if (emailRegex.match(signML).hasMatch()) {
        isEmailValid1 = true;
    }
    if (isEmailValid1 == false) {
        QMessageBox::warning(this, "이메일 형식", "이메일 형식을 맞춰서 적어주세요");
    }else {
        QString USEid = ui->lineEdit_4->text();
        QByteArray message = "checkML●" + USEid.toUtf8();
                                 tcpSocket->write(message);
    }

}

void Widget::on_check_phone_clicked()
{
    if(ui->lineEdit_5->text().isEmpty()) {
        ui->notice_signin->setText("전화번호를 입력해주세요.");
        return;
    }else {
        ui->notice_signin->setText("");
    }
    QString signPH = ui->lineEdit_5->text();
    QRegularExpression phoneRegex("^[0-9]{2,}-[0-9]{3,}-[0-9]{4,}$");
    if (phoneRegex.match(signPH).hasMatch()) {
        isPhoneValid1 = true;
    }
    if (isPhoneValid1 == false){
        QMessageBox::warning(this, "핸드폰 번호 형식", "핸드폰 번호 형식을 맞춰서 적어주세요");
    }else{
        QString USEid = ui->lineEdit_5->text();
        QByteArray message = "checkPH●" + USEid.toUtf8();
                                 tcpSocket->write(message);
    }

}


void Widget::on_backtohome_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void Widget::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    QString mypage = "mypage●" + loginID;
    tcpSocket->write(mypage.toUtf8());
}

// 수정
void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        if (ui->stackedWidget->currentIndex() == 1){
            on_login_pushButton_clicked();
        }
        else if (ui->stackedWidget->currentIndex() == 2){
            on_join_pushButton_clicked();
        }
        else if (ui->stackedWidget->currentIndex() == 3){
            on_return_findid_clicked();
        }
        else if (ui->stackedWidget->currentIndex() == 4){
            on_return_findpw_clicked();
        }
        else if (ui->stackedWidget->currentIndex() == 5){
            on_pushButton_checknick_clicked();
        }
        else if (ui->stackedWidget->currentIndex() == 6 && ui->tabWidget->currentIndex() == 1){
            on_pushButton_makechatroom_clicked();
        }
        else if (ui->stackedWidget->currentIndex() == 6 && ui->tabWidget->currentIndex() == 0){
            on_pushButton_gochatroom_clicked();
        }
        else if (ui->stackedWidget->currentIndex() == 7){
            on_pushButton_9_clicked();
        }
        else if (ui->stackedWidget->currentIndex() == 8){
            on_backtohome_clicked();
        }

    }
}

void Widget::joinIdFlag(){
    joinid_flag=0;
}
void Widget::joinEmailFlag(){
    joinemail_flag=0;
}
void Widget::joinPhoneFlag(){
    joinphone_flag=0;
}
