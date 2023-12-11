#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <ctime>
#include <vector>
#include <random>
#include "login1.h"
using namespace std;

// 로그인 클래스 내 함수
// 초기 출력문
void LOgin::print_bokrail()
{
    sleep(6);
    system("clear");
    cout << "┌⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┐" << endl;
    cout << "│            Welcome  to            │" << endl;
    cout << "│           Let's BokRail           │" << endl;
    cout << "└⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┘" << endl;
    sleep(3);
    system("clear");
}
// 항목 선택 출력문
void LOgin::choice_login()
{
    cout << "============================================" << endl;
    cout << endl;
    cout << "        원하시는 기능응 선택하세요" << endl;
    cout << endl;
    cout << "   1.로그인 2.회원가입 3.아이디찾기" << endl;
    cout << "  4.비밀번호찾기 5.승차권조회 6.종료" << endl;
    cout << endl;
    cout << "============================================" << endl;
}
// 회원가입 주의사항 출력문
void LOgin::print_account2()
{
    cout << "============================================" << endl;
    cout << "         << 회원가입 주의사항 >>" << endl;
    cout << "아이디, 이메일, 폰 번호 중 하나라도 중복되면" << endl;
    cout << "처음부터 다시 입력해야하니 신중을 기하십시오" << endl;
    cout << "============================================" << endl;
}
// 로그인 항목 선택 함수
void LOgin::totallogin(Login login, Login print, Login account)
{
    // 로그인,회원가입,찾기 기능 실행 시 필요
    int num3;
    Ticketing a;

    while (1)
    {
        num3 = 0;
        choice_login();
        cin >> num3;
        sleep(1);
        system("clear");
        sleep(1);
        if (num3 == 1)
        {
            // 로그인
            login2(login, print);
            break;
        }
        else if (num3 == 2)
        {
            // 회원가입

            account2(account, print);
        }
        else if (num3 == 3)
        {
            // 아이디 찾기
            findid(account, print);
        }
        else if (num3 == 4)
        {
            // 비밀번호 찾기
            findpw(account, print);
        }
        else if (num3 == 5)
        {
            if (flag4 == 1)
            { // 승차권 확인
                a.tickets();
            }
            else
            {
                cout << "승차권을 확인할 수 없습니다." << endl;
            }
        }
        else if (num3 == 6)
        {
            exit(0);
        }
        else
        {
            cout << "잘못입력됨" << endl;
        }
        sleep(1);
        system("clear");
    }
}
// 로그인 함수
int LOgin::login2(Login login, Login print)
{

    int num = 0;
    while (!num)
    {

        ifstream fin("/home/lms116/다운로드/miniproject2/test1/member.txt", ios_base::in);
        if (!fin.is_open())
        {
            cerr << "안 열림" << endl;
            return 1;
        }
        cin.ignore(256, '\n');
        cout << "┌⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┐" << endl;
        cout << "│                                          │" << endl;
        cout << "│                 Login Page               │" << endl;
        cout << "│                                          │" << endl;
        cout << "└⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼⎼┘" << endl;
        cout << "============================================" << endl;
        cout << "ID를 입력하세요 >> ";
        getline(cin, login.id);

        cout << "PW를 입력하세요 >> ";
        getline(cin, login.pw);
        string line;
        while (getline(fin, line))
        {
            istringstream iss(line);
            getline(iss, print.id, ',');
            getline(iss, print.pw, ',');
            getline(iss, print.email, ',');
            getline(iss, print.phone, ',');
            if ((login.id == print.id) && (login.pw == print.pw))
            {
                cout << "<< 로그인 성공 >>" << endl;
                ID = login.id;
                flag4 = 1;
                num = 1;
                sleep(1);
                system("clear");
                break;
            }
        }
        if (num == 0)
        {
            cout << "로그인실패, 다시 입력 요망!!" << endl;
            sleep(1);
            system("clear");
            continue;
        }

        fin.close();
    }
}
// 회원가입 함수
int LOgin::account2(Login account, Login print)
{
    sleep(1);
    int num2 = 0;
    while (!num2)
    {
        print_account2();
        int num3 = 0;
        int num4 = 0;
        // 문제 생기면 제거
        account.id = "0";
        // 유일한 번호 변수
        account.unique = 0;
        ifstream fin("/home/lms116/다운로드/miniproject2/test1/member.txt", ios_base::in);
        if (!fin.is_open())
        {
            cerr << "안 열림" << endl;
            return 1;
        }
        cout << "ID를 입력하세요 >> ";
        getline(cin, account.id);

        string line2;
        while (getline(fin, line2))
        {
            account.unique += 1;
            istringstream iss(line2);
            getline(iss, print.id, ',');
            getline(iss, print.pw, ',');
            getline(iss, print.email, ',');
            getline(iss, print.phone, ',');
            if (account.id == print.id)
            {
                num3 += 1;
            }
            else
            {
                num4 = 0;
            }
        }
        if (num3 == 0)
        {
            cout << "사용가능한 아이디" << endl;
            num2 = 1;
        }
        else if (num3 == 1)
        {
            cout << "아이디 중복, 다시 입력 요망!!" << endl;
            sleep(1);
            system("clear");
            continue;
        }

        cout << "PW를 입력하세요 >> ";
        getline(cin, account.pw);

        cout << "E-Mail을 입력하세요 >> ";
        getline(cin, account.email);
        num3 = 0;

        string line3;
        while (getline(fin, line3))
        {
            num2 = 0;
            istringstream iss(line3);
            getline(iss, print.id, ',');
            getline(iss, print.pw, ',');
            getline(iss, print.email, ',');
            getline(iss, print.phone, ',');
            if (account.email == print.email)
            {
                num3 += 1;
            }
            else
            {
                num4 = 0;
            }
        }
        if (num3 == 0)
        {
            cout << "사용가능한 이메일" << endl;
            num2 = 1;
        }
        else
        {
            cout << "이메일 중복, 다시 입력 요망!!" << endl;
            sleep(1);
            system("clear");
            continue;
        }

        cout << "휴대폰 번호를 입력하세요 >> ";
        getline(cin, account.phone);
        num3 = 0;
        string line4;
        while (getline(fin, line4))
        {
            num2 = 0;
            istringstream iss(line4);
            getline(iss, print.id, ',');
            getline(iss, print.pw, ',');
            getline(iss, print.email, ',');
            getline(iss, print.phone, ',');
            if (account.phone == print.phone)
            {
                num3 += 1;
            }
            else
            {
                num4 = 0;
            }
        }
        if (num3 == 0)
        {
            cout << "사용가능한 휴대폰 번호" << endl;
            num2 = 1;
        }
        else
        {
            cout << "휴대폰 번호 중복, 다시 입력 요망!!" << endl;
            sleep(1);
            system("clear");
            continue;
        }

        fin.close();
    }
    cout << "회원가입에 성공하셨습니다" << endl;
    // 회원번호 파일
    LOgin::unique(account);

    ofstream fout("/home/lms116/다운로드/miniproject2/test1/member.txt", ios_base::app);
    if (fout)
    {
        fout << account.id << "," << account.pw << "," << account.email << ","
             << account.phone << endl;
    }
    else
    {
        // 버퍼 지움
        cerr << "입력 안 됨" << endl;
        return 1;
    }
    fout.close();
}
// 회원 번호 함수
void LOgin::unique(Login &account)
{
    ofstream fout2("/home/lms116/다운로드/miniproject2/test1/unique.txt", ios_base::app);
    fout2 << account.id << "," << account.unique << endl;
    cout << "회원번호 " << account.unique << "이(가) 부여되었습니다" << endl;
    sleep(1);
    fout2.close();
}
// 아이디 찾기 함수
int LOgin::findid(Login account, Login print)
{
    int num5 = 0;
    while (!num5)
    {
        // int num3 = 0;
        int num4 = 0;
        ifstream fin("/home/lms116/다운로드/miniproject2/test1/member.txt", ios_base::in);
        if (!fin.is_open())
        {
            cerr << "안 열림" << endl;
            return 1;
        }
        cout << "이메일을 입력하세요 >> ";
        cin >> account.email;
        string line2;
        while (getline(fin, line2))
        {
            istringstream iss(line2);
            getline(iss, print.id, ',');
            getline(iss, print.pw, ',');
            getline(iss, print.email, ',');
            getline(iss, print.phone, ',');
            if (account.email == print.email)
            {
                cout << "아이디는 " << print.id << endl;
                num5 = 1;
                break;
            }
            else
            {
                num4 = 0;
            }
        }
        sleep(1);
        system("clear");
    }
}
// 비밀번호 찾기 함수
int LOgin::findpw(Login account, Login print)
{
    int num6 = 0;
    while (!num6)
    {
        int num7 = 0;
        ifstream fin("/home/lms116/다운로드/miniproject2/test1/member.txt", ios_base::in);
        if (!fin.is_open())
        {
            cerr << "안 열림" << endl;
            return 1;
        }
        cout << "이메일을 입력하세요 >> ";
        cin >> account.email;
        string line3;
        while (getline(fin, line3))
        {
            istringstream iss2(line3);
            getline(iss2, print.id, ',');
            getline(iss2, print.pw, ',');
            getline(iss2, print.email, ',');
            getline(iss2, print.phone, ',');
            if (account.email == print.email)
            {
                cout << "비밀번호는 " << print.pw << endl;
                num6 = 1;
                break;
            }
            else
            {
                num7 = 0;
            }
        }
        sleep(1);
        system("clear");
    }
}

