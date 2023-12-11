#include "server.h"
#include "ui_server.h"

#include <QCoreApplication>
#include <QSqlQuery>
#include <QtSql>
#include <QDebug>
#include <QSqlError>
#include <QtConcurrent/QtConcurrent>


Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);

    initialize();    // 서버 시작
    connectPython(); // 파이썬 서버에 연결
    dbopen();        // 데이터베이스 열기

    clientList.clear();
}

Server::~Server()
{
    if (clientList.size() > 0)
    {
        for (int i = 0; i < clientList.size(); i++)
        {
            clientList[i].socket->disconnectFromHost();
            clientList[i].socket->deleteLater();
        }
    }

    db.close();

    delete ui;
}

void Server::connectPython()
{
    socket1.connectToHost("10.10.21.112", 8080);
    if (socket1.waitForConnected()) // waitForConnected는 연결되면 true를 반환
    {
        // 연결 성공
        ui->pylabel->setText("파이썬 서버\n\n연결 완료");
        connect(&socket1, SIGNAL(readyRead()),
                this,      SLOT(readMessage1()));
    }
    else
        ui->pylabel->setText("파이썬 서버\n\n연결 실패");
}

void Server::dbopen()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("10.10.21.103");
    db.setPort(3307);
    db.setDatabaseName("ai_emotion_prj");
    db.setUserName("aidb");
    db.setPassword("0123");

    if (!db.open())
        ui->dblabel->setText("데이터베이스\n연결 실패\n" + db.lastError().text());
    else
        ui->dblabel->setText("데이터베이스\n\n연결 성공");

}

