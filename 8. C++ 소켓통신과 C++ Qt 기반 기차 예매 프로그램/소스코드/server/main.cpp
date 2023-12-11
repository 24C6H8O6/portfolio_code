#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server w;  // e1. 위젯 객체 생성
    w.setWindowTitle("TCP 서버");
    w.show();

    return a.exec();
}
