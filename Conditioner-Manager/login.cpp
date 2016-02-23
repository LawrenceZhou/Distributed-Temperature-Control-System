#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle("登录界面");
    setFixedSize(160,160);
    ui->roomnumber->setRange(1,30);
}

Login::~Login()
{
    delete ui;
}

void Login::on_cancel_clicked()
{
    close();
}

void Login::on_confirm_clicked()
{
    roomnumber = ui->roomnumber->value();
    accept();
}

int Login::getRoomNumber()
{
    return roomnumber;
}