void Server::initialize()
{
    // 서버 ip 찾기
    QHostAddress hostAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

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

    tcpServer = new QTcpServer(this);

    if (!tcpServer->listen(QHostAddress::Any, 8888)) {
        QMessageBox::critical(this, tr("TCP Server"),
                              tr("서버를 시작할 수 없습니다. 에러메세지 : %1.")
                              .arg(tcpServer->errorString()));
        close();
        return; // 실패시 종료
    }

    // tcpServer 소켓이 정상적으로 생성되면, 서버 정보를 serverlabel에 표시
    ui->serverlabel->setText(tr("서버 동작 중 \n"
                                "IP : %1\n"
                                "PORT : %2")
                                 .arg(hostAddress.toString()).arg(tcpServer->serverPort()));

    // tcpServer 소켓에 클라이언트의 연결 요청 Signal 이 생기면, 동작할 Slot 연결
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::newConnection()
{
    QTcpSocket *connectedSocket = tcpServer->nextPendingConnection();

    if (!connectedSocket) {
        ui->connMsgEdit->append("New Connection: Connected socket is null.");
        return;
    }

    QHostAddress clientAddr = connectedSocket->peerAddress();
    quint16 clientPort = connectedSocket->peerPort();

    ClientInfo clientInfo;
    clientInfo.socket = connectedSocket;
    clientInfo.address = clientAddr;
    clientInfo.port = clientPort;

    clientList.append(clientInfo);

    connect(connectedSocket, &QTcpSocket::readyRead, this, &Server::readMessage);
    connect(connectedSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
    connect(connectedSocket, &QTcpSocket::disconnected, connectedSocket, &QObject::deleteLater);

    ui->connMsgEdit->append("클라이언트 접속: " + clientAddr.toString() +
                            ", 포트: " + QString::number(clientPort));
}

void Server::clientDisconnected()
{
    // clientList에서 연결이 끊긴 클라이언트 삭제
    int disconSocketNum;
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    for (int i = 0; i < clientList.size(); ++i)
    {
        if (socket == clientList[i].socket)
        {
            disconSocketNum = i;
            break;
        }
    }

    ui->connMsgEdit->append("클라이언트 접속 종료: " + clientList[disconSocketNum].address.toString() +
                            ", 포트: " + QString::number(clientList[disconSocketNum].port));

    clientList.removeAt(disconSocketNum);
}

void Server::readMessage() // 클라이언트에서 read
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    if (socket->bytesAvailable() >= 0)
    {
        QByteArray buffer = socket->readAll();
        QString bufferStr = QString::fromUtf8(buffer);
        QList<QString> msgData = bufferStr.split("●");

        if (msgData[0] == "LOGIN_REQUEST")            // 로그인 요청
            login(socket, msgData);

        else if (msgData[0] == "SIGN_REQUEST")        // 회원가입 요청
            sign(socket, msgData);

        else if (msgData[0] == "FIND_ID_REQUEST")     // 아이디 찾기 요청
            find_id(socket, msgData);

        else if (msgData[0] == "FIND_PW_REQUEST")     // 비밀번호 찾기 요청
            find_pw(socket, msgData);

        else if (msgData[0] == "CHECK_ID_REQUEST")    // 아이디 중복 확인 요청
            id_chk(socket, msgData);

        else if (msgData[0] == "CHECK_PHONE_REQUEST") // 핸드폰 중복 확인 요청
            phone_chk(socket, msgData);

        else if (msgData[0] == "TESTDATA")            // 검사 데이터 분석 요청
            socket1.write(buffer);
            // 파이썬 서버에 그대로 전송
            // TESTDATA, 아이디, 첫 번째 그림, 첫 번째 텍스트, ... 네 번째 그림, 네 번째 텍스트

        else if (msgData[0] == "VIEWRESULT")          // 특정 아이디의 검사 날짜 데이터 요청
            select_date(socket);

        else if (msgData[0] == "VIEWTIMERESULT")      // 검사 날짜와 일치하는 검사 결과 데이터 요청
            select_result(socket, msgData);

        else
            ui->connMsgEdit->append("메시지 인식 불가");

//        ui->connMsgEdit->append(buffer);  // 확인용
//        qDebug() << bufferStr;
    }
}

//파이썬에서 read
void Server::readMessage1()
{
    if (socket1.bytesAvailable() >= 0)
    {
        QByteArray buffer = socket1.readAll();
        QString bufferStr = QString::fromUtf8(buffer);
        QList<QString> msgData = bufferStr.split("●");

        if (msgData[0] == "test_result")  // 감정 분석 결과 데이터를 수신
            result_handling(msgData);

        else
            ui->connMsgEdit->append("파이썬 서버 메시지 인식 불가");

//        ui->connMsgEdit->append("py server: " + buffer);  // 확인용
//        qDebug() << "py server: " << bufferStr;
    }
}

void Server::login(QTcpSocket* socket, QList<QString> msgData)
{
    QSqlQuery qry;
    qry.prepare("SELECT id, pw FROM user_info WHERE id=:id AND pw=:pw");
    qry.bindValue(":id", msgData[1]);
    qry.bindValue(":pw", msgData[2]);

    if (!qry.exec())
        QMessageBox::critical(this, "SQL ERROR", "데이터베이스 조회에 실패하였습니다.");

    if (!qry.next())
        socket->write("LOGIN_FAILURE●");
    else
    {
        QString msg = "LOGIN_SUCCESS●" + msgData[1] + "●"; // 로그인 성공, id 정보를 함께 전송
        socket->write(msg.toUtf8());

        // QList<ClientInfo> clientList의 id 정보를 업데이트
        for (int i = 0; i < clientList.size(); ++i)
        {
            if (socket == clientList[i].socket)
            {
                clientList[i].id = msgData[1];
                break;
            }
        }
    }
}

void Server::sign(QTcpSocket *socket, QList<QString> msgData)
{
    QSqlQuery qry;
    qry.prepare("INSERT INTO user_info (id, pw, name, sex, birth, phone) VALUES (:id, :pw, :name, :sex, :birth, :phone)");
    qry.bindValue(":id", msgData[1]);
    qry.bindValue(":pw", msgData[2]);
    qry.bindValue(":name", msgData[3]);
    qry.bindValue(":sex", msgData[4]);
    qry.bindValue(":birth", msgData[5]);
    qry.bindValue(":phone", msgData[6]);

    if (!qry.exec())
        socket->write("SIGN_FAILURE●");
    else
        socket->write("SIGN_SUCCESS●");
}

void Server::find_id(QTcpSocket *socket, QList<QString> msgData)
{
    QSqlQuery qry;
    qry.prepare("SELECT id FROM user_info WHERE name=:name AND phone=:phone");
    qry.bindValue(":name", msgData[1]);
    qry.bindValue(":phone", msgData[2]);

    if (!qry.exec())
        QMessageBox::critical(this, "SQL ERROR", "데이터베이스 조회에 실패하였습니다.");

    if (!qry.next())
        socket->write("ID_NOT_FOUND●");
    else
    {
        QString msg = "FOUND_ID●" + qry.value(0).toString() + "●";
        socket->write(msg.toUtf8());
    }
}

void Server::find_pw(QTcpSocket *socket, QList<QString> msgData)
{
    QSqlQuery qry;
    qry.prepare("SELECT pw FROM user_info WHERE id=:id AND name=:name AND phone=:phone");
    qry.bindValue(":id", msgData[1]);
    qry.bindValue(":name", msgData[2]);
    qry.bindValue(":phone", msgData[3]);

    if (!qry.exec())
        QMessageBox::critical(this, "SQL ERROR", "데이터베이스 조회에 실패하였습니다.");

    if (!qry.next())
        socket->write("PW_NOT_FOUND●");
    else
    {
        QString msg = "FOUND_PW●" + qry.value(0).toString() + "●";
        socket->write(msg.toUtf8());
    }
}

void Server::id_chk(QTcpSocket *socket, QList<QString> msgData)
{
    QSqlQuery qry;
    qry.prepare("SELECT id FROM user_info WHERE id=:id");
    qry.bindValue(":id", msgData[1]);

    if (!qry.exec())
        QMessageBox::critical(this, "SQL ERROR", "데이터베이스 조회에 실패하였습니다.");

    if (!qry.next())
        socket->write("CHECK_ID_RESPONSE●AVAILABLE●");
    else
        socket->write("CHECK_ID_RESPONSE●UNAVAILABLE●");
}

void Server::phone_chk(QTcpSocket *socket, QList<QString> msgData)
{
    QSqlQuery qry;
    qry.prepare("SELECT phone FROM user_info WHERE phone=:phone");
    qry.bindValue(":phone", msgData[1]);

    if (!qry.exec())
        QMessageBox::critical(this, "SQL ERROR", "데이터베이스 조회에 실패하였습니다.");

    if (!qry.next())
        socket->write("CHECK_PHONE_RESPONSE●VALID●");
    else
        socket->write("CHECK_PHONE_RESPONSE●INVALID●");
}

void Server::result_handling(QList<QString> msgData)
{
    // QList<ClientInfo> clientList의 id 정보로 소켓 찾기
    QTcpSocket *socket = nullptr;

    for (int i = 0; i < clientList.size(); ++i)
    {
        if (msgData[1] == clientList[i].id)
        {
            socket = clientList[i].socket;
            break;
        }
    }

    if (socket != nullptr)
    {
        QString msg = "TEST_RESULT●";

        // 아이디 정보를 제외한 나머지 정보를 전송
        for (int j = 2; j < msgData.size(); ++j)
        {
            msg.append(msgData[j]);
            msg.append("●");
        }

        socket->write(msg.toUtf8());
    }

    // 검사 결과 DB에 저장
    QSqlQuery qry;
    qry.prepare("INSERT INTO test_result (result_id, image_index, x_axis, y_axis, emotions, final_result, date_time) VALUES (:result_id, :image_index, :x_axis, :y_axis, :emotions, :final_result, :date_time)");
    qry.bindValue(":result_id", msgData[1]);
    qry.bindValue(":image_index", msgData[2]);
    qry.bindValue(":x_axis", msgData[3]);
    qry.bindValue(":y_axis", msgData[4]);
    qry.bindValue(":emotions", msgData[5]);
    qry.bindValue(":final_result", msgData[6]);
    qry.bindValue(":date_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if (!qry.exec())
        ui->connMsgEdit->append("검사 결과 데이터베이스 저장 실패");
    else
        ui->connMsgEdit->append("검사 결과 데이터베이스 저장 성공");
}

void Server::select_date(QTcpSocket *socket)
{
    // 아이디를 찾아 검사 날짜 조회
    // 소켓 정보로 아이디 찾기
    QString id = "";

    for (int i = 0; i < clientList.size(); ++i)
    {
        if (socket == clientList[i].socket)
        {
            id = clientList[i].id;
            break;
        }
    }

    if (id != "")
    {
        QSqlQuery qry;
        qry.prepare("SELECT date_time FROM test_result WHERE result_id=:id");
        qry.bindValue(":id", id);

        if (!qry.exec())
            qDebug() << "불러오기 실패";

        // 날짜 정보 저장할 컨테이너
        QStringList date_list;

        while(qry.next())
            date_list.append(qry.value(0).toString());

        // RESULT_DATES, 날짜 데이터 개수, 날짜 데이터를 전송
        QString msg = "RESULT_DATES●" + QString::number(date_list.size()) + "●";

        if (date_list.size() > 0)
            msg.append(date_list.join(","));

        socket->write(msg.toUtf8());
    }
}

void Server::select_result(QTcpSocket *socket, QList<QString> msgData)
{
    // 아이디, 날짜 정보로 검사 결과 데이터를 조회
    QSqlQuery qry;
    qry.prepare("SELECT image_index, x_axis, y_axis, emotions, final_result FROM test_result WHERE result_id=:id AND date_time=:time");
    qry.bindValue(":id", msgData[1]);
    qry.bindValue(":time", msgData[2]);

    if (!qry.exec())
        qDebug() << "불러오기 실패";

    // 검사 결과 정보 저장할 컨테이너
    QStringList result_list;

    if (qry.next())
    {
        result_list.append(qry.value(0).toString());
        result_list.append(qry.value(1).toString());
        result_list.append(qry.value(2).toString());
        result_list.append(qry.value(3).toString());
        result_list.append(qry.value(4).toString());
    }

    QString msg = "RESULT_LIST●";
    msg.append(result_list.join("●"));

    socket->write(msg.toUtf8());
}
