#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    int getRoomNumber();

private slots:
    void on_cancel_clicked();

    void on_confirm_clicked();

private:
    Ui::Login *ui;
    int roomnumber;
};

#endif // LOGIN_H
