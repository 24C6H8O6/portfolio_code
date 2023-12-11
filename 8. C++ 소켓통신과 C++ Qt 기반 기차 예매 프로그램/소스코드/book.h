#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QComboBox>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTcpSocket>
#include <QRadioButton>
#include <QScrollArea>
#include <QDate>

class map;
class Widget;
class Screen;
class trainList;

QT_BEGIN_NAMESPACE
namespace Ui { class Book; }
QT_END_NAMESPACE

class Book : public QWidget
{
    Q_OBJECT

public:
    Book(QWidget *parent = nullptr);
    void setWidgetInstance(Widget* widget) { this->parentWidget = widget; }
    void setScreenInstance(Screen* screen) { this->screenWidget = screen; }
    ~Book();
    void getTcpSocket(QTcpSocket *tcpsocket);
    QHostAddress getMyIP();
    void setmenubuttons(int idx);
    void changeButtonStyle_1();
    void changeButtonStyle_2();
    void changeButtonStyle_3();

    void setDateButton_1();
    void setDateButton_2();
    void setDateButton_3();

    void showTimeScroll_1();
    void showTimeScroll_2();
    void showTimeScroll_3();

    void changehourButton_1();
    void changehourButton_2();
    void changehourButton_3();

    void setSearchTrainButton();

    void setCustomNum_1();
    void setCustomNum_2();

    void setUnderButton();
    void setCalender(int chk);

    void updateCalendar();
    void prevMonth();
    void nextMonth();
    void checkButtonEnable();

private:
    Widget* parentWidget;
    Screen* screenWidget;
    trainList *trainlist;
    map *map_1;


    bool chkconnect;
    void initialize();
    QTcpSocket *tcpSocket;
    QHostAddress myAddress;

    Ui::Book *ui;


    // map class에서 정보 받기
    QString mapdeparture;
    QString maparrival;
    int stackedidx;

    // 달력관련 변수
    QString selectDate;

    QPushButton *goCalendar_1;
    QPushButton *goCalendar_2;
    QPushButton *goCalendar_3;

    QPushButton *prevButton;
    QPushButton *nextButton;
    QWidget *newWidget;
    QDate currentDate_1;
    QLabel *labelMonthYear;
    QVector<QVector<QPushButton *>> calendarButtons;
    QVBoxLayout *layout;

    // 승객수 표시
    QLabel *customnum_1;
    QLabel *customnum_2;

    // 시간설정 스크롤 area
    QScrollArea *booktime_1;
    QScrollArea *booktime_2;
    QScrollArea *booktime_3;

    // 날짜 버튼
    QVector<QRadioButton *> radiobuttons_1;
    QVector<int> dayIdx_1;
    QVector<QRadioButton *> radiobuttons_2;
    QVector<int> dayIdx_2;
    QVector<QRadioButton *> radiobuttons_3;
    QVector<int> dayIdx_3;

    // 시간 버튼
    QVector<QRadioButton *> radiobuttons_4;
    QVector<QRadioButton *> radiobuttons_5;
    QVector<QRadioButton *> radiobuttons_6;

    QLabel *date_1;
    QLabel *date_2;
    QLabel *date_3;

    QString station1 = "광주송정";
    QString station2 = "서울";
    QString datetime_1;
    QString datetime_2;
    QString datetime_3;

     // 승객수
    int num = 1;
    int adultnum = num;
    int childnum=0;
    int eldernum=0;

    QVBoxLayout *mainLayout_1;
    QVBoxLayout *mainLayout_2;

    QFrame *chooseArea_1;
    QFrame *chooseArea_4;

    QPushButton *downwidget_1;
    QPushButton *downwidget_2;

    QPushButton *departure;
    QPushButton *arrival;

    QPushButton *departure2;
    QPushButton *arrival2;
//    QPushButton *toggleButton_1;
    QPushButton *toggleButton_2;
    QPushButton *toggleButton_3;
//    QPushButton *toggleButton_4;
    QPushButton *toggleButton_5;
    QPushButton *toggleButton_6;
    QPushButton *toggleButton_7;

    QVBoxLayout *widgetLayout_1;
    QVBoxLayout *widgetLayout_2;
    QVBoxLayout *widgetLayout_3;
    QVBoxLayout *widgetLayout_4;
    QVBoxLayout *widgetLayout_5;
    QVBoxLayout *widgetLayout_6;
    QVBoxLayout *widgetLayout_7;

    QVBoxLayout *widgetLayout_8;

    QWidget *widgetContainer_1;
    QWidget *widgetContainer_2;
    QWidget *widgetContainer_3;
    QWidget *widgetContainer_4;
    QWidget *widgetContainer_5;
    QWidget *widgetContainer_6;
    QWidget *widgetContainer_7;

    QWidget *widgetContainer_8;

    QPropertyAnimation *animation;

//    QLabel *arrow_1;
    QLabel *arrow_2;
    QLabel *arrow_3;
//    QLabel *arrow_4;
    QLabel *arrow_5;
    QLabel *arrow_6;
    QLabel *arrow_7;

//    QPixmap arrowimg_1;
    QPixmap arrowimg_2;
    QPixmap arrowimg_3;
//    QPixmap arrowimg_4;
    QPixmap arrowimg_5;
    QPixmap arrowimg_6;
    QPixmap arrowimg_7;

    QVector<QString> station = {"서울", "수원", "천안아산", "대전", "서대구", "동대구", "울산", "부산", "용산", "광명", "익산", "광주송정", "목포"};
    QVector<QString> dayofweek = {"월", "화", "수", "목", "금", "토", "일"};

    // 자동 로그인 정보
    QString ID;
    QString PW;
    QString NAME;
    QString HP;
    QString EM;

signals:
    void gotrainlist(const QString &message);
    void gomap();
    void logout();
    void sendInfo(const QString &id, const QString &password, const QString &dep, const QString &arv);

public slots:
    void getstations(const QString &dep, const QString &arv, const int &idx);
    void getsignal(const QString &id, const QString &password, const QString &name, const QString &dep, const QString &arv);

private slots:
    bool eventFilter(QObject *obj, QEvent *event);
    void toggleExpandingWidget();
    void switchscreen();
    void showmyinfo();
    void showmap();
    void showscreen();
    void showtrainlist();
    void showlogin();

};
#endif // BOOK_H
