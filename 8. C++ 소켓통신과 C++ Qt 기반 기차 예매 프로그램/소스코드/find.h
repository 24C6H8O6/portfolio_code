#ifndef FIND_H
#define FIND_H

#include <QWidget>
#include <QTcpSocket>
#include <QStackedWidget>

class Widget;
class Login;
namespace Ui {
class Find;
}

class Find : public QWidget
{
    Q_OBJECT

public:
    void setWidgetInstance(Widget* widget) { this->parentWidget = widget; }
    explicit Find(QWidget *parent = nullptr);
    ~Find();
    QHostAddress getMyIP();
    void getTcpSocket(QTcpSocket *tcpsocket);
    Ui::Find *ui;

private:
    Widget* parentWidget;
    void initialize();
    QTcpSocket *tcpSocket;
    bool chkconnect;
    QHostAddress myAddress;
    QStackedWidget *stackedPages;
    Login *login;

signals:
    void gologin();

private slots:
    void getsignal();
    void sendMessage(const QString &message);
    void sendButton();
    void changeButton();
    void setIDfind();
    void setPwfind();
    void validchk();
    bool IDchk(const QString &str);
    QString emailselect(int index);
    bool Specialchk(const QString &email);
    bool containsOnlyDigits(const QString &str);
    void showlogin();
//    void serverDisconnected();

};

#endif // FIND_H
