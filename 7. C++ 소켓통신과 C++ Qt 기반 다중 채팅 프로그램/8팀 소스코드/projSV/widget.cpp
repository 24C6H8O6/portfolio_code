#include "widget.h"
#include "ui_widget.h"

#include <QtWidgets>
#include <QDebug>
#include <QtSql>
#include <QtConcurrent/QtConcurrent>
#include <QtNetwork>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("/home/lms116/다운로드/database.db");  // 데이터베이스 파일 이름
     db.setDatabaseName("C:/Users/user/Downloads/database.db");
    if (!db.open())
    {
        qDebug() << "Error: Failed to connect database.";
    }

//    QSqlQuery createTableQuery;
//    createTableQuery.prepare("CREATE TABLE IF NOT EXISTS users (name TEXT, password TEXT, Phone_number TEXT, email TEXT, logged_in INTEGER)");
//    if (!createTableQuery.exec())
//    {
//        qDebug() << "Error: Failed to create table.";
//    }
    clientSocketList.clear();
    initialize();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

Widget::~Widget()
{
    foreach (QTcpSocket *clientSocket, clientSocketList) {
        clientSocket->disconnectFromHost();
        clientSocket->deleteLater();
    }
    delete ui;
}

void Widget::initialize()
{
    // e4. hostAddress 찾기
    // (이 소스코드가 실행될 프로그램(서버)의 ip 찾기)
    QHostAddress hostAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // localhost(127.0.0.1) 가 아닌 것을 사용
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            //qDebug() << ipAddressesList.at(i);
            hostAddress = ipAddressesList.at(i);
            break;
        }
    }

    // 인터넷이 연결되어 있지 않다면, localhost(127.0.0.1) 사용
    if (hostAddress.toString().isEmpty())
        hostAddress = QHostAddress(QHostAddress::LocalHost);

    // e5. tcpServer 서버 소켓 생성
    // [1] 생성 : 서버 소켓 생성
    // [2] 결합 : 서버 IP 결합
    tcpServer = new QTcpServer(this);

    // e6. tcpServer 소켓 대기 시작
    // [3] 대기 : 클라이언트의 연결 요청 대기
    if (!tcpServer->listen(QHostAddress::Any, 25000)) {
        QMessageBox::critical(this, tr("TCP Server"),
                              tr("서버를 시작할 수 없습니다. 에러메세지 : %1.")
                              .arg(tcpServer->errorString()));
        close();
        return; // 실패시 종료
    }

    // tcpServer 소켓이 정상적으로 생성되면, 서버 정보를 lable widget에 표시
    ui->labelStatus->setText(tr("서버 동작 중 \n\n"
                                "IP : %1\n"
                                "PORT : %2\n")
                                 .arg(hostAddress.toString()).arg(tcpServer->serverPort()));

    // e6.1 Connect, Socket Signal, class Slot 설정
    // tcpServer 소켓에 클라이언트의 연결 요청 Signal 이 생기면, 동작할 Slot 연결
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    ui->connMsgEdit->clear();
}

void Widget::newConnection()
{
    QTcpSocket *connectedSocket = tcpServer->nextPendingConnection();

    if (!connectedSocket) {
        qDebug() << "New Connection: Connected socket is null.";
        return;
    }

    QHostAddress clientAddr = connectedSocket->peerAddress();
    quint16 clientPort = connectedSocket->peerPort();

    qDebug() << "New Connection from:" << clientAddr.toString() << "port:" << clientPort;

    ClientInfo clientInfo;
    clientInfo.socket = connectedSocket;
    clientInfo.address = clientAddr;
    clientInfo.port = clientPort;


    clientList.append(clientInfo);

    connect(connectedSocket, &QTcpSocket::readyRead, this, &Widget::readMessage);
    connect(connectedSocket, &QTcpSocket::disconnected, this, &Widget::clientDisconnected);

    ui->connMsgEdit->append("클라이언트 접속: " + clientAddr.toString() + ", 포트: " + QString::number(clientPort));
}

