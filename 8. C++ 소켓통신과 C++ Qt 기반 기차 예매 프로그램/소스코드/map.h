#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class map; }
QT_END_NAMESPACE

class Book;
class Widget;

class map : public QWidget
{
    Q_OBJECT

public:
    map(QWidget *parent = nullptr);
    ~map();
    void getway(int idx);
    void setWidgetInstance(Widget* widget) { this->parentWidget = widget; }
    void getuserinfo(QString id, QString password, QString name, QString phonenum, QString email);
    void getTcpSocket(QTcpSocket *tcpsocket);
    QHostAddress getMyIP();
    bool eventFilter(QObject *obj, QEvent *event);

private:
    bool chkconnect;
    QTcpSocket *tcpSocket;
    QHostAddress myAddress;
    Ui::map *ui;

    Widget *parentWidget;
    Book *book;

    int bookstackedidx;
    QVector<QString> gyungbustations = {"서울", "수원", "천안아산", "대전", "서대구", "동대구", "울산", "부산"};
    QVector<QString> honamstations = {"서울", "용산", "광명", "천안아산", "익산", "광주송정", "목포"};

    QAbstractButton *lastStartButton = nullptr;
    QAbstractButton *lastEndButton = nullptr;
    QString lastClickedObjectNumber="50";
    QString start;
    QString end;

    bool gobookchk_1=false;
    bool gobookchk_2=false;

    QVBoxLayout *widgetLayout_8;
    QWidget *widgetContainer_8;

    QPropertyAnimation *animation;

    QString ID;
    QString PW;
    QString NAME;
    QString HP;
    QString EM;

signals:
    void sendInfo(const QString& departure, const QString& arrival, const int &idx);
    void gobook();
    void logout();

private slots:
    void showlogin();
    void showmyinfo();
    void changeScreen();
    void gyungbu_button_clicked();
    void honam_button_clicked();
    void MapStartButton(QAbstractButton *button);
    void showbook();
    void getsignal();
    void backbook();

};
#endif // MAP_H
