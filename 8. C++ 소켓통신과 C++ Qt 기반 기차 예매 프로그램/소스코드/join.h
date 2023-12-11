#ifndef JOIN_H
#define JOIN_H

#include <QWidget>
#include <QTcpSocket>

class Widget;
class Login;

namespace Ui {
class Join;
}

class Join : public QWidget
{
    Q_OBJECT

public:
    void setWidgetInstance(Widget* widget) { this->parentWidget = widget; }
    explicit Join(QWidget *parent = nullptr);
    ~Join();
    QHostAddress getMyIP();
    void getTcpSocket(QTcpSocket *tcpsocket);
    Ui::Join *ui;

private:
    Widget* parentWidget;
    int cnt=0;
    bool id_dupchk=false;
    bool hp_dupchk=false;
    bool em_dupchk=false;

    void initialize();
    QTcpSocket *tcpSocket;
    bool chkconnect;
    QHostAddress myAddress;
    Login *login;

signals:
    void gologin();

private slots:
    void getsignal();
    void sendMessage(const QString &message);
    void sendButton();
    void Reset();
    void dupliButton();
    void validchk();
    QString emailselect(int index);
    bool IDchk(const QString &str);
    bool hangulchk(ushort unicode);
    bool namechk(const QString &name);
    bool containsOnlyDigits(const QString &str);
    bool containsMixed(const QString &str);
    bool passwordchk();
    bool Specialchk(const QString &email);
    void disconnectButton();
//    void serverDisconnected();
    void showlogin();
};

#endif // JOIN_H