void Widget::sendDataToClient(const QString &data, const QHostAddress &address, quint16 port) {
    foreach (const ClientInfo &clientInfo, clientList) {
        if (clientInfo.address == address && clientInfo.port == port) {
            clientInfo.socket->write(data.toUtf8());
            break;
        }
    }
}

void Widget::clientDisconnected()
{
    QTcpSocket *disconnectedSocket = qobject_cast<QTcpSocket *>(sender());
    if (!disconnectedSocket)
        return;

    QHostAddress clientAddr = disconnectedSocket->peerAddress();
    quint16 clientPort = disconnectedSocket->peerPort();

    qDebug() << "Client Disconnected:" << clientAddr.toString() << "port:" << clientPort;

    for (int i = 0; i < clientList.size(); ++i) {
        if (clientList[i].socket == disconnectedSocket) {
            QString userId = clientList[i].userId; // 사용자 ID 얻기
            // 사용자 ID와 관련된 정리 작업이나 동작 수행
            clientList.removeAt(i);
            break;
        }
    }

    disconnectedSocket->deleteLater();

    ui->connMsgEdit->append("클라이언트 접속 해제: " + clientAddr.toString() + ", 포트: " + QString::number(clientPort));
}

void Widget::readMessage()
{
    QTcpSocket *connectedSocket = qobject_cast<QTcpSocket *>(sender());
    if (!connectedSocket)
        return;

    QByteArray readData = connectedSocket->readAll(); // 수정: clientSocket -> connectedSocket
    processRequest(readData, connectedSocket); // 수정: clientSocket -> connectedSocket
    ui->connMsgEdit->append("-> 클라이언트에서 보낸 메시지: " + readData);
}

void Widget::on_pushButton_clicked()
{
    QString message = ui->lineEdit->text();
    qDebug() << "Message sent to client:" << message;
    QByteArray byteArray = message.toUtf8();
    if (!clientList.isEmpty()) {
        clientList[0].socket->write(byteArray); // 수정: clientSocket -> clientList[0].socket
    }
    ui->connMsgEdit->append("-> 서버에서 클라이언트로 보냄: " + message);
}

void Widget::on_pushButton_2_clicked()
{
    if (!clientList.isEmpty()) {
        clientList[0].socket->disconnectFromHost(); // 수정: clientSocket -> clientList[0].socket
    }
}

