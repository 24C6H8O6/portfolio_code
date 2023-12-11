#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>

class QTcpServer;
class QTcpSocket;

namespace Ui { class Widget; }

class Widget : public QWidget
{
    Q_OBJECT

    struct ClientInfo {
        QTcpSocket *socket;
        QHostAddress address;
        quint16 port;
        QString userId;
        QString team;
        QString nickname;
    };

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    void initialize();
    void sendDataToClient(const QString &data, const QHostAddress &address, quint16 port);

    QTcpServer *tcpServer;
    QString clnt_addr;
    QByteArray requestData;
    QList<QTcpSocket*> clientSocketList;
    QList<ClientInfo> clientList;

    QVector<QString> names;
    QVector<QString> nicknames;

    bool performLogin(QString username, QString password, QTcpSocket *clientSocket);
    QString performSign(QString username, QString password, QString realname, QString phone_number, QString email);
    bool performIdfind(QString email,QTcpSocket *clientSocket);
    bool performPwfind(QString username, QString phone_number, QTcpSocket *clientSocket);
    bool performIdduplicate(QString username,QTcpSocket *clientSocket);
    bool performPHduplicate(QString pn,QTcpSocket *clientSocket);
    bool performMLduplicate(QString mail,QTcpSocket *clientSocket);
    bool performLogout(const QString &username, QTcpSocket *clientSocket);

private slots:
    void newConnection();
    void clientDisconnected();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void readMessage();
    void processRequest(const QByteArray &requestData, QTcpSocket *clientSocket);
};
#endif // WIDGET_H
