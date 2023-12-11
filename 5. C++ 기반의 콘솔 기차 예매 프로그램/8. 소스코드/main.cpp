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

int main()
{
    while (1)
    {
        int ticketflag = 0;
        Ticketing aa;
        LOgin mylogin;
        int num = 0;
        int num2 = 0;
        string num4;
        // 유일한 번호 변수
        int unique;
        // 로그인 시 필요한 값
        int ret;
        // 로그인 구조체
        Login login;
        // 로그인할 때 파일 출력하는 구조체
        Login print;
        // 회원가입 구조체
        Login account;
        // 회원가입 화면
        mylogin.print_bokrail();
        mylogin.totallogin(login, print, account);
        while (!ticketflag)
        {
            aa.input();
            int b = aa.get_curr_hour();
            aa.seats();
            aa.payment();
            ticketflag = aa.ticket_confirm();
        }
        cout << "승차권을 확인하시겠습니까?(y/n) ";
        cin >> num4;
        if (num4 == "y")
        {
            sleep(2);
            system("clear");
            aa.tickets();
        }
        else if (num4 == "n")
        {
            cout << "ok!!" << endl;
        }
    }

    return 0;
}