// 예매 클래스 내 함수
// 2-1-1. 현재 시간 함수
void Ticketing::times()
{
    // 현재시간
    struct tm *curr_tm;
    time_t curr_time = time(nullptr);
    curr_tm = localtime(&curr_time);
    curr_year = curr_tm->tm_year;
    curr_month = curr_tm->tm_mon;
    curr_day = curr_tm->tm_mday;
    curr_hour = curr_tm->tm_hour;
    curr_minute = curr_tm->tm_min;
    cout << (curr_year - 100) << "년 " << curr_month << "월 " << curr_day << "일" << endl;
    cout << "현재시간 " << curr_hour << ":" << curr_minute << endl;
    new_curr_hour = curr_hour + 1;
}
// 2-1-2. 출발지에 원 도시 지정 함수
void Ticketing::arrive_depart()
{
    if (depart == "1")
    {
        depart = "서울";
    }
    else if (depart == "2")
    {
        depart = "대전";
    }
    else if (depart == "3")
    {
        depart = "광주";
    }
    else if (depart == "4")
    {
        depart = "대구";
    }
    else if (depart == "5")
    {
        depart = "울산";
    }
    else if (depart == "6")
    {
        depart = "부산";
    }
    // 도착지에 원 도시 지정
    if (arrive == "1")
    {
        arrive = "서울";
    }
    else if (arrive == "2")
    {
        arrive = "대전";
    }
    else if (arrive == "3")
    {
        arrive = "광주";
    }
    else if (arrive == "4")
    {
        arrive = "대구";
    }
    else if (arrive == "5")
    {
        arrive = "울산";
    }
    else if (arrive == "6")
    {
        arrive = "부산";
    }
}
// 2-1-3. 일반실, 특실 선택 함수
void Ticketing::normal_spec()
{
    while (1)
    {
        cout << "1.일반실\t2.특실" << endl;
        cout << "일반실과 특실 중 선택하세요 >> ";
        cin >> charge_num;
        if (charge_num == "1")
        {
            cout << "일반실을 선택하셨습니다" << endl;
            charge_num = "일반실";
            charge = normal_charge;
            break;
        }
        else if (charge_num == "2")
        {
            cout << "특실을 선택하셨습니다" << endl;
            charge_num = "특실";
            charge = special_charge;
            break;
        }
        else
        {
            cout << "다시 선택해주세요" << endl;
            continue;
        }
    }
    sleep(1);
    system("clear");
}
// 2-1. 선택 함수
void Ticketing::input()
{
    int num;
    times();
    // 편도 또는 왕복 출력문 넣을 것
    while (1)
    {
        cout << "1. 편도\t2.왕복" << endl;
        cout << "편도인지 왕복인지 고르시오 >> ";
        cin >> ticket;
        if (ticket == 1 || ticket == 2)
        {
            break;
        }
        else
        {
            cout << "다시 입력 요망" << endl;
            continue;
        }
    }
    sleep(1);
    system("clear");

    cout << "출발가능지역" << endl;
    cout << "1.서울,2.대전,3.광주,4.대구,5.울산,6.부산" << endl;
    while (1)
    {

        cout << "출발지 선택 >> ";
        cin >> depart; // 고쳐
        if (depart == "1" || depart == "2" || depart == "3" || depart == "4" || depart == "5" || depart == "6")
        {
            break;
        }
        else
        {
            cout << "다시 입력 요망" << endl;
            continue;
        }
    }
    while (1)
    {
        cout << "도착가능지역" << endl;
        cout << "1.서울,2.대전,3.광주,4.대구,5.울산,6.부산" << endl;
        cout << "도착지 선택 >> ";
        cin >> arrive;
        if (arrive == "1" || arrive == "2" || arrive == "3" || arrive == "4" || arrive == "5" || arrive == "6")
        {
            break;
        }
        else
        {
            cout << "다시 입력 요망" << endl;
            continue;
        }
    }

    int gap = abs(stoi(depart) - stoi(arrive));
    if (gap == 5)
    {
        gap = 4;
    }
    arrive_depart();
    sleep(1);
    system("clear");
    normal_charge = 15600 * gap;
    special_charge = 21830 * gap;
    cout << "번호   열차   출발지  도착지  출발시간  도착예정시간  일반실    특실" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << i << "\t"
             << "ktx   "
             << depart
             << "\t"
             << arrive
             << "\t" << (new_curr_hour + 2 * i) % 24
             << "시\t" << (new_curr_hour + gap + 2 * i) % 24 << "시\t      " << normal_charge
             << "원\t" << special_charge << "원" << endl;
    }

    cout << "번호를 선택하세요" << endl;
    cin >> num;
    curr_hour = (curr_hour + (2 * num) + 1) % 24;
    sleep(1);
    system("clear");
    // 일반실, 특실
    normal_spec();
}
// 2-2-1자리 선택
int Ticketing::choice_seat()
{
    int flag = 0;
    int flag2 = 0;
    char munja;
    while (!flag)
    {
        cout << "열을 입력해주세요(A,B,C,D) >> ";
        cin >> column;
        if (column == "A" || column == "B" || column == "C" || column == "D")
        {
            cout << column << "열을 선택하셨습니다." << endl;
        }
        else
        {
            cout << "잘못 입력하셨음.." << endl;
            continue;
        }
        cout << "행을 입력해주세요(1~14) >>";
        cin >> row;
        if (row >= 1 && row <= 14)
        {
            cout << row << "행을 선택하셨습니다." << endl;
        }
        else
        {
            cout << "잘못 입력하셨음.." << endl;
            continue;
        }
        // 자리 변환
        if (column == "A")
        {
            column2 = 1;
        }
        else if (column == "B")
        {
            column2 = 2;
        }
        else if (column == "C")
        {
            column2 = 4;
        }
        else if (column == "D")
        {
            column2 = 5;
        }
        // 자리가 빈자리가 아니면 선택할 수 없음을 알리기(나중에 할 것)
        if (seat[row - 1][column2] == "■")
        {
            cout << "이미 예약된 자리입니다, 다시 입력해주세요" << endl;
            continue;
        }
        while (!flag2)
        {
            cout << "선택하신 자리가 " << row << column << "이 맞습니까?(y,n)";
            cin >> munja;
            if (munja == 'y')
            {
                cout << "자리 선택이 완료되었습니다" << endl;
                flag = 1;
                flag2 = 1;
                system("clear");
            }
            else if (munja == 'n')
            {
                cout << "다시 선택해 주시기 바랍니다" << endl;
                break;
            }
            else
            {
                cout << "잘못된 선택입니다" << endl;
                cout << "다시 입력해주세요" << endl;
            }
        }
    }

    flag3 = 1;
    return flag3;
}
// 2-2. 2차원 벡터로 좌석 생성
void Ticketing::seats()
{

    int num1;
    int num2;
    seat = vector<vector<string>>(14, vector<string>(6, "0"));
    // 2차원 벡터 초기화
    seat.resize(14); // 행 크기 지정
    for (int i = 0; i < 14; i++)
    {
        seat[i].resize(6); // 열 크기 지정
        for (int j = 1; j < 6; j++)
        {
            if (j == 3)
            {
                seat[i][j] = " ";
            }
            else
            {
                seat[i][j] = "□";
            }
        }
    }
    for (int i = 0; i < 4; i++)
    {
        while (1)
        {
            num1 = (rand() % 13) + 1;
            num2 = (rand() % 5) + 1;
            if (num2 == 3)
                continue;
            else
                break;
        }
        seat[num1][num2] = "■";
    }

    // 2차원 벡터 출력 + 좌석 선택 함수(do~while문)
    flag3 = 0;
    do
    {
        cout << "No. A B   C D" << endl;
        int num = 1;
        for (int i = 0; i < 14; i++)
        {
            cout << num << " ";
            if (num < 10)
            {
                cout << " ";
            }
            for (int j = 1; j < 6; j++)
            {
                cout << seat[i][j] << " ";
            }
            cout << endl;
            num++;
        }
        if (flag3 == 0)
        {
            flag3 = choice_seat();
            seat[row - 1][column2] = "■";
        }
        flag3 += 1;
    } while (flag3 != 3);
}
// 2-3. 결재
void Ticketing::payment()
{
    int user_money;
    int dif_money;
    cout << "선택하신 승차권의 가격은 " << charge << "원입니다" << endl;
    dif_money = charge;

    while (1)
    {
        cout << dif_money << "원 입금하시길 바랍니다 >> ";
        cin >> user_money;
        if (dif_money == user_money)
        {
            cout << dif_money << "원 결재가 완료되었습니다" << endl;
            ticket1 = 1;
            sleep(2);
            system("clear");
            break;
        }
        else
        {
            cout << dif_money << "원을 정확히 입금해주시길 바랍니다" << endl;
        }
    }
    // 영수증
    cout << "============================================" << endl;
    cout << "\t\t영수증" << endl;
    cout << (curr_year - 100) << "년" << curr_month << "월" << curr_day << "일" << endl;
    cout << charge_num << "ㅣ" << row << column << endl;
    cout << depart << " " << curr_hour << ":00 > "
         << arrive << (curr_hour + 2) << ":00" << endl;
    cout << "결재금액 : " << charge << endl;
    cout << "=============================================" << endl;
    sleep(2);
    system("clear");
}
// 2-4. 승차권
void Ticketing::tickets()
{
    cout << "=============================================" << endl;
    cout << "\t\t  승차권" << endl;
    cout << endl;
    // cout << "            출발지  도착지" << endl;
    cout << "            " << depart << "발 " << arrive << "행 열차" << endl;
    cout << "            출발시간 : " << curr_hour << "시" << endl;
    cout << "            좌석 : " << charge_num << " " << row << "행 " << column << "열" << endl;
    cout << "            가격 : " << charge << "원" << endl;
    cout << "=============================================" << endl;
    sleep(5);
    system("clear");
}
// 승차권 확인을 위한 값을 전달하기 위한 함수
int Ticketing::ticket_confirm()
{
    int ticketflag = ticket1;
    return ticketflag;
}
// 현재 시간 전달 함수(보류)
int Ticketing::get_curr_hour() const
{
    return curr_hour;
}