void Widget::processRequest(const QByteArray &requestData, QTcpSocket *clientSocket)
{
    QString requestString = QString::fromUtf8(requestData).trimmed(); // 공백 제거
    // 데이터 분석 로직
    QStringList parts = requestString.split("●");
    if (parts.size() >= 3 && parts[0] == "LOGIN") {
        QString username = parts[1];
        QString password = parts[2];
        qDebug() << "로그인???";
        QString response;
        if (performLogin(username, password,clientSocket)) {
            response = "LOGIN_SUCCESS●";
        } else {
            response = "LOGIN_FAIL";
        }

        // 클라이언트에 응답 전송
        clientSocket->write(response.toUtf8());
    }
    else if(parts.size() >= 6 && parts[0] == "SIGNIN")
    {
        QString username = parts[1];
        QString password = parts[2];
        QString realname = parts[3];
        QString phonenumber = parts[4];
        QString email = parts[5];
        QString response = performSign(username, password, realname, phonenumber, email);
        clientSocket->write(response.toUtf8());
    }
    else if(parts.size() >= 2 && parts[0] == "findID") {
        QString email = parts[1];
        performIdfind(email,clientSocket);
    }
    else if(parts.size() >= 3 && parts[0] == "findpw") {
        QString username = parts[1];
        QString phone_number = parts[2];
        performPwfind(username,phone_number,clientSocket);
    }
    else if(parts.size() >= 2 && parts[0] == "checkID"){
        QString username = parts[1];
        performIdduplicate(username,clientSocket);
    }
    else if(parts.size() >= 2 && parts[0] == "checkPH"){
        QString pn = parts[1];
        performPHduplicate(pn,clientSocket);
    }
    else if(parts.size() >= 2 && parts[0] == "checkML"){
        QString mail = parts[1];
        performMLduplicate(mail,clientSocket);
    }
    else if (parts.size() >= 1 && parts[0] == "logout") {
        QString response;
        QString username = ""; // 클라이언트의 사용자 ID를 저장할 변수
        for (const ClientInfo &clientInfo : clientList) {
            if (clientInfo.socket == clientSocket) {
                username = clientInfo.userId;
                break;
            }
        }

        if (!username.isEmpty() && performLogout(username, clientSocket)) {
            response = "LOGOUT_SUCCESS";
        } else {
            response = "LOGOUT_FAIL";
        }

        // 클라이언트에 응답 전송
        clientSocket->write(response.toUtf8());
    }
    else if (parts.size() >= 3 && parts[0] == "sendtext") {
        QString teamselect = parts[1];
        QString receivetext = parts.mid(2).join(" "); // 모든 부분을 합칠 때 공백을 추가
        QString response = ""; // 사용자 ID와 메시지를 담을 변수

        QString teamName = "AllPeople"; // 기본 값은 AllPeople로 설정

        if (teamselect != "AllPeople") {
            teamName = teamselect; // 선택한 팀이 AllPeople이 아닌 경우 해당 팀 이름 저장
        }

        // 보내는 사람의 이름 찾기
        QString senderUsername;
        for (const ClientInfo &clientInfo : clientList) {
            if (clientInfo.socket == clientSocket) {
                senderUsername = clientInfo.nickname;
                break;
            }
        }

        for (const ClientInfo &clientInfo : clientList) {
            if (teamselect == "AllPeople" || clientInfo.team == teamselect) {
                response = "receiveText●" +senderUsername + "●" + receivetext;
                qDebug() << "작동됨!!";
                clientInfo.socket->write(response.toUtf8());
            }
        }
    }


    else if (parts.size() >= 3 && parts[0] == "whisper") {
        QString targetUser = parts[1];
        QString whisperMessage = parts.mid(2).join(" ");
        QString responseToSender = "귓속말 to " + targetUser + ": " + whisperMessage;

                                           QString senderUsername; // 송신자의 사용자 이름

        // 송신자의 사용자 이름 찾기
        for (const ClientInfo &clientInfo : clientList) {
            if (clientInfo.socket == clientSocket) {
                senderUsername = clientInfo.userId;
                break;
            }
        }

        // 귓속말 메시지를 적절한 클라이언트에게 보내기
        for (const ClientInfo &clientInfo : clientList) {
            if (clientInfo.userId == targetUser) {
                QString responseToReceiver = "귓속말 from " + senderUsername + ": " + whisperMessage;
                                                                                             clientInfo.socket->write(responseToReceiver.toUtf8());
            } else if (clientInfo.socket == clientSocket) {
                clientInfo.socket->write(responseToSender.toUtf8());
            }
        }
    }

    else if (parts.size() >= 1 && parts[0] == "AllPeople") {
        // 클라이언트가 선택한 팀 정보를 서버에 저장
        for (int i = 0; i < clientList.size(); ++i) {
            if (clientList[i].socket == clientSocket) {
                clientList[i].team = "AllPeople";
                break;
            }
        }
    }

    // "team1", "team2", "team3" 요청 처리
    else if (parts.size() >= 1 && (parts[0] == "team1" || parts[0] == "team2" || parts[0] == "team3")) {
        QString selectedTeam = parts[0];

        // 클라이언트가 선택한 팀 정보를 서버에 저장
        for (int i = 0; i < clientList.size(); ++i) {
            if (clientList[i].socket == clientSocket) {
                clientList[i].team = selectedTeam;
                break;
            }
        }
    }
    // 로그인한 유저 리스트를 로그인 했을 때 바로 띄우기 위한 값 받아오기
    else if (parts[0]=="goUserlist"){
        names.clear();
        nicknames.clear();
        // 유저 리스트 쿼리문
        // 벡터 names와 nicknames에 현재 로그인 되어있는 유저의 아이디,닉네임 값을 각각 저장하여
        // QDataStream으로 message와 함께 보냄
        QSqlQuery query;
        if(!query.exec("SELECT name, nickname FROM users WHERE logged_in = 1"))
        {
            qDebug() << "Failed to execute query." << query.lastError().text();
            return;
        }

        while (query.next()) {
            QString name = query.value(0).toString();
            QString nickname = query.value(1).toString();
            nicknames.append(nickname);
            names.append(name);
        }
        QString message = "userlist";
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << message << names << nicknames;
        clientSocket->write(byteArray);
    }

    // 유저리스트 탭을 선택했을 때 기존 유저리스트와 로그인된 유저 수 비교
    else if (parts[0] == "index1"){
        // 새로운 로그인 유저 리스트 뽑기(아이디, 닉네임)
        QVector<QString> new_names;
        QVector<QString> new_nicknames;
        QSqlQuery query;
        if(!query.exec("SELECT name, nickname FROM users WHERE logged_in = 1"))
        {
            qDebug() << "Failed to execute query." << query.lastError().text();
            return;
        }
        while (query.next()) {
            QString name = query.value(0).toString();
            QString nickname = query.value(1).toString();
            new_nicknames.append(nickname);
            new_names.append(name);
        }

        QString message = "adduserlist";
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << message << new_names << new_nicknames;
        clientSocket->write(byteArray);
    }
    // 채팅방리스트 탭을 선택했을 때
    else if (parts[0] == "index0"){
        QVector<QString> chatroomlist;
        QSqlQuery query;
        if(!query.exec("SELECT roomname FROM NewTable"))
        {
            qDebug() << "Failed to execute query." << query.lastError().text();
            return;
        }
        while (query.next()){
            QString chatroom = query.value(0).toString();
            chatroomlist.append(chatroom);
        }
        QString message = "chatroomlist";
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << message << chatroomlist;
        clientSocket->write(byteArray);
    }
    // 닉네임 입력 선택했을 때
    else if (parts[0] == "checknick"){
        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE nickname=?");
        query.addBindValue(parts[1]);
        if(!query.exec())
        {
            qDebug() << "Failed to execute query." << query.lastError().text();
            return;
        }
        if (query.next()){
            QString existnick = "existnick●";
            clientSocket->write(existnick.toUtf8());
        }else{
            QSqlQuery query2;
            query2.prepare("UPDATE users SET nickname=? WHERE name=?");
            query2.addBindValue(parts[1]);
            query2.addBindValue(parts[2]);
            for (int i = 0; i < clientList.size(); ++i) {
                if (clientList[i].socket == clientSocket) {
                    clientList[i].nickname = parts[1];
                    break;
                }
            }
            if(query2.exec()){
                QString noexistnick = "notexistnick●";
                clientSocket->write(noexistnick.toUtf8());
            }
        }
    }
    // 채팅방 제목 중복 확인(0907 추가된 부분 413~525줄 까지 수정 및 추가)
    else if (parts[0] == "chattitle"){
        QSqlQuery query;
        query.prepare("SELECT * FROM NewTable WHERE roomname=?");
        query.addBindValue(parts[1]);
        if(!query.exec())
        {
            qDebug() << "Failed to execute query." << query.lastError().text();
            return;
        }
        if (query.next()){
            QString existtitle = "existtitle●";
            clientSocket->write(existtitle.toUtf8());
        }else{
            QString notexisttitle = "notexisttitle●";
            clientSocket->write(notexisttitle.toUtf8());
        }
    }

    // 내정보 창 띄우기
    else if (parts[0]=="mypage"){
        QSqlQuery query;
        query.prepare("SELECT name, password, Phone_number, email, nickname, rname FROM users WHERE name = ?");
        query.addBindValue(parts[1]);

        if (query.exec()) {
            if(query.next()) {
                qDebug() << "내정보 보내기";
                QString id = query.value(0).toString();
                QString pw = query.value(1).toString();
                QString ph = query.value(2).toString();
                QString ml = query.value(3).toString();
                QString nick = query.value(4).toString();
                QString nm = query.value(5).toString();
                QString MYINFO = "mypage●" + id +"●"+ pw +"●"+ nm +"●"+ ph +"●" + ml +"●"+ nick;
                clientSocket->write(MYINFO.toUtf8());
            }
        }
    }
    QByteArray receivedData = requestData;
    QDataStream stream(&receivedData, QIODevice::ReadOnly);
    QString message;

    stream >> message;
    QStringList messages = message.split(",");
    if (messages[0] == "gochatroom"){
        QVector<QString> users;
        stream >> users;
        qDebug() << "채팅방 DB에 들어갈 벡터";
        qDebug() << users;
        if (users.size()==2){
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO NewTable (roomname, mem1, mem2) VALUES (?,?,?)");
            insertQuery.addBindValue(messages[1]);
            insertQuery.addBindValue(users[0]);
            insertQuery.addBindValue(users[1]);
            if (insertQuery.exec()) {
                qDebug() << "DB 업데이트 성공!";
            }else{
                qDebug() << "DB 업데이트 실패!";
            }
        }
        else if (users.size()==3){
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO NewTable (roomname, mem1, mem2, mem3) VALUES (?,?,?,?)");
            insertQuery.addBindValue(messages[1]);
            insertQuery.addBindValue(users[0]);
            insertQuery.addBindValue(users[1]);
            insertQuery.addBindValue(users[2]);
            if (insertQuery.exec()) {
                qDebug() << "DB 업데이트 성공!";
            }else{
                qDebug() << "DB 업데이트 실패!";
            }
        }
        else if (users.size()==4){
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO NewTable (roomname, mem1, mem2, mem3, mem4) VALUES (?,?,?,?,?)");
            insertQuery.addBindValue(messages[1]);
            insertQuery.addBindValue(users[0]);
            insertQuery.addBindValue(users[1]);
            insertQuery.addBindValue(users[2]);
            insertQuery.addBindValue(users[3]);
            if (insertQuery.exec()) {
                qDebug() << "DB 업데이트 성공!";
            }else{
                qDebug() << "DB 업데이트 실패!";
            }
        }
        else if (users.size()==5){
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO NewTable (roomname, mem1, mem2, mem3, mem4, mem5) VALUES (?,?,?,?,?,?)");
            insertQuery.addBindValue(messages[1]);
            insertQuery.addBindValue(users[0]);
            insertQuery.addBindValue(users[1]);
            insertQuery.addBindValue(users[2]);
            insertQuery.addBindValue(users[3]);
            insertQuery.addBindValue(users[4]);
            if (insertQuery.exec()) {
                qDebug() << "DB 업데이트 성공!";
            }else{
                qDebug() << "DB 업데이트 실패!";
            }
        }
    }
    else if (messages[0]=="enterroom")
    {
        qDebug() << "enterroom";
        QVector<QString> chatroomlists;
        stream >> chatroomlists;
        qDebug() << "채팅방 DB에 들어갈 벡터";
        qDebug() << chatroomlists;
        QSqlQuery query;
        query.prepare("SELECT mem1, mem2, mem3, mem4, mem5 FROM NewTable WHERE roomname = ?");
        query.addBindValue(chatroomlists[0]);
        if(!query.exec())
        {
            qDebug() << "Failed to execute query." << query.lastError().text();
            return;
        }
        if (query.next()){
            chatroomlists.append(query.value(0).toString());
            chatroomlists.append(query.value(1).toString());
            chatroomlists.append(query.value(2).toString());
            chatroomlists.append(query.value(3).toString());
            chatroomlists.append(query.value(4).toString());
            for (int i = 0; i < clientList.size(); ++i) {
                if (clientList[i].socket == clientSocket) {
                    clientList[i].team = chatroomlists[0];
                    break;
                }
            }
        }
        QString message = "enterroom";
        qDebug() << "enterroom에 속한 벡터 chatroomlists 보냄";
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << message << chatroomlists;
        clientSocket->write(byteArray);
    }
}

