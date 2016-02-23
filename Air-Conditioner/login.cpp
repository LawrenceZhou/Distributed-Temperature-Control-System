#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle("登录界面");
    setFixedSize(200,160);
    ui->roomnumber->addItem("1");
    ui->roomnumber->addItem("2");
    ui->roomnumber->addItem("3");
    ui->roomnumber->addItem("4");
    ui->roomnumber->addItem("5");
}

Login::~Login()
{
    delete ui;
}

void Login::on_confirm_clicked()
{
    roomNumber = ui->roomnumber->currentText();   //设计为客户端登陆自主选房间号
    accept();

}

void Login::on_cancel_clicked()
{
    close();
}

QString Login::getRoomNumber()
{
    return roomNumber;
}
