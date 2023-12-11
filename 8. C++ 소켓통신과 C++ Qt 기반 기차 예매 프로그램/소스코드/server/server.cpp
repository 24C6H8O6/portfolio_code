#include "server.h"
#include "ui_server.h"

#include <QtWidgets>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

// e2. 위젯 생성자 실행
Server::Server(QWidget *parent) : QWidget(parent), ui(new Ui::Server)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/lms20/바탕화면/QTtask/BokRail2/server/login.db");
    QSqlQuery query;
    bool isOpendDB = db.open();
    if (isOpendDB) {
        qDebug() << "Database opened successfully.";

        // 테이블 생성 쿼리
        query.prepare("CREATE TABLE IF NOT EXISTS INFO (ID TEXT, NAME TEXT, NICK TEXT, PW TEXT, HP TEXT, EMAIL TEXT, FRIEND TEXT, CHAT TEXT)");

        if (!query.exec()) {
            qDebug() << "Error: Failed to create table.";
            qDebug() << "Error detail:" << query.lastError().text();
        } else {
            qDebug() << "Table creat successfully.";
        }
    } else {
        qDebug() << "Failed to open database.";
    }
    // e3. 통신을 위한 initialize() 실행
    initialize();
}

void Server::initialize()
{
    // e4. hostAddress 찾기
    // (이 소스코드가 실행될 프로그램(서버)의 ip 찾기)
    ipAddressesList = QNetworkInterface::allAddresses();
    // localhost(127.0.0.1) 가 아닌 것을 사용
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
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
    if (!tcpServer->listen(hostAddress, 25000)) {
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
    connect(ui->disconnectButton, SIGNAL(clicked()),
            this,              SLOT(disconnectButton()));
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection2()));

    ui->connMsgEdit->clear();
}

void Server::newConnection2()
{
    // e7. tcpServer 소켓에 클라이언트 소켓 정보 결합
    // [4] 수락 : tcpServer 소켓 + 연결 요청시 전달 받은 클라이언트 정보를 기반
    //           새로운 연결 QTcpSocket 생성
    QTcpSocket *connectedSocket = tcpServer->nextPendingConnection();
    connectedSockets.insert(connectedSocket);
    connect(connectedSocket, SIGNAL(readyRead()),
            this, SLOT(readMessage()));
    connect(connectedSocket, SIGNAL(disconnected()),
            this, SLOT(clientDisconnected()));
    // 서버에 연결 정보 출력
    QString clnt_addr = connectedSocket->peerAddress().toString();
    QString currTime = QTime::currentTime().toString("hh시 mm분 ss초 zzz밀리초");
    QString text_conn = QString("<-클라이언트 연결 성공 (%1, %2)").arg(currTime, clnt_addr);
    ui->connMsgEdit->append(text_conn);

}

