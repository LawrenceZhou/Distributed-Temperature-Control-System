#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "room.h"
#include <QDateTime>
#include <QTimer>
#include <QUdpSocket>
#include <QTcpSocket>
#include <string>
#include <sstream>
#include <qthread.h>
using namespace std;

Room room("");
//QHostAddress a("192.168.43.77");
QHostAddress a("10.8.179.104");
//QHostAddress a("172.20.10.6");

MainWindow::MainWindow(Room r, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("空调界面");
    setFixedSize(450,270);
    ui->goaltemp->setRange(18,25);
    ui->goalspeed->addItem("低速");
    ui->goalspeed->addItem("中速");
    ui->goalspeed->addItem("高速");

    room = r;
    ui->roomnumber->setText(room.getRoomNumber());
    if(room.getRoomState() == OFF)
    {
        ui->status->setText("关机");
    }

    ui->currmoney->setText("");
    ui->currspeed->setText("");
    ui->currtemp->setText("");
    ui->mode->setText("");
    ui->setgoalspeed->setText("");
    ui->setgoaltemp->setText("");

    //新建一个QTimer对象
    timer = new QTimer();
    //设置定时器每个多少毫秒发送一个timeout()信号
    timer->setInterval(1000);
    //启动定时器
    timer->start();

    //信号和槽
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_power_clicked()
{
    if(room.getRoomState() == OFF)
    {
        room.setRoomState(READY);
        ui->goaltemp->setValue(25);    //需修改：按照中央空调发来的默认参数包修改
        ui->goalspeed->setCurrentIndex(1);

        ui->status->setText("运行");
        tcpSocket = new QTcpSocket(this);
        tcpSocket->connectToHost(a,8888);
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(recv()));
        room.goalTemp=ui->goaltemp->value();
        room.goalSpeed = ui->goalspeed->currentIndex()+1;
        RequestPkt on_pkt(room.getRoomNumber().toInt(),room.goalTemp,room.goalSpeed,SWITCHON,NOTWAKEUP);
        string s =on_pkt.getClassToString();

        const char* c= s.c_str();
        qDebug()<<c;

        tcpSocket->write(c);

        QString gtemp = QString::number(room.goalTemp,10);

        gspeedstring = ui->goalspeed->currentText();
        ui->setgoalspeed->setText(gspeedstring);
        ui->setgoaltemp->setText(gtemp);

    }
    else if(room.getRoomState() == ON || room.getRoomState() == READY)
    {
        room.setRoomState(OFF);
        ui->status->setText("关机");
        RequestPkt on_pkt(room.getRoomNumber().toInt(),room.goalTemp,room.goalSpeed,SWITCHOFF,NOTWAKEUP);
        string s = on_pkt.getClassToString();

        const char* c= s.c_str();
        qDebug()<<c;

        tcpSocket->write(c);
        //tcpSocket->close();

    }


}
void MainWindow::onTimerOut()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->time->setText(str);
}

void MainWindow::on_sendrequest_clicked()
{
    if(room.getRoomState()==OFF)
    {}
    else
    {
        if(room.getRoomState()==READY)
        {
            room.setRoomState(ON);
            ui->status->setText("运行");
        }
        room.goalTemp=ui->goaltemp->value();
        room.goalSpeed=ui->goalspeed->currentIndex()+1;
        RequestPkt* re_pkt = new RequestPkt(room.getRoomNumber().toInt(),room.goalTemp,room.goalSpeed,RUNNING,NOTWAKEUP);

        srequest =re_pkt->getClassToString();



        requesttimer = new QTimer();

        connect(requesttimer, SIGNAL(timeout()), this, SLOT(sendrequest()));
        requesttimer->start(10000);



        QString gtemp = QString::number(room.goalTemp,10);

        gspeedstring = ui->goalspeed->currentText();
        ui->setgoalspeed->setText(gspeedstring);
        ui->setgoaltemp->setText(gtemp);

    }
}

void MainWindow::sendrequest()
{
    const char* rc= srequest.c_str();
    qDebug()<<rc;
    tcpSocket->write(rc);
    if(requesttimer->isActive())
    {
        requesttimer->stop();
    }
}





void MainWindow::recv()
{
    ControlPkt *pkt = new ControlPkt(0,0,0,0,0,0,0,0,0,0);

    QString qs = tcpSocket->readAll();

    qDebug()<<qs;

    string st = qs.toStdString();


    pkt->getStringToClass(st);

    pkt->printClass();

    room.money =pkt->charge;
    ui->currmoney->setText(QString::number(room.money,'f',2));

    if(pkt->is_init == INIT)
    {

        room.goalSpeed=pkt->def_wind_speed-1;
        room.roomTemp =pkt->cur_temp;
        room.default_set=pkt->cur_temp;
        //room.goalTemp = pkt->def_temp;
        room.mode =pkt->mode;

        ui->goalspeed->setCurrentIndex(room.goalSpeed);
        ui->goaltemp->setValue(room.goalTemp);
        ui->currtemp->setText(QString::number(room.roomTemp,10));
        if(room.getRoomState()==ON)
        {
        QString speedstring = ui->goalspeed->currentText();
                ui->currspeed->setText(speedstring);
        }

        if(room.mode == COOL)
        {
            ui->mode->setText("制冷");
        }else
        {
            ui->mode->setText("加热");
        }
    }
    else
    {
        room.roomTemp =pkt->cur_temp;
        ui->currtemp->setText(QString::number(room.roomTemp,10));

        if(room.getRoomState()==ON)
        {
        QString speedstring = ui->goalspeed->currentText();
                ui->currspeed->setText(speedstring);
        }

        //qDebug()<<pkt->state<<"  "<<room.roomTemp<<"   "<<room.goalTemp<<endl;
        if(pkt->state == SLEEP && room.roomTemp == room.goalTemp)
        {
            //room.goalTemp=ui->goaltemp->value();
            RequestPkt* re_pkt = new RequestPkt(room.getRoomNumber().toInt(),room.goalTemp,room.goalSpeed,RUNNING,WAKEUP);

            srequest =re_pkt->getClassToString();

            wakeuptimer = new QTimer();

            connect(wakeuptimer, SIGNAL(timeout()), this, SLOT(sendrequest()));
            requesttimer->start(120000);
        }
        else if(pkt->state == DEAD)
        {
            room.setRoomState(OFF);
            ui->status->setText("关机");
            //tcpSocket->close();
        }

    }
    //qDebug()<<pkt->room_num;

    //qDebug()<<pkt->def_temp<<"  "<<pkt->def_wind_speed <<"   "<<pkt->mode;
}


