// login1.h(헤더파일)
#ifndef LOGIN1_H_
#define LOGIN1_H_
#include <string>
#include <vector>

using namespace std;
// 구조체
struct Login
{
    string id;
    string pw;
    string email;
    string phone;
    int unique;
};

// 로그인 클래스
class LOgin
{
private:
    string ID;     // 로그인 시 아이디
    string PW;     // 로그인 시 비밀번호
    string Email;  // 회원가입 이메일
    string phone;  // 회원가입 휴대폰번호
    Login p;       // 파일입출력을 위한 구조체
    int flag4 = 0; // 승차권 확인 변수

public:
    int login2(Login login, Login print);                     // 로그인
    int account2(Login account, Login print);                 // 회원가입
    void unique(Login &account);                              // 회원번호 생성 및 저장
    int findid(Login account, Login print);                   // 아이디 찾기
    int findpw(Login account, Login print);                   // 비밀번호 찾기
    void print_account2();                                    // 회원가입 출력문
    void print_bokrail();                                     // 복레일 시작 출력문
    void choice_login();                                      // 초기메뉴
    void totallogin(Login login, Login print, Login account); // 로그인 항목 선택
};

// 예매 클래스
class Ticketing
{
private:
    string var;                  // 로그인 클래스와 연결하기 위한 값
    int ticket;                  // 1.편도, 2.왕복
    string depart;               // 출발지
    string arrive;               // 도착지
    int curr_year;               // 년
    int curr_month;              // 월
    int curr_day;                // 일
    int curr_hour;               // 출발 시간
    int new_curr_hour;           // 출발 시간을 새롭게 변수로 지정
    int curr_minute;             // 분
    int normal_charge;           // 일반실 요금
    int special_charge;          // 특실 요금
    string charge_num;           // 일반실 선택 번호
    int charge;                  // 요금
    vector<vector<string>> seat; // 2차원 벡터(좌석)석
    int row;                     // 행
    string column;               // 열
    int column2 = 0;             // 열을 새로운 변수로 지정
    int flag3;                   // while루프 관련 변수
    int ticket1;                 // 승차권 확인 변수

public:
    void input();
    void choice();
    void times();
    int get_curr_hour() const;
    void seats();
    int choice_seat();
    void payment();
    void tickets();
    void arrive_depart();
    void normal_spec();
    int ticket_confirm();
    void reset_seat();
};

#endif