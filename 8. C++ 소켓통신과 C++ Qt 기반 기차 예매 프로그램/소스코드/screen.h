#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>

class Book;
class Widget;

namespace Ui {
class Screen;
}

class Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Screen(QWidget *parent = nullptr);
    ~Screen();

private:
    QString ID;
    QString PW;
    QString departure;
    QString arrival;

    Ui::Screen *ui;
    Widget *widget;
    Book *book;

signals:
    void gowidget(const QString &id, const QString &password, const QString &dep, const QString &arv);

public slots:
    void getuserinfo(const QString &id, const QString &password, const QString &dep, const QString &arv);

private slots:
    void showwidget();
};

#endif // SCREEN_H