bool Widget::performLogin(QString username, QString password, QTcpSocket *clientSocket)

{
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE name = :username AND password = :password AND logged_in = 0");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec())
    {
        qDebug() << "Error: Failed to execute query.";
        return false;
    }

    if (query.next())
    {
        QString userId = username;

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE users SET logged_in = 1 WHERE name = :username");
        updateQuery.bindValue(":username", username);

        if (!updateQuery.exec()) {
            qDebug() << "Error: Failed to update login status.";
            // Handle the error, such as rolling back the login transaction
            return false;
        }

        // 로그인 성공한 사용자 정보를 ClientInfo에 저장
        for (int i = 0; i < clientList.size(); ++i) {
            if (clientList[i].socket == clientSocket) {
                clientList[i].userId = userId;
                clientList[i].team = "AllPeople";
                break;
            }
        }
        return true; // 로그인 성공
    }
    else
    {
        QSqlQuery duplicateQuery;
        duplicateQuery.prepare("SELECT * FROM users WHERE name = :username AND logged_in = 1");
        duplicateQuery.bindValue(":username", username);
        if (duplicateQuery.exec() && duplicateQuery.next()) {
            QString response = "다른 사용자가 로그인 하고 있습니다.ㅇㅇ";
            sendDataToClient(response, clientSocket->peerAddress(), clientSocket->peerPort()); // 수정: clientSocket -> sendDataToClient
            qDebug() << "Login failed due to duplicate login attempt.";
        }
        else {
            QString response = "ID나 비번이 틀렸습니다.";
            sendDataToClient(response, clientSocket->peerAddress(), clientSocket->peerPort()); // 수정: clientSocket -> sendDataToClient
            qDebug() << "Login failed due to invalid username or password.";
        }

        return false; // 로그인 실패
    }
}