void Server::readMessage()
{
    QTcpSocket *connectedSocket = static_cast<QTcpSocket *>(sender());

    // 읽어들인 데이터를 QByteArray에 저장
    QByteArray readData = connectedSocket->readAll();

    // QByteArray를 QString으로 변환하여 UI에 출력
    QString message = QString::fromUtf8(readData);
    QStringList loginfo = message.split("/");
    QString id, name, nick, pw, hp, em;
    QSqlQuery query;
    QString serverMessage;
    qDebug() << loginfo;
    if (message.startsWith("login: ")) {
        message = message.remove("login: ");
        QStringList loginfo = message.split(",");
        id = loginfo.at(0);
        pw = loginfo.at(1);
        if (userloginfo.values().contains(id)) {
            serverMessage = id + ": 이미 접속 중인 ID입니다";
        } else {
            query.prepare("SELECT NAME FROM INFO WHERE ID = :ID AND PW = :PW");
            query.bindValue(":ID", id);
            query.bindValue(":PW", pw);

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                return;
            }
            if (query.next()) {
                QString username = query.value(0).toString();
                serverMessage = "로그인 성공: " + id + "," + pw + "," + username;
                yourlogin = false;
                QString bufferMessage;
                userloginfo.insert(connectedSocket, id);
                if (buffermsg.contains(id)) {
                    for (const QByteArray& message : buffermsg[id]) {
                        bufferMessage = bufferMessage + QString::fromUtf8(message) + "$";
                    }
                    connectedSocket->write(bufferMessage.toUtf8());
                    buffermsg[id].clear();
                }
            }else if(id == "" || pw == "") {
                serverMessage = "아이디나 비밀번호를\n입력해주세요";
            }else {
                serverMessage = "아이디나 비밀번호가\n다릅니다";
            }
        }
    } else if(message.startsWith("out: ")){
        if(userloginfo.contains(connectedSocket)) {
            userloginfo.remove(connectedSocket);
        }
    } else if (loginfo.size() == 6) {
        id = loginfo.at(0);
        name = loginfo.at(1);
        nick = loginfo.at(2);
        pw = loginfo.at(3);
        hp = loginfo.at(4);
        em = loginfo.at(5);

        qDebug() << id;
        qDebug() << name;
        qDebug() << nick;
        qDebug() << pw;
        qDebug() << hp;
        qDebug() << em;

        QSqlDatabase::database().transaction();
        query.prepare("INSERT INTO INFO (ID, NAME, NICK, PW, HP, EMAIL) VALUES(:ID, :NAME, :NICK, :PW, :HP, :EMAIL)");
        query.bindValue(":ID", id);
        query.bindValue(":NAME", name);
        query.bindValue(":NICK", nick);
        query.bindValue(":PW", pw);
        query.bindValue(":HP", hp);
        query.bindValue(":EMAIL", em);
        QSqlDatabase::database().commit();

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            return;
        }
        serverMessage = "회원가입 완료";

    } else if (loginfo.size() == 1)  {
        if (message.startsWith("ID: ")) {
            id = message.remove("ID: ");

            query.prepare("SELECT * FROM INFO WHERE ID = :ID");
            query.bindValue(":ID", id);

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                return;
            }

            if (query.next()) {
                serverMessage = "이미 존재하는 ID입니다.";
            }else {
                serverMessage = "사용 가능한 ID입니다";
            }

        } else if(message.startsWith("HP: ")) {
            hp = message.remove("HP: ");

            query.prepare("SELECT * FROM INFO WHERE HP = :HP");
            query.bindValue(":HP", hp);

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                return;
            }

            if (query.next()) {
                serverMessage = "이미 가입한 HP입니다";
            }else {
                serverMessage = "사용 가능한 HP입니다";
            }

        } else if(message.startsWith("EM: ")) {
            em = message.remove("EM: ");

            query.prepare("SELECT * FROM INFO WHERE EMAIL = :EM");
            query.bindValue(":EM", em);

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                return;
            }

            if (query.next()) {
                serverMessage = "이미 가입한 Email입니다";
            }else {
                serverMessage = "사용 가능한 Email입니다";
            }
        } else if(message.contains("로그아웃 요청")) {
            id = message.remove("로그아웃 요청");
            qDebug() << "왔니?" << id;
            disconnectUser(id);
        } else if(message.contains("로그아웃")) {
            if(userloginfo.contains(connectedSocket)) {
                userloginfo.remove(connectedSocket);
            }
        } else if(message.startsWith("chatone: ")) {
            findchat1();
        } else if(message.startsWith("chatgroup: ")) {
            findchatgroup();
        } else if(message.startsWith("findfriend: ")) {
            // 수정부분

            QString myid;
            QString friends;
            if(userloginfo.contains(connectedSocket)) {
                myid = userloginfo[connectedSocket];
            }
            message = message.remove("findfriend: ");
            query.prepare("SELECT FRIEND FROM INFO WHERE ID = :ID");
            query.bindValue(":ID", myid);
            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                return;
            }
            if (query.next()) {
                friends = query.value(0).toString();
            }


            query.prepare("SELECT ID, NAME, NICK FROM INFO");
            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                return;
            }
            while (query.next()) {
                QString userID = query.value(0).toString();
                QString userName = query.value(1).toString();
                QString userNick = query.value(2).toString();
                if(!friends.contains(userID)){
                    if (userName == message || userNick == message) {
                        id += "," +userID;
                        name += "," + userName;
                        nick += "," + userNick;
                    }
                }
            }
            id.remove(0, 1);
            name.remove(0, 1);
            nick.remove(0, 1);
            if(!id.isEmpty()) {
                serverMessage = "findfriend: " + id + "@" + name+ "@" + nick;
            }

        } else if(message.startsWith("addfriend: ")) {
            QString myid;
            if(userloginfo.contains(connectedSocket)) {
                myid = userloginfo[connectedSocket];
            }
            QString userFriend;
            query.prepare("SELECT FRIEND FROM INFO WHERE ID = :ID");
            query.bindValue(":ID", myid);
            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                return;
            }

            if (query.next()) {
                userFriend = query.value(0).toString();
            }

            query.prepare("SELECT ID, NAME, NICK FROM INFO");
            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                return;
            }
            while (query.next()) {
                QString userID = query.value(0).toString();
                QString userName = query.value(1).toString();
                QString userNick = query.value(2).toString();

                if (userID != myid && !userFriend.contains(userID)) {
                    id += "," +userID;
                    name += "," + userName;
                    nick += "," + userNick;
                }
            }
            id.remove(0, 1);
            name.remove(0, 1);
            nick.remove(0, 1);
            if(!id.isEmpty()) {
                serverMessage = "addfriend: " + id + "@" + name+ "@" + nick;
            }
            qDebug() << serverMessage;
        } else if(message.startsWith("plusfriend: ")) {
            if(userloginfo.contains(connectedSocket)) {
                id = userloginfo[connectedSocket];
            }
            message = message.remove("plusfriend: ");
            QSqlDatabase::database().transaction();

            query.prepare("UPDATE INFO SET FRIEND = FRIEND || :FRIEND WHERE ID = :ID AND FRIEND IS NOT NULL");
            query.bindValue(":ID", id);
            query.bindValue(":FRIEND", "," + message);

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                QSqlDatabase::database().rollback();
                return;
            }

            query.prepare("UPDATE INFO SET FRIEND = FRIEND || :FRIEND WHERE ID = :ID AND FRIEND IS NOT NULL");
            query.bindValue(":ID", message);
            query.bindValue(":FRIEND", "," + id);

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                QSqlDatabase::database().rollback();
                return;
            }

            query.prepare("UPDATE INFO SET FRIEND = :FRIEND WHERE ID = :ID AND FRIEND IS NULL");
            query.bindValue(":ID", id);
            query.bindValue(":FRIEND", message);

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                QSqlDatabase::database().rollback();
                return;
            }

            query.prepare("UPDATE INFO SET FRIEND = :FRIEND WHERE ID = :ID AND FRIEND IS NULL");
            query.bindValue(":ID", message);
            query.bindValue(":FRIEND", id);

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                QSqlDatabase::database().rollback();
                return;
            }

            QSqlDatabase::database().commit();
            serverMessage = "plusfriend: " + id;
        } else if(message.startsWith("creat: ")) {
            message = message.remove("creat: ");
            Check_existroom(message);
        } else if(message.startsWith("creatgroup: ")) {
            message = message.remove("creatgroup: ");
            QString myid = userloginfo[connectedSocket];
            message = myid + "," + message;
            QStringList UserList = message.split(",");
            if(UserList.size()<3) {
                Check_existroom(message);
            } else {
                make_grouproom(message);
            }
        } else if(message.startsWith("내정보아이디: ")) {
            QString myid = message.remove("내정보아이디: ");
            QString mypw;
            query.prepare("SELECT PW FROM INFO WHERE ID = :ID");
            query.bindValue(":ID", myid);
            if(query.exec())
            {
                while(query.next())
                {
                    mypw = query.value("PW").toString();
                }
            }
            serverMessage = "^친구창갱신";
            kind_friend(myid, mypw);
            qDebug() << myid << " 내 비밀번호는 " << mypw;
        }  else if(message.startsWith("친구온라인: ")) {
            QString yourid = message.remove("친구온라인: ");
            QString yourpw;
            query.prepare("SELECT PW FROM INFO WHERE ID = :ID");
            query.bindValue(":ID", yourid);
            if(query.exec())
            {
                while(query.next())
                {
                    yourpw = query.value("PW").toString();
                }
            }
            serverMessage = "^친구창갱신";
            kind_friend(yourid, yourpw);
            qDebug() << yourid << " 내 비밀번호는 " << yourpw;
        } else if(message.startsWith("chatroom_")) {
            QString myid;
            QString chat;
            if(userloginfo.contains(connectedSocket)) {
                myid = userloginfo[connectedSocket];
            }
            int colonIndex = message.indexOf(":"); // ":" 문자의 위치를 찾음

            if (colonIndex != -1) {
                roomname = message.left(colonIndex); // ":" 이전 부분
                chat = message.mid(colonIndex + 1); // ":" 이후 부분
                qDebug() << "Roomname:" << roomname;
                qDebug() << "Chat:" << chat;
            }

            query.prepare("SELECT ID, CHAT FROM INFO WHERE CHAT IS NOT NULL");

            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                QSqlDatabase::database().rollback();
                return;
            }

            while(query.next()) {
                QString yourid = query.value(0).toString();
                QString yourrooms = query.value(1).toString();
                QStringList yourRoomList = yourrooms.split(",");
                if(yourRoomList.contains(roomname)) {
                    QString sendMessage;
                    sendMessage = "chat: " + chat;
                    for (QTcpSocket* socket : userloginfo.keys()) {
                        if (socket != connectedSocket && userloginfo.value(socket) == yourid) {
                            socket->write(sendMessage.toUtf8());
                            break;
                        }
                    }
                }
            }
        }
    }

    // 읽은 후 상대방에게 전송
    QByteArray messageData = serverMessage.toUtf8();  // 메시지를 미리 바이트 배열로 변환
    if (connectedSocket->state() == QAbstractSocket::ConnectedState) {
        if(serverMessage.startsWith("plusfriend: ")) {
            for (QTcpSocket* socket : userloginfo.keys()) {
                if (userloginfo.value(socket) == message) {
                    socket->write(messageData);
                    break;
                }
            }
        } else {
            connectedSocket->write(messageData);
        }
    }
}

