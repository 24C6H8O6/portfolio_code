#ifndef LOGIN_H
#define LOGINT_H

#include <QWidget>
#include <QTcpSocket>

class Screen;
class Widget;
class Join;
class Find;
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    void setWidgetInstance(Widget* widget) { this->parentWidget = widget; }
    void setScreenInstance(Screen *screen) { this->screenWidget = screen; }
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    QHostAddress getMyIP();
    Ui::Login *ui;
    void getTcpSocket(QTcpSocket *tcpsocket);

private:
    QString ID;
    QString PW;
    QString departure;
    QString arrival;

    bool chkconnect;
    Screen *screenWidget;
    Widget *parentWidget;
    Login *login;
    Join *join;
    Find *find;
    void initialize();
    QTcpSocket *tcpSocket;
    QHostAddress myAddress;

signals:
    void gojoin();
    void gofind();

public slots:
    void getsignal(const QString &id, const QString &password, const QString &dep, const QString &arv);

private slots:
    void sendMessage(const QString &message);
    void sendButton();
    void showjoin();
    void showfind();
    void disconnectButton();
//    void serverDisconnected();
};

#endif // LOGIN_H
