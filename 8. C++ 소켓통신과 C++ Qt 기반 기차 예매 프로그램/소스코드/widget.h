#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
#include <QPushButton>
#include <QGroupBox>

class Join;
class Find;
class Login;
class Book;
class Screen;
class map;
class trainList;
class trainTime;
class trainFeePage;
class Seat;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void setScreenInstance(Screen *screen) { this->parentWidget = screen; }
    QHostAddress getMyIP();
    void getinfo(const QString &id, const QString &password, const QString &dep, const QString &arv);
    Ui::Widget *ui;
    void train_list();
    void setData();
    void receiveData();
    void backlogin();

    struct trainInfo{
        QString trainNum;
        QString tripway;
        QString station1;
        QString station2;
        QString deptime_year;
        QString deptime_month;
        QString deptime_day;
        int deptime_hour;
        int deptime_min;
        QString yoil;
        QString dur_time;
        int price;
        int adult_seat;
        int child_seat;
        int elder_seat;
    };
    trainInfo traininfo;
    trainInfo traininfo_arr;
    trainInfo traininfo_dep;
    // setData 관련 벡터
    QVector<QString> stt_vector1 = {"SEOUL","SUWON","CHEONANASAN","DAEJEON","SEODAEGU","DONGDAEGU","ULSAN","BUSAN"};
    QVector<QString> stt_vector2 = {"SEOUL","YONGSAN","GWANGMYEONG","CHEONANASAN","IKSAN","GWANGJUSONGJEONG","MOKPO"};
    QVector<QString> now_stt_vector;
    QVector<QString> stop_time_vector;

private:
    void initialize();
    trainList *trainlist;
    trainTime *traintime;
    trainFeePage *trainfeepage;
    map *map_1;
    Book *book;
    Login *login;
    Join *join;
    Find *find;
    Seat *seat;
    Screen* parentWidget;
    QTcpSocket *tcpSocket;
    QHostAddress myAddress;

    QTimer *timer;
    QMovie *movie;

    QString ID;
    QString PW;
    QString NAME;
    QString departure;
    QString arrival;

    // train_list 함수 내 버튼2 작동 필요
    QString trainNum;
    QString depStation;
    QString arrStation;
    QString depTime;
    QString arrTime;
    int trainFee;
    int norm_fee;
    int spec_fee;
    int num;
    // 테두리 관련해서 쓰는 벡터
    QVector<QPushButton *> btnVector1;
    QVector<QPushButton *> btnVector2;
    QVector<QGroupBox *> gbVector;
    // 예매 관련(왕복 선택 시 사용)
    int flag = 0;
    // 나이별 인원수

signals:
    void gologin(const QString &id, const QString &password, const QString &dep, const QString &arv);
    void gojoin();
    void goseat(const int &custom);
    void logout();
    void gofind();
    void gotrainlist();
    void gobook(const QString &id, const QString &password, const QString &name, const QString &dep, const QString &arv);
    void gomap();
    void sendInfo(const QString &id, const QString &password, const QString &dep, const QString &arv);

public slots:
    void getuserinfo(const QString &id, const QString &password, const QString &dep, const QString &arv);
    void getsignal(const QString &id, const QString &password, const QString &dep, const QString &arv);
    void showseat(const int &num);

private slots:
    void readMessage();
    void connectserver();
    void showlogin();
    void showjoin();
    void showfind();
    void showbook();
    void showtrainlist(const QString &message);
//    void logout();
};

#endif // WIDGET_H
