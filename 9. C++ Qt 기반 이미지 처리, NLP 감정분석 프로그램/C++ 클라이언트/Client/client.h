#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QFileDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QStandardPaths>
//추가
#include <QScreen>
#include <QButtonGroup>
#include <QSqlDatabase>
#include <QRegularExpressionValidator>
#include <QMovie>
//#include "qcustomplot.h"
#include <QApplication>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    QHostAddress getMyIP();

    //추가
//    bool userdbOpen();





private:
    Ui::Client *ui;
    void initialize();
    QTcpSocket *tcpSocket;
    QString serverIP;
    QString serverPort;
    QButtonGroup *image_Btns;
    QList<QString> picturePathList; // 모든 이미지 경로 리스트
    QList<QString> pictureList; // 선택한 이미지 리스트

    //추가
//    QTcpSocket* clientSocket;

    QString message;

    int login_cnt = 0;
    QString login_id;
//    QList<QString> userlist;
    QButtonGroup *genderRadioGroup = new QButtonGroup(this);

    QSqlDatabase userdb;
    QMessageBox msgbox0;
    QMessageBox msgbox1;
    QMessageBox msgbox2;
    QMessageBox msgbox3;
    QMessageBox msgbox4;
    QMessageBox msgbox5;
    QMessageBox msgbox6;
    QMessageBox msgbox7;
    QMessageBox msgbox8;

    void btnStyleSheet();   // 버튼 스타일시트
    void drawStarOnLabel(int x, int y, int size, const QColor& color);
    void moveStar(double x, double y);

private slots:
    void connectButton();
    void readMessage(); // 서버로부터 메세지를 받을 때 호출 됨
    void disconnected();
    void findPath();
    void sendFile();

    //지호추가
    void centerOnScreen();
    void LoginPage();
    void SignPage();
    void FindPage();
    void Sign();

    void Signup();
    void CheckId();
    void CheckPw(const QString& text);
    void CheckBirth();
    void CheckName();
    void CheckPhone();
    void FindId();
    void FindPw();
    void Login();
//    void processRequest();

    void picturePick(QAbstractButton*); // 이미지를 선택했을 때 테두리 생성 및 리스트 추가
    void testStart();       // 선택한 이미지로 검사 시작
    void testBack();        // 검사 중 뒤로가기
    void send_testData();   // 설문 결과를 서버에 보내고 검토중 화면으로 이동
    void viewResult();      // 날짜별 설문 결과 페이지
    void BacktoMain();      // 설문결과,설문시작페이지로 가는버튼
    void BacktoMain2();     // 시간 나오는 페이지에서 설문 시작 페이지로 가는 버튼
};
#endif // CLIENT_H
