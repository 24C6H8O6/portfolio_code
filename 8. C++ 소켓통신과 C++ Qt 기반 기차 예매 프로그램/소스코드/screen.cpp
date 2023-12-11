#include "screen.h"
#include "ui_screen.h"
#include "widget.h"
#include "book.h"
#include "ui_widget.h"

Screen::Screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Screen)
{
    ui->setupUi(this);

    connect(ui->bokrailbtn, SIGNAL(clicked()), this, SLOT(showwidget()));
}

void Screen::showwidget()
{
    widget = new Widget(this);
    widget->setScreenInstance(this);
    if(ID.isEmpty() && PW.isEmpty()) {
        widget->ui->autologin->setVisible(false);
        widget->ui->autologinimg->setVisible(false);
    } else {
        widget->ui->autologin->setVisible(true);
        widget->ui->autologinimg->setVisible(true);
    }
    connect(this, &Screen::gowidget, widget, &Widget::getsignal);

    QFrame *screenFrame = findChild<QFrame *>("Screen");
    if (screenFrame) {
        screenFrame->hide();
    }
    emit gowidget(ID, PW, departure, arrival);
    disconnect(this, &Screen::gowidget, widget, &Widget::getsignal);
}

void Screen::getuserinfo(const QString &id, const QString &password, const QString &dep, const QString &arv)
{
    ID = id;
    PW = password;
    departure = dep;
    arrival = arv;
    this->show();
}

Screen::~Screen()
{
    delete ui;
}
