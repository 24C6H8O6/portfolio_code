#ifndef TRAINLIST_H
#define TRAINLIST_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QtNetwork>
#include <QGroupBox>
#include "traintime.h"
#include "trainfeepage.h"

class Widget;
class Seat;
class Book;

QT_BEGIN_NAMESPACE
namespace Ui { class trainList; }
QT_END_NAMESPACE


class trainList : public QWidget
{
    Q_OBJECT

public:
    void setWidgetInstance(Widget* widget) { this->parentWidget = widget; }
    void setBookInstance(Book *book) { this->bookWidget = book; }

    void getuserinfo(QString id, QString password, QString name, QString phonenum, QString email);

    void getTcpSocket(QTcpSocket *tcpsocket);
    QHostAddress getMyIP();

    Ui::trainList *ui;
    trainList(QWidget *parent = nullptr);
    ~trainList();
    void showseat();
    void getMessage(QString message);
    // 열차조회
    void train_list();
    // 열차 조회 페이지에서 금액 누를 시 나오는
    // 열차시각, 운임요금을 조회할 수 있는 페이지 열기
    void showTraininfo();
    // traintime(열차 시각) 클래스에 구조체로 데이터를 보내고
    // 열차 시각 페이지 열기
    void showTrainTime();
    // trainfeepage(운임 정보) 클래스에 열차번호, 일반실/특실 요금을 보내고
    // 운임 정보 페이지 띄움
    void showTrainFeePage();
    // 하단 메뉴바 관련 함수
    void setUnderButton();
    // traintime으로 넘길 구조체
    // (열차번호, 출발역, 도착역, 출발시간, 도착시간, 선택한 날짜정보가 담긴 벡터, 요일)
    struct traindata{
        QString trainNum;
        QString dep_stt;
        QString arr_stt;
        QString dep_time;
        QString arr_time;
        QVector<int> sel_date;
        QString yoil;
    };
    traindata trainData;
    // 총 인원수 정보 가져오기
    void getcustomNum(int num);


signals:
    void gobook();
    void goseat(int num);

private slots:
    void backbook();
    void getsignal();
    // 내정보 페이지
    void showMyinfo();
    // 범위 지정 후 애니메이션 사라지게 하는 함수
    bool eventFilter(QObject *obj, QEvent *event);
    // 이전날 버튼 눌렀을 때 이벤트(어제날짜 부터는 안 나오도록)
    void on_pushButton_preDate_clicked();
    // 다음날 버튼 누를 시 이벤트(오늘 이후 30일까지 가능하도록)
    void on_pushButton_nextDate_clicked();

private:
    // 총인원수
    int all_custom;


    QString dateMessage;

    bool chkconnect;
    void initialize();
    QTcpSocket *tcpSocket;
    QHostAddress myAddress;

    Widget *parentWidget;
    Book *bookWidget;
    // 각 클래스을 선언한 변수
    trainTime *traintime;
    trainFeePage *trainfeepage;
    Seat *seat;
    // 선택한 날짜,시간 담는 변수
    QDateTime currentDateTime;
    // 애니메이션 변수
    QPropertyAnimation *animation;
    // 메뉴 애니메이션
    QVBoxLayout *widgetLayout_ani;
    QWidget *widgetContainer_ani;
    // 금액 버튼 누를 시 밑에서 나오는 애니메이션
    QVBoxLayout *widgetLayout_ani2;
    QWidget *widgetContainer_ani2;
    // 애니메이션 범위 설정
    QRect containerRect2;
    // 금액 버튼 누를 시 선택한 버튼의 정보가 변수에 할당
    // (열차번호, 출발역, 도착역, 출발시간, 도착시간, 금액)
    QString trainNum;
    QString depStation;
    QString arrStation;
    QString depTime;
    QString arrTime;
    int trainFee;
    // 선택한 날짜에 대한 요일벡터(영), 요일벡터(한), 연, 월, 일, 시, 분, 요일
    QVector<QString> dow_eng;
    QVector<QString> dow_kor;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    QString yoil;
    // 이전날, 다음날 버튼 선택 시 요일 저장하는 변수
    QString dayOfWeek;
    // 현재 시간(시:분:초)
    QString nowTime;
    int day_dif=0;
    int bool_check;
//    QList<QGroupBox*> groupBoxList;
    // 운임 요금 페이지에 쓰일 정보
    int norm_fee;
    int spec_fee;
    int num;
    // 테두리 관련해서 쓰는 벡터
    QVector<QPushButton *> btnVector1;
    QVector<QPushButton *> btnVector2;
    QVector<QGroupBox *> gbVector;

    int flag = 0;

    QString ID, PW, NAME, HP, EM;

};
#endif // TRAINLIST_H