void Server::disconnectButton()
{
    for (QTcpSocket *socket : connectedSockets) {
        if (socket->state() == QAbstractSocket::ConnectedState) {
            socket->disconnectFromHost();
        }
    }
    userloginfo.clear();
}

void Server::disconnectUser(QString id)
{
    for (QTcpSocket* socket : userloginfo.keys()) {
        if (userloginfo.value(socket) == id) {
            socket->disconnectFromHost();
            userloginfo.remove(socket);

            QString text_close = QString("->클라이언트 접속 종료 (ID: %1)").arg(id);
            ui->connMsgEdit->append(text_close);
            break;
        }
    }
}

void Server::clientDisconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    connectedSockets.remove(socket);
    QString text_close = QString("->클라이언트 접속 종료");
    ui->connMsgEdit->append(text_close);
}

Server::~Server()
{
    delete ui;
}

void Server::kind_friend(QString ID, QString PW)
{
    QTcpSocket *connectedSocket = static_cast<QTcpSocket*>(sender());

    QString friend_id;
    QString friend_name;
    QString friend_nic;
    QString friend_kind;
    QStringList FList;
    QSqlQuery sql;
    sql.prepare("SELECT * FROM INFO WHERE ID = :ID AND PW = :PW");
    sql.bindValue(":ID", ID);
    sql.bindValue(":PW", PW);
    if(sql.exec())
    {
        while(sql.next())
        {
            QString name = sql.value("NAME").toString();
            QString nic = sql.value("NICK").toString();

            friend_name += name;
            friend_nic += nic;

            QString Friend = sql.value("FRIEND").toString();
            FList = Friend.split(",");
        }
    }

    friend_id = ID;
    for(int i=0; i<FList.size(); i++)
    {
        sql.prepare("SELECT * FROM INFO WHERE ID = :ID");
        sql.bindValue(":ID", FList[i]);
        if(sql.exec())
        {
            while(sql.next())
            {
                friend_id += "," + FList[i];
                QString name = sql.value("NAME").toString();
                QString nic = sql.value("NICK").toString();
                if(userloginfo.values().contains(FList[i]))  // 로그인시
                {
                    friend_name += "," + name + "IN";
                    friend_nic += "," + nic + "IN";
                }
                else if(!userloginfo.values().contains(FList[i])) // 로그오프시
                {
                    friend_name += "," + name + "OUT";
                    friend_nic += "," + nic + "OUT";
                }
            }
        }
        if(!yourlogin) {
            if(userloginfo.values().contains(FList[i])) {
                for (QTcpSocket* socket : userloginfo.keys()) {
                    if (userloginfo[socket] == FList[i]) {
                        QString updatemessage = "친구온라인: "+FList[i];
                        socket->write(updatemessage.toUtf8());
                        break;
                    }
                }
            }
            yourlogin = true;
        }
    }

    friend_kind = friend_id + "@" + friend_name + "@" + friend_nic;
    qDebug() << friend_kind;

    connectedSocket->write(friend_kind.toUtf8());
}