//username, password, realname, phonenumber, email
QString Widget::performSign(QString username, QString password, QString realname, QString phone_number, QString email)
{
    QSqlQuery query;

    // 아이디 중복 확인 쿼리
    query.prepare("SELECT * FROM users WHERE name = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        qDebug() << "중복 확인 쿼리 실행 오류:" << query.lastError().text();
            return "회원가입 실패: 중복 확인 오류";
    }

    if (query.next()) {
        qDebug() << "아이디 중복";
        return "중복된 아이디입니다.●";
    }

    // 이메일 중복 확인 쿼리
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", email);
    if (!query.exec()) {
        qDebug() << "이메일 중복 확인 쿼리 실행 오류:" << query.lastError().text();
            return "회원가입 실패: 중복 확인 오류";
    }

    if (query.next()) {
        qDebug() << "이메일 중복";
        return "중복된 이메일입니다.●";
    }

    // 핸드폰 번호 중복 확인 쿼리
    query.prepare("SELECT * FROM users WHERE Phone_number = :phone_number");
    query.bindValue(":phone_number", phone_number);
    if (!query.exec()) {
        qDebug() << "핸드폰 번호 중복 확인 쿼리 실행 오류:" << query.lastError().text();
        return "회원가입 실패: 중복 확인 오류";
    }

    if (query.next()) {
        qDebug() << "핸드폰 번호 중복";
        return "중복된 핸드폰 번호입니다.●";
    }

    // 중복이 없을 경우 회원가입 실행
    query.prepare("INSERT INTO users (name, password, rname, Phone_number, email, logged_in) VALUES (:username, :password, :rname, :phone_number, :email, 0)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":rname", realname);
    query.bindValue(":phone_number", phone_number);
    query.bindValue(":email", email);
    if (!query.exec()) {
        qDebug() << "회원가입 쿼리 실행 오류:" << query.lastError().text();
            return "회원가입 실패: 데이터베이스 오류";
    }

    return "회원가입 완료●";
}

bool Widget::performIdfind(QString email,QTcpSocket *clientSocket)
{
    QSqlQuery query;

    query.prepare("SELECT name FROM users WHERE email = :email");
    query.bindValue(":email", email);

    if (!query.exec())
    {
        qDebug() << "Error: Failed to execute query.";
        return false;
    }

    if (query.next())
    {
        QString foundUsername = query.value(0).toString();
        QString response = "아이디는 :●" + foundUsername;
        clientSocket->write(response.toUtf8());
        return true; // ID 찾기 성공
    }
    else
    {
        QString response = "이메일이 일치하지 않습니다.●";
        clientSocket->write(response.toUtf8());
        return false; // ID 찾기 실패
    }
}

bool Widget::performPwfind(QString username, QString phone_number, QTcpSocket *clientSocket)
{
    QSqlQuery query;

    query.prepare("SELECT password FROM users WHERE name = :username AND Phone_number = :phone_number ");
    query.bindValue(":username", username);
    query.bindValue(":phone_number", phone_number);

    if (!query.exec())
    {
        qDebug() << "Error: Failed to execute query.";
        return false;
    }

    if (query.next())
    {
        QString foundUserpassword = query.value(0).toString();
        QString response = "비밀번호는 :●" + foundUserpassword;
        clientSocket->write(response.toUtf8());
        return true;
    }
    else
    {
        QString response = "아이디 또는 전화번호가 일치하지 않습니다.●";
        clientSocket->write(response.toUtf8());
        return false;
    }
}

bool Widget::performIdduplicate(QString username,QTcpSocket *clientSocket)
{
    QSqlQuery query;
    query.prepare("SELECT name FROM users WHERE name = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        qDebug() << "아이디 중복 확인 쿼리 실행 오류:" << query.lastError().text();
            QString response = "중복 확인 중 오류 발생";
        clientSocket->write(response.toUtf8());
        return false;
    }

    if (query.next()) {
        qDebug() << "아이디 중복";
        QString response = "중복된 아이디입니다.●";
        clientSocket->write(response.toUtf8());
        return true;
    } else {
        QString response = "사용가능한 아이디입니다.●";
        clientSocket->write(response.toUtf8());
        return false;
    }
}

//performPHduplicate(pn,clientSocket)

bool Widget::performPHduplicate(QString pn,QTcpSocket *clientSocket)
{
    QSqlQuery query;
    query.prepare("SELECT Phone_number FROM users WHERE Phone_number = :usernumber");
    query.bindValue(":usernumber", pn);
    if (!query.exec()) {
        qDebug() << "폰번호 중복 확인 쿼리 실행 오류:" << query.lastError().text();
            QString response = "중복 확인 중 오류 발생";
        clientSocket->write(response.toUtf8());
        return false;
    }

    if (query.next()) {
        qDebug() << "핸드폰 중복";
        QString response = "중복된 핸드폰 번호입니다.●";
        clientSocket->write(response.toUtf8());
        return true;
    } else {
        QString response = "사용가능한 전화번호입니다.●";
        clientSocket->write(response.toUtf8());
        return false;
    }
}


//performMLduplicate(mail,clientSocket);
bool Widget::performMLduplicate(QString mail,QTcpSocket *clientSocket)
{
    QSqlQuery query;
    query.prepare("SELECT email FROM users WHERE email = :usermail");
    query.bindValue(":usermail", mail);
    if (!query.exec()) {
        qDebug() << "이멜 중복 확인 쿼리 실행 오류:" << query.lastError().text();
            QString response = "중복 확인 중 오류 발생";
        clientSocket->write(response.toUtf8());
        return false;
    }

    if (query.next()) {
        qDebug() << "이멜 중복";
        QString response = "중복된 이메일입니다.●";
        clientSocket->write(response.toUtf8());
        return true;
    } else {
        QString response = "사용가능한 이메일입니다.●";
        clientSocket->write(response.toUtf8());
        return false;
    }
}

bool Widget::performLogout(const QString &username, QTcpSocket *clientSocket)
{
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE users SET logged_in = 0 WHERE name = :username");
    updateQuery.bindValue(":username", username);
    if (!updateQuery.exec()) {
        qDebug() << "Error: 로그아웃 상태 업데이트 실패.";
        // 필요한 경우 에러 처리
        return false;
    }
    return true; // 로그아웃 성공
}