RequestPkt::RequestPkt(int _room_num, double _goal_temp, int _goal_wind_speed, int _turn_on_event, int _wake_up_event)
{
    room_num = _room_num;
    goal_temp = _goal_temp;
    goal_wind_speed = _goal_wind_speed;
    turn_on_event = _turn_on_event;
    wake_up_event = _wake_up_event;
}

ControlPkt::ControlPkt(int _is_init, int _room_num, int _mode, int _cur_temp, int _def_temp, int _def_wind_speed, double _charge, double _charge_rate, double _energy, int _state)
{
    is_init = _is_init;
    room_num=_room_num;
    mode=_mode;
    cur_temp =_cur_temp;
    def_temp =_def_temp;
    def_wind_speed = _def_wind_speed;
    charge =_charge;
    charge_rate = _charge_rate;
    energy = _energy;
    state = _state;
}

string ControlPkt::getClassToString() {
    stringstream s1,s2,s3,s4,s5,
            s6,s7,s8,s9, s10;
    s1 << room_num;
    s2<<is_init;

    s3 << cur_temp;
    s4 << def_temp;
    s5 << def_wind_speed;
    s6 << charge;
    s7 << charge_rate;
    s8 << energy;
    s9 << state;
    s10<<mode;

    string ss1, ss2, ss3, ss4, ss5,
            ss6, ss7, ss8, ss9, ss10,s;
    s1 >> ss1;
    s2>>ss2;
    s3 >> ss3;
    s4 >> ss4;
    s5 >> ss5;
    s6 >> ss6;
    s7 >> ss7;
    s8 >> ss8;
    s9 >> ss9;
    s10>>ss10;
    s = ss1 + "_" + ss2 + "_" +
            ss3 + "_" + ss4 + "_" + ss5 +
            "_" + ss6 + "_" + ss7 + "_" + ss8
            + "_" + ss9 +"_"+ss10+ "\0";
    return s;

}

void ControlPkt::getStringToClass(string target) {
    string array[15];
    int len = 0;
    string temp = "";
    for (int i=0; i<target.length(); i++) {
        if (target[i] == '_') {
            array[len++] = temp;
            temp = "";
        }
        else{
            temp += target[i];
        }
    }
    array[len] = temp;
    stringstream s1,s2,s3,s4,s5,
            s6,s7,s8,s9,s10;
    s1 << array[0];
    s2 << array[1];
    s3 << array[2];
    s4 << array[3];
    s5 << array[4];
    s6 << array[5];
    s7 << array[6];
    s8 << array[7];
    s9 << array[8];
    s10<<array[9];

    s1 >> room_num;
    s2 >> is_init;
    s3 >> cur_temp;
    s4 >> def_temp;
    s5 >> def_wind_speed;
    s6 >> charge;
    s7 >> charge_rate;
    s8 >> energy;
    s9 >> state;
    s10>>mode;


}


void ControlPkt::printClass() {
    //qDebug()<<is_init<<" "<<charge<<" "<<charge_rate<<" "<<cur_temp<<" "<<def_temp<<" "<<def_wind_speed<<" "<<energy<<" "<<mode<<" "<<room_num<<" "<<state<<endl;
}

string RequestPkt::getClassToString() {

    stringstream s1,s2,s3,s4,s5;
    s1 << room_num;
    s2 << goal_temp;
    s3 << goal_wind_speed;
    s4 << turn_on_event;
    s5 << wake_up_event;

    string ss1,ss2,ss3,ss4,ss5,s;
    s1 >> ss1;
    s2 >> ss2;
    s3 >> ss3;
    s4 >> ss4;
    s5 >> ss5;
    s = ss1 + "_" + ss2 + "_" +
            ss3 + "_" + ss4 + "_" + ss5+"\0" ;

    return s;

}

void RequestPkt::getStringToClass(string target) {
    string array[15];
    int len = 0;
    string temp = "";
    for (int i=0; i<target.length(); i++) {
        if (target[i] == '_') {
            array[len++] = temp;
            temp = "";
        }
        else{
            temp += target[i];
        }
    }
    array[len] = temp;

    stringstream s1,s2,s3,s4,s5;
    s1 << array[0];
    s2 << array[1];
    s3 << array[2];
    s4 << array[3];
    s5 << array[4];

    s1 >> room_num;
    s2 >> goal_temp;
    s3 >> goal_wind_speed;
    s4 >> turn_on_event;
    s5 >> wake_up_event;

}
