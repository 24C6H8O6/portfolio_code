#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QtNetwork>
#include <QMap>

class QTcpServer;
class QNetworkSession;

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();
    void kind_friend(QString, QString);
    void Check_existroom(QString message);
    void make_grouproom(QString message);
    void findchat1();
    void findchatgroup();

private:
    QHostAddress hostAddress;
    QList<QHostAddress> ipAddressesList;
    Ui::Server *ui;
    void initialize();
    QTcpServer *tcpServer;
    QSet<QTcpSocket*> connectedSockets;
    QMap<QTcpSocket*, QString> userloginfo;
    QMap<QString, QVector<QByteArray>> buffermsg;
    QString roomname;
    bool yourlogin;
    int roomnum=0;



private slots:
    void disconnectUser(QString id);
    void newConnection2();
    void clientDisconnected();
    void readMessage();
//    void sendMessage(QTcpSocket *connectedSocket, const QString &message);
//    void sendButton();
    void disconnectButton();
};

#endif // SERVER_H
