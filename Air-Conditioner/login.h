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
    QString getRoomNumber();

private slots:
    void on_confirm_clicked();

    void on_cancel_clicked();

private:
    Ui::Login *ui;
    QString roomNumber;
};


#endif // LOGIN_H
