#include "client.h"
#include <QWidget>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client w;
    w.setWindowTitle(" Emotion Check");
    w.show();
    return a.exec();
}
