#ifndef SEAT_H
#define SEAT_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QVector>
#include <QList>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QTcpSocket>

class Widget;
class trainList;

QT_BEGIN_NAMESPACE
namespace Ui { class Seat; }
QT_END_NAMESPACE

class Seat : public QWidget
{
    Q_OBJECT

public:
    Seat(QWidget *parent = nullptr);
    QComboBox *combobox;
    ~Seat();
    void setReservedSeat(int car);
    void setWidgetInstance(Widget* widget) { this->parentWidget = widget; }
    void getuserinfo(QString id, QString password, QString name, QString phonenum, QString email);
    void getTcpSocket(QTcpSocket *tcpsocket);
    QHostAddress getMyIP();
    void showmyinfo();
    bool eventFilter(QObject *obj, QEvent *event);

private:
    // 애니메이션 위젯
    bool chkconnect;
    QTcpSocket *tcpSocket;
    QHostAddress myAddress;

    Widget *parentWidget;
    trainList *trainlist;

    QVBoxLayout *widgetLayout_8;
    QWidget *widgetContainer_8;

    QPropertyAnimation *animation;

    QString ID;
    QString PW;
    QString NAME;
    QString HP;
    QString EM;

    Ui::Seat *ui;
    QVector<QVector<QString>> reservedSeats;
    int num;
    int chk;
    QList <QString> seatnum;

signals:
    void logout();

private slots:
    void getsignal(const int &custom);
    void getcomboboxtext(int index);
    void changeButtonStyle(QAbstractButton *button);
    void reserveComplete();
    void showlogin();
};
#endif // SEAT_H