void Server::Check_existroom(QString message)
{
    QTcpSocket *connectedSocket = static_cast<QTcpSocket*>(sender());
    QString serverMessage;
    QSqlQuery query;
    QStringList UserList = message.split(",");
    QString myid = userloginfo[connectedSocket];
    QString chatnames;
    QStringList chatlist;
    QString urchatnames;
    QStringList urchatlist;
    QStringList yourlist;
    bool chkexist = false;

    qDebug() << UserList.at(1);

    query.prepare("SELECT CHAT FROM INFO WHERE ID = :ID AND CHAT IS NOT NULL");
    query.bindValue(":ID", UserList.at(1));

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
        QSqlDatabase::database().rollback();
        return;
    }

    if(query.next()) {

        urchatnames = query.value(0).toString();
        urchatlist = urchatnames.split(",");
    }

    query.prepare("SELECT CHAT FROM INFO WHERE ID = :ID AND CHAT IS NOT NULL");
    query.bindValue(":ID", myid);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
        QSqlDatabase::database().rollback();
        return;
    }

    if(query.next()) {

        chatnames = query.value(0).toString();
        chatlist = chatnames.split(",");

        for (QString &chatname : chatlist) {
            int cnt=0;
            query.prepare("SELECT ID, CHAT FROM INFO WHERE ID != :ID AND CHAT IS NOT NULL");
            query.bindValue(":ID", myid);
            if (!query.exec()) {
                qDebug() << "Error: Failed to execute query";
                QSqlDatabase::database().rollback();
                return;
            }

            QSet<QString> uniqueUsers;  // 저장된 모든 사용자의 ID를 저장할 QSet
            while(query.next()) {
                QString yourchats = query.value(1).toString();
                yourlist = yourchats.split(",");
                for(QString &temp : yourlist) {
                    if(temp == chatname) {
                        cnt++;
                        uniqueUsers.insert(query.value(0).toString()); // 사용자 ID를 QSet에 추가
                    }
                }
            }

            // uniqueUsers에 2명만 있고, 그 두 명이 myid와 UserList.at(1)인 경우에만
            // existroom 메시지를 발송
            if(cnt==1 && uniqueUsers.contains(UserList.at(1))) {
                roomname = chatname;
                serverMessage = "existroom: " + roomname + "," +UserList.at(0) + "," + UserList.at(1);
                qDebug() << serverMessage;
                qDebug() << UserList;
                chkexist = true;
                break;
            }
        }
    } else {

        roomname = QString ("chatroom_%1").arg(roomnum);
        roomnum++;
        QSqlDatabase::database().transaction();

        query.prepare("UPDATE INFO SET CHAT = CHAT || :CHAT WHERE ID = :ID AND CHAT IS NOT NULL");
        query.bindValue(":ID", UserList.at(0));
        query.bindValue(":CHAT", "," + roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        query.prepare("UPDATE INFO SET CHAT = CHAT || :CHAT WHERE ID = :ID AND CHAT IS NOT NULL");
        query.bindValue(":ID", UserList.at(1));
        query.bindValue(":CHAT", "," + roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        query.prepare("UPDATE INFO SET CHAT = :CHAT WHERE ID = :ID AND CHAT IS NULL");
        query.bindValue(":ID", UserList.at(0));
        query.bindValue(":CHAT", roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        query.prepare("UPDATE INFO SET CHAT = :CHAT WHERE ID = :ID AND CHAT IS NULL");
        query.bindValue(":ID", UserList.at(1));
        query.bindValue(":CHAT", roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        QSqlDatabase::database().commit();
        serverMessage = "creatroom: " + roomname + "," +UserList.at(0) + "," + UserList.at(1);
        qDebug() << serverMessage;
        qDebug() << UserList;
        chkexist = true;
    }
    if(!chkexist) {
        roomname = QString ("chatroom_%1").arg(roomnum);
        roomnum++;
        QSqlDatabase::database().transaction();

        query.prepare("UPDATE INFO SET CHAT = CHAT || :CHAT WHERE ID = :ID AND CHAT IS NOT NULL");
        query.bindValue(":ID", UserList.at(0));
        query.bindValue(":CHAT", "," + roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        query.prepare("UPDATE INFO SET CHAT = CHAT || :CHAT WHERE ID = :ID AND CHAT IS NOT NULL");
        query.bindValue(":ID", UserList.at(1));
        query.bindValue(":CHAT", "," + roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        query.prepare("UPDATE INFO SET CHAT = :CHAT WHERE ID = :ID AND CHAT IS NULL");
        query.bindValue(":ID", UserList.at(0));
        query.bindValue(":CHAT", roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        query.prepare("UPDATE INFO SET CHAT = :CHAT WHERE ID = :ID AND CHAT IS NULL");
        query.bindValue(":ID", UserList.at(1));
        query.bindValue(":CHAT", roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        QSqlDatabase::database().commit();
        serverMessage = "creatroom: " + roomname + "," +UserList.at(0) + "," + UserList.at(1);
        qDebug() << serverMessage;
        qDebug() << UserList;
    }

    if(!userloginfo.values().contains(UserList.at(1))){
        if(serverMessage.startsWith("creatroom: ")){
            buffermsg[UserList.at(1)].append(serverMessage.toUtf8());
        }
        connectedSocket->write(serverMessage.toUtf8());
    } else {
        for (QTcpSocket* socket : userloginfo.keys()) {
            if (userloginfo[socket] == UserList.at(0)) {
                socket->write(serverMessage.toUtf8());
            }  else {
                QString yourmessage = "on" + serverMessage;
                socket->write(yourmessage.toUtf8());
            }
        }
    }
}

void Server::make_grouproom(QString message)
{
    QTcpSocket *connectedSocket = static_cast<QTcpSocket*>(sender());
    QString serverMessage;
    QSqlQuery query;
    QStringList UserList = message.split(",");

    qDebug() << UserList;

    roomname = QString ("chatroom_%1").arg(roomnum);
    roomnum++;
    QSqlDatabase::database().transaction();

    for(int i=0; i<UserList.size(); i++) {

        query.prepare("UPDATE INFO SET CHAT = CHAT || :CHAT WHERE ID = :ID AND CHAT IS NOT NULL");
        query.bindValue(":ID", UserList.at(i));
        query.bindValue(":CHAT", "," + roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        query.prepare("UPDATE INFO SET CHAT = :CHAT WHERE ID = :ID AND CHAT IS NULL");
        query.bindValue(":ID", UserList.at(i));
        query.bindValue(":CHAT", roomname);

        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }
    }

    QSqlDatabase::database().commit();

    serverMessage = "creatgrouproom: " + roomname;
    for(int i=0; i<UserList.size(); i++) {
        serverMessage += "," +UserList.at(i);
    }
    qDebug() << serverMessage;
    qDebug() << UserList;

    for(int i=1; i<UserList.size(); i++) {
        if(!userloginfo.values().contains(UserList.at(i))){
            buffermsg[UserList.at(i)].append(serverMessage.toUtf8());
        } else {
            for (QTcpSocket* socket : userloginfo.keys()) {
                if (userloginfo[socket] == UserList.at(i)) {
                    socket->write(serverMessage.toUtf8());
                }
            }
        }
    }
    connectedSocket->write(serverMessage.toUtf8());
}

void Server::findchat1()
{
    QTcpSocket *connectedSocket = static_cast<QTcpSocket*>(sender());
    QString myid = userloginfo[connectedSocket];

    QString serverMessage;
    QString chatnames;
    QString usernames;
    QStringList chatlist;
    QSqlQuery query;

    query.prepare("SELECT CHAT FROM INFO WHERE ID = :ID AND CHAT IS NOT NULL");
    query.bindValue(":ID", myid);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
        QSqlDatabase::database().rollback();
        return;
    }

    if(query.next()) {
        chatnames = query.value(0).toString();
        chatlist = chatnames.split(",");
    }
    for (QString chatname : chatlist) {
        query.prepare("SELECT ID, CHAT FROM INFO WHERE ID != :ID AND CHAT IS NOT NULL");
        query.bindValue(":ID", myid);
        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        while(query.next()) {
            QString yourchats = query.value(1).toString();
            QStringList yourlist = yourchats.split(",");
            for(QString temp : yourlist) {
                if(temp == chatname) {
                    usernames += query.value(0).toString() + ",";
                }
            }
        }
        usernames.chop(1);
        usernames += "@";
    }
    usernames.chop(1);
    qDebug() << usernames;
    QStringList users = usernames.split("@");
    qDebug() << users.size();
    if(!usernames.isEmpty()){
        for(int i =0; i<users.size(); i++ ) {
            QStringList usernum = users.at(i).split(",");
            if(usernum.size()<2) {
                serverMessage += chatlist.at(i) + "@" +users.at(i) + "/";
            }
        }
        serverMessage.chop(1);
    }

    qDebug() << serverMessage;
    if (connectedSocket->state() == QAbstractSocket::ConnectedState) {
        connectedSocket->write(serverMessage.toUtf8());
    }
}

void Server::findchatgroup()
{
    QTcpSocket *connectedSocket = static_cast<QTcpSocket*>(sender());
    QString myid = userloginfo[connectedSocket];

    QString serverMessage = "group: ";
    QString chatnames;
    QString usernames;
    QStringList chatlist;
    QSqlQuery query;

    query.prepare("SELECT CHAT FROM INFO WHERE ID = :ID AND CHAT IS NOT NULL");
    query.bindValue(":ID", myid);

    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query";
        QSqlDatabase::database().rollback();
        return;
    }

    if(query.next()) {
        chatnames = query.value(0).toString();
        chatlist = chatnames.split(",");
    }
    for (QString chatname : chatlist) {
        query.prepare("SELECT ID, CHAT FROM INFO WHERE ID != :ID AND CHAT IS NOT NULL");
        query.bindValue(":ID", myid);
        if (!query.exec()) {
            qDebug() << "Error: Failed to execute query";
            QSqlDatabase::database().rollback();
            return;
        }

        while(query.next()) {
            QString yourchats = query.value(1).toString();
            QStringList yourlist = yourchats.split(",");
            for(QString temp : yourlist) {
                if(temp == chatname) {
                    usernames += query.value(0).toString() + ",";
                }
            }
        }
        usernames.chop(1);
        usernames += "@";
    }
    usernames.chop(1);
    qDebug() << usernames;
    QStringList users = usernames.split("@");
    qDebug() << users.size();
    if(!usernames.isEmpty()){
        for(int i =0; i<users.size(); i++ ) {
            QStringList usernum = users.at(i).split(",");
            if(usernum.size()>1) {
                serverMessage += chatlist.at(i) + "@" +users.at(i) + "/";
            }
        }
        serverMessage.chop(1);
    }

    qDebug() << serverMessage;
    if (connectedSocket->state() == QAbstractSocket::ConnectedState) {
        connectedSocket->write(serverMessage.toUtf8());
    }
}
