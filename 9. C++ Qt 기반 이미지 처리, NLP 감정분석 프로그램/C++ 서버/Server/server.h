#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class QTcpServer;
class QTcpSocket;

class Server : public QWidget
{
    Q_OBJECT

    struct ClientInfo {
        QTcpSocket *socket;
        QHostAddress address;
        quint16 port;
        QString id;
    };

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

private:
    Ui::Server *ui;

    // C++ 클라이언트 관련
    QTcpServer *tcpServer;
    void initialize();
    QList<ClientInfo> clientList;

    // 데이터베이스 관련
    QSqlDatabase db;
    void dbopen();

    // 파이썬 서버 관련
    void connectPython();
    QTcpSocket socket1;  //파이썬과 연결하는 소켓

    // C++ 클라이언트 메시지 처리 관련
    void login(QTcpSocket* socket, QList<QString> msgData);
    void sign(QTcpSocket* socket, QList<QString> msgData);
    void find_id(QTcpSocket* socket, QList<QString> msgData);
    void find_pw(QTcpSocket* socket, QList<QString> msgData);
    void id_chk(QTcpSocket* socket, QList<QString> msgData);        // 아이디 중복 확인
    void phone_chk(QTcpSocket* socket, QList<QString> msgData);     // 핸드폰 중복 확인
    void select_date(QTcpSocket* socket);                           // 감정 분석 결과의 날짜를 조회
    void select_result(QTcpSocket* socket, QList<QString> msgData); // 감정 분석 결과 조회

    // 파이썬 서버 메시지 처리 관련(검사 결과를 클라이언트에게 보내고 데이터베이스에 저장)
    void result_handling(QList<QString> msgData);

private slots:
    void newConnection();      // 새로운 클라이언트 연결 처리
    void clientDisconnected(); // 연결이 끊긴 클라이언트 처리
    void readMessage();        // 클라이언트에서 read
    void readMessage1();       // 파이썬에서 read
};

#endif // SERVER_H
