#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui { class Widget; }

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    QHostAddress getMyIP();

private:
    Ui::Widget *ui;
    void initialize();
    QTcpSocket *tcpSocket;
    QString serverIP;
    QString serverPort;
    QString loginID;
    QString loginPW;
    // 클라이언트에 최초로 불러오는 아이디, 닉네임 리스트
    QVector<QString> idList;
    QVector<QString> nickList;

    // 수정 필요(닉네임 값 들어가면 없앨 것)
    QString loginNICK;

    // 선택한 유저의 리스트가 들어가는 벡터
    QVector<QString> userlist;
    int num = 0;
    int num_user = 0;

    // 채팅방 리스트가 들어가는 벡터
    QVector<QString> chatroomlist;
    QString chatroom;
    int num_chat = 0;
    QVector<QString> roomlist;

    // 0907 추가된 부분(유저가 만드는 채팅방 제목)
    QString chattitle;

    // 0907 추가된 부분(유저가 선택하여 들어갈 채팅방 제목)
    QString real_chatRoomTitle;
    QVector<QString> real_members;

    // 회원가입 flag
    int joinid_flag = 0;
    int joinemail_flag = 0;
    int joinphone_flag = 0;
    bool isIdValid1 = false;
    bool isPwValid1 = false;
    bool isEmailValid1 = false;
    bool isPhoneValid1 = false;


private slots:
    void connectButton();
    void readMessage(); // 서버로부터 메세지를 받을 때 호출 됨
    void disconnected();

    void on_idfindButton_clicked();
    void on_pwfindButton_clicked();
    // 버튼에 슬롯 재설정 필요
    void on_login_pushButton_clicked();
    void on_pushButton_checknick_clicked();
    void f_endstr(QString &);
    void on_pushButton_makechatroom_clicked();
    void on_tabWidget_tabBarClicked(int index);
    void on_join_pushButton_clicked();
    void on_pushButton_6_clicked();
    void on_back_2_clicked();
    void on_back_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_return_findid_clicked();
    void on_return_findpw_clicked();
    void on_pushButton_9_clicked();
    void showchat();
    void setChatroom();
    void on_pushButton_10_clicked();



    void on_pushButton_8_clicked();
    void on_pushButton_gochatroom_clicked();
    void on_pushButton_applytitle_clicked();
    void on_pushButton_cancel_room_clicked();
    void on_check_id_clicked();
    void on_check_mail_clicked();
    void on_check_phone_clicked();
    void on_backtohome_clicked();
    void on_pushButton_7_clicked();
    void keyPressEvent(QKeyEvent *event);
    void joinIdFlag();
    void joinEmailFlag();
    void joinPhoneFlag();
};
#endif // WIDGET_H
