#ifndef OPTION_H
#define OPTION_H

#include <QDialog>

namespace Ui {
class option;
}

class option : public QDialog
{
    Q_OBJECT

public:
    explicit option(QWidget *parent = nullptr);
    ~option();

private:
    Ui::option *ui;
};

#endif // OPTION_H
