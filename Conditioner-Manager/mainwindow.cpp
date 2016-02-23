#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DataPact.h"
#include <QUdpSocket>
//#include "thread.h"
#include "database.h"
#include <QTcpServer>
#include<QTcpSocket>
#include  <QTimer>
#include <QDateTime>
#include "recordview.h"

QUdpSocket *udpSocket;
//int number=0;

//QHostAddress a("192.168.1.106");
QHostAddress a("10.205.4.196");
//QHostAddress a("172.20.10.6");

MainWindow::MainWindow(int roomNumber, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)//new
{
    ui->setupUi(this);
    setWindowTitle("管理界面");
    setFixedSize(520,560);

    ui->ltemp->setText("");
    ui->utemp->setText("");
    ui->mode->setText("");

    rNumber = 5;
    //rNumber = roomNumber;
    C = new CentralCondition(5, 0, 0, DEFAULT_C_LTEMP, DEFAULT_C_UTEMP);
    ui->roomnumber->setText(QString::number(rNumber));
    ui->rule->setText("");
    ui->status->setText("关机");
    ui->time->setText("");
    state = OFF;
    //新建一个QTimer对象
    timer = new QTimer();
    //设置定时器每个多少毫秒发送一个timeout()信号
    timer->setInterval(1000);
    //启动定时器
    timer->start();


    //信号和槽
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    timer2 = new QTimer();
    timer2->setInterval(1000);
    timer2->start();
    connect(timer2, SIGNAL(timeout()), this, SLOT(response()));

    ui->modeselect->addItem("加热");
    ui->modeselect->addItem("制冷");


    ui->ruleselect->addItem("时间计费");
    ui->ruleselect->addItem("能量计费");

    ui->ltempselect->setRange(DEFAULT_C_LTEMP,DEFAULT_C_UTEMP);

    ui->utempselect->setRange(DEFAULT_C_UTEMP,DEFAULT_C_UTEMP);

    for (int i = 1; i <= rNumber; i++)
    {
        ui->payroomnumber->addItem(QString::number(i)+"号房间");
    }



    for(int i=1;i<=rNumber;i++)
    {
        C->setRequestCurrTemp(i,ENVIROTEMP);//设置环境温度
    }

    //TCP PART
    TcpSocketTable[0] = new TcpSocket_Room;
    TcpSocketTable[1] = new TcpSocket_Room;
    TcpSocketTable[2] = new TcpSocket_Room;
    TcpSocketTable[3] = new TcpSocket_Room;
    TcpSocketTable[4] = new TcpSocket_Room;

    TcpSocketTable[0]->TcpSocket = new QTcpSocket();
    TcpSocketTable[1]->TcpSocket = new QTcpSocket();
    TcpSocketTable[2]->TcpSocket = new QTcpSocket();
    TcpSocketTable[3]->TcpSocket = new QTcpSocket();
    TcpSocketTable[4]->TcpSocket = new QTcpSocket();

    roomTimer[0]= new roomtimer;
    roomTimer[1]= new roomtimer;
    roomTimer[2]= new roomtimer;
    roomTimer[3]= new roomtimer;
    roomTimer[4]= new roomtimer;

    roomTimer[0]->used=0;
    roomTimer[1]->used=0;
    roomTimer[2]->used=0;
    roomTimer[3]->used=0;
    roomTimer[4]->used=0;

    roomTimer[0]->interval=0;
    roomTimer[1]->interval=0;
    roomTimer[2]->interval=0;
    roomTimer[3]->interval=0;
    roomTimer[4]->interval=0;
}

MainWindow::~MainWindow()//new
{
    delete ui;
}

void MainWindow::onTimerOut()//new
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->time->setText(str);

    if(state ==ON)
    {
        QString qmsg;
        for(int i =0; i<5;i++)
        {
            if(C->request[i]->getRequestState()==RESPONSED)
            {

                qmsg += "房间号：";
                qmsg += QString::number(i+1,10);
                qmsg += "  目标温度：";
                qmsg += QString::number(C->request[i]->getRequestGTemp());
                if(C->request[i]->getRequestGSpeed()==0)
                {
                    qmsg += "  目标风速：低速\n";
                }
                else if(C->request[i]->getRequestGSpeed()==1)
                {
                    qmsg += "  目标风速：中速\n";
                }
                else
                {
                    qmsg += "目标风速：高速\n";
                }

            }
        }
        ui->respondingrequest->setText(qmsg);

        QString qmsg2;

        for(int i =0; i<5;i++)
        {
            if(C->request[i]->getRequestState()==WAITING)
            {

                qmsg2 += "房间号：";
                qmsg2 += QString::number(i+1,10);
                qmsg2 += "  目标温度：";
                qmsg2 += QString::number(C->request[i]->getRequestGTemp());
                if(C->request[i]->getRequestGSpeed()==0)
                {
                    qmsg2 += "  目标风速：低速\n";
                }
                else if(C->request[i]->getRequestGSpeed()==1)
                {
                    qmsg2 += "  目标风速：中速\n";
                }
                else
                {
                    qmsg2 += "目标风速：高速\n";
                }

            }
        }
        ui->waitingrequest->setText(qmsg2);

        QString qmsg3;

        for(int i =0; i<5;i++)
        {
            if(C->request[i]->state==RESPONSED)
            {
                qmsg3 += "房间号：";
                qmsg3 += QString::number(i+1,10);
                qmsg3 += "  正在响应";
                qmsg3 += "  当前温度：";
                qmsg3 += QString::number(C->request[i]->cur_temp,10);
                if(C->request[i]->getRequestGSpeed()==0)
                {
                    qmsg3 += "  当前风速：低速\n";
                }
                else if(C->request[i]->getRequestGSpeed()==1)
                {
                    qmsg3 += "  当前风速：中速\n";
                }
                else
                {
                    qmsg3 += "  当前风速：高速\n";
                }
            }
            else if(C->request[i]->state==DEAD)
            {
                qmsg3 += "房间号：";
                qmsg3 += QString::number(i+1,10);
                qmsg3+= "  无请求\n";
            }
            else if(C->request[i]->state==WAITING)
            {
                qmsg3 += "房间号：";
                qmsg3 += QString::number(i+1,10);
                qmsg3+= "  未响应";
                qmsg3 += "  当前温度：";
                qmsg3 += QString::number(C->request[i]->cur_temp,10);

            }
            else if(C->request[i]->state== SLEEP)
            {
                qmsg3 += "房间号：";
                qmsg3 += QString::number(i+1,10);
                qmsg3+= "  挂起";
                qmsg3 += "  当前温度：";
                qmsg3 += QString::number(C->request[i]->cur_temp,10);
                qmsg3 += "\n";
            }
        }

        ui->roomlist->setText(qmsg3);
    }
}

void MainWindow::response()
{
    for(int i=0;i<5;i++)
    {
        if(TcpSocketTable[i]->bound==1 && C->request[i]->getRequestState() != SLEEP)
        {
            send(i,NORMALPKT);
        }
    }
}

void MainWindow::on_power_clicked()//new
{
    if(state == OFF)
    {
        ui->modeselect->setCurrentIndex(0);
        ui->ruleselect->setCurrentIndex(0);
        ui->ltempselect->setValue(DEFAULT_C_LTEMP);
        ui->utempselect->setValue(DEFAULT_C_UTEMP);

        state = READY;
        ui->status->setText("待机");

        TcpServer = new QTcpServer(this);
        if(!TcpServer->listen(QHostAddress::Any,22222))
        {
            qDebug()<<TcpServer->errorString();
        }
        connect(TcpServer,SIGNAL(newConnection()),this,SLOT(get_connect()));
    }
    else if(state == ON || state == READY)
    {
        state = OFF;
        ui->status->setText("关机");
    }
}

void MainWindow::on_modeselect_activated(int index)//new
{
    if(state == OFF || state == ON)
    {}
    else if(state == READY )
    {

        if(index == 0)
        {

            ui->ltempselect->setRange(DEFAULT_C_LTEMP,DEFAULT_C_UTEMP);

            ui->utempselect->setRange(DEFAULT_C_LTEMP,DEFAULT_C_UTEMP);

            ui->ltempselect->setValue(DEFAULT_C_LTEMP);

            ui->utempselect->setValue(DEFAULT_C_UTEMP);
        }
        else
        {
            C->setMode(index);

            ui->ltempselect->setRange(DEFAULT_H_LTEMP,DEFAULT_H_UTEMP);

            ui->utempselect->setRange(DEFAULT_H_LTEMP,DEFAULT_H_UTEMP);

            ui->ltempselect->setValue(DEFAULT_H_LTEMP);

            ui->utempselect->setValue(DEFAULT_H_UTEMP);
        }
    }
}

void MainWindow::on_set_clicked()//new
{
    if (state == READY)
    {
        state=ON;
        int mode = ui->modeselect->currentIndex();
        int rule = ui->ruleselect->currentIndex();
        int ltemp = ui->ltempselect->value();
        int utemp = ui->utempselect->value();

        ui->status->setText("运行");

        if(mode == COOL)
        {
            ui->mode->setText("制冷");

        }
        else
        {
            ui->mode->setText("加热");
        }

        if(rule == 0)
        {
            ui->rule->setText("时间计费");
        }
        else
        {
            ui->rule->setText("能量计费");
        }

        ui->ltemp->setText(QString::number(ltemp));

        ui->utemp->setText(QString::number(utemp));

        C->resetCCondition(5, mode, rule, ltemp, utemp);

    }
}

void MainWindow::on_pay_clicked()
{
    if(state == ON || state == READY)
    {
        int roomnum=ui->payroomnumber->currentIndex()+1;
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        C->request[roomnum-1]->endtime=str;
        //结上一次数据库
        endRequest(C->request[roomnum-1]);
        C->request[roomnum-1]->state = DEAD;
        send(roomnum-1,OFFPKT);
        roomTimer[roomnum-1]->used =0;
        QMessageBox::information(NULL, "出账成功！", QObject::tr("房间编号：%1   \n起始时间：%2  \n结束时间：%3  \n消费金额：%4元 消费电量：%5度").arg(roomnum).arg(C->request[roomnum-1]->startttime).arg(C->request[roomnum-1]->endtime).arg(C->request[roomnum-1]->charge).arg(C->request[roomnum-1]->energy));

        C->request[roomnum-1]->charge=0;
        C->request[roomnum-1]->energy=0;
        C->request[roomnum-1]->chargetemp =0;
        C->request[roomnum-1]->energytemp =0;
        C->request[roomnum-1]->onceWakedUp=0;

    }
}

void MainWindow::get_connect()  //new
{
    connection = TcpServer->nextPendingConnection();
    connect(connection,SIGNAL(readyRead()),this,SLOT(recv()));

}

void MainWindow::send(int socket_num, int ptype)//new
{
    if(ptype == INITPKT)   //ok,初始化
    {
        ControlPkt pkt(INIT,socket_num+1,C->getMode(),ENVIROTEMP,DEFAULTTEMP,DEFAULTSPEED,0,CHARGERATE,0,RESPONSED);
        string s = pkt.getClassToString();

        const char* c= s.c_str();
        //qDebug()<<c;

        TcpSocketTable[socket_num]->TcpSocket->write(c);
        qDebug()<<"已发送"<<endl;
    }
    else if(ptype == NORMALPKT) //普通
    {
        ControlPkt pkt(NOTINIT,socket_num+1,C->getMode(),C->request[socket_num]->cur_temp,DEFAULTTEMP,DEFAULTSPEED,C->request[socket_num]->charge,CHARGERATE,C->request[socket_num]->energy, RESPONSED);

        string s = pkt.getClassToString();

        const char* c= s.c_str();
        //qDebug()<<c;

        TcpSocketTable[socket_num]->TcpSocket->write(c);

    }
    else if(ptype== OFFPKT)//到达目标温度，睡眠
    {
        ControlPkt pkt(NOTINIT,socket_num+1,C->getMode(),C->request[socket_num]->cur_temp,DEFAULTTEMP,DEFAULTSPEED,C->request[socket_num]->charge,CHARGERATE,C->request[socket_num]->energy, DEAD);

        string s = pkt.getClassToString();

        const char* c= s.c_str();
        //qDebug()<<c;

        TcpSocketTable[socket_num]->TcpSocket->write(c);
    }
    else if(ptype== SLEEPPKT)//到达目标温度，睡眠
    {
        ControlPkt pkt(NOTINIT,socket_num+1,C->getMode(),C->request[socket_num]->cur_temp,DEFAULTTEMP,DEFAULTSPEED,C->request[socket_num]->charge,CHARGERATE,C->request[socket_num]->energy, SLEEP);

        string s = pkt.getClassToString();

        const char* c= s.c_str();
        //qDebug()<<c;

        TcpSocketTable[socket_num]->TcpSocket->write(c);
    }

}

void MainWindow::recv()//new
{
    RequestPkt *rpkt = new RequestPkt(0,0,0,0,0);
    QString qs = connection->readAll();

    //qDebug()<<qs;

    string st = qs.toStdString();
    rpkt->getStringToClass(st);
    //qDebug()<<rpkt->turn_on_event<<rpkt->goal_temp<<rpkt->room_num<<rpkt->goal_wind_speed<<rpkt->wake_up_event;
    if(rpkt->turn_on_event == SWITCHON)   //开机
    {
        TcpSocketTable[rpkt->room_num-1]->TcpSocket = connection;
        TcpSocketTable[rpkt->room_num-1]->bound =1;
        connection = new QTcpSocket();

        send(rpkt->room_num-1,INITPKT);

        if(rpkt->room_num==1)
        {
            connect(TcpSocketTable[0]->TcpSocket,SIGNAL(readyRead()),this,SLOT(recv0()));
        }
        else if(rpkt->room_num==2)
        {
            connect(TcpSocketTable[1]->TcpSocket,SIGNAL(readyRead()),this,SLOT(recv1()));
        }
        else if(rpkt->room_num==3)
        {
            connect(TcpSocketTable[2]->TcpSocket,SIGNAL(readyRead()),this,SLOT(recv2()));
        }
        else if(rpkt->room_num==4)
        {
            connect(TcpSocketTable[3]->TcpSocket,SIGNAL(readyRead()),this,SLOT(recv3()));
        }
        else if(rpkt->room_num==5)
        {
            connect(TcpSocketTable[4]->TcpSocket,SIGNAL(readyRead()),this,SLOT(recv4()));
        }
    }

}

void MainWindow::recv0()
{
    RequestPkt *rpkt = new RequestPkt(0,0,0,0,0);
    QString qs = TcpSocketTable[0]->TcpSocket->readAll();

    qDebug()<<qs;

    string st = qs.toStdString();
    rpkt->getStringToClass(st);
    qDebug()<<rpkt->turn_on_event<<rpkt->goal_temp<<rpkt->room_num<<rpkt->goal_wind_speed<<rpkt->wake_up_event;

    if(rpkt->turn_on_event == RUNNING)    //请求
    {
        int gtemp = rpkt->goal_temp;

        int gspeed = rpkt->goal_wind_speed;

        int roomnum = rpkt->room_num;

        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        if(C->getRequestState(roomnum)==SLEEP||C->getRequestState(roomnum)==WAITING||C->getRequestState(roomnum)==RESPONSED)
        {
            if(rpkt->wake_up_event==NOTWAKEUP)
            {
                C->request[roomnum-1]->endtime =str;
                //shujuku
                endRequest(C->request[roomnum-1]);


                C->request[roomnum-1]->chargetemp=0;
                C->request[roomnum-1]->energytemp=0;
                 C->request[roomnum-1]->onceWakedUp=0;
                C->request[roomnum-1]->endtime="";
            }
        }

        if(C->getRequestState(roomnum)==DEAD||C->getRequestState(roomnum)==SLEEP)
        {

            if(C->getResponseNumber()<3)
            {

                if(roomTimer[roomnum-1]->used == 0)
                {
                    C->request[roomnum-1]->goaltemp = gtemp;
                    C->request[roomnum-1]->speed = gspeed;
                    C->request[roomnum-1]->state = RESPONSED;
                    C->request[roomnum-1]->settime = str;

                    if(rpkt->wake_up_event == NOTWAKEUP)
                    {
                        C->request[roomnum-1]->startttime = str;
                        //shujuku
                        addRequestDB(C->request[roomnum-1],C);
                        addStartTime(C->request[roomnum-1]);
                    }


                    roomTimer[roomnum-1]->used =1;

                    roomTimer[roomnum-1]->interval = (3-gspeed)*10000;



                }
            }
            else
            {
                C->request[roomnum-1]->goaltemp = gtemp;
                C->request[roomnum-1]->speed = gspeed;
                C->request[roomnum-1]->state = WAITING;
                roomTimer[roomnum-1]->interval =20000;
                C->request[roomnum-1]->settime = str;
                //shujuku
                if(rpkt->wake_up_event == NOTWAKEUP)
                {
                    addRequestDB(C->request[roomnum-1],C);
                }

            }
        }


        else if(C->getRequestState(roomnum)==RESPONSED||C->getRequestState(roomnum)==WAITING)
        {
            C->request[roomnum-1]->goaltemp = gtemp;
            C->request[roomnum-1]->speed = gspeed;
            roomTimer[roomnum-1]->interval =(3-gspeed)*10000;
        }


        if(rpkt->wake_up_event == NOTWAKEUP)
        {
            //C->request[roomnum-1]->startttime = str;
            //addRequestDB(C->request[roomnum-1],C);
            //addStartTime(C->request[roomnum-1]);
            roomtimer0 = new QTimer();
            roomtimer0->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer0->start();
            connect(roomtimer0, SIGNAL(timeout()), this, SLOT(tempchange0()));
        }else
        {
            roomtimer0->setInterval(roomTimer[roomnum-1]->interval);
            C->request[roomnum-1]->onceWakedUp=1;//starttime bug

            roomtimer0->start();
        }

    }

    else if(rpkt->turn_on_event == SWITCHOFF)   //guanji!!!
    {
        int roomnum = rpkt->room_num;
        roomTimer[roomnum-1]->used =0;
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        C->request[roomnum-1]->endtime=str;
        //结上一次数据库
        endRequest(C->request[roomnum-1]);
        C->request[roomnum-1]->state = DEAD;
        C->request[roomnum-1]->chargetemp =0;
        C->request[roomnum-1]->energytemp =0;
         C->request[roomnum-1]->onceWakedUp=0;

    }
}

void MainWindow::recv1()
{
    RequestPkt *rpkt = new RequestPkt(0,0,0,0,0);
    QString qs = TcpSocketTable[1]->TcpSocket->readAll();

    qDebug()<<qs;

    string st = qs.toStdString();
    rpkt->getStringToClass(st);
    qDebug()<<rpkt->turn_on_event<<rpkt->goal_temp<<rpkt->room_num<<rpkt->goal_wind_speed<<rpkt->wake_up_event;

    if(rpkt->turn_on_event == RUNNING)    //请求
    {
        int gtemp = rpkt->goal_temp;

        int gspeed = rpkt->goal_wind_speed;

        int roomnum = rpkt->room_num;

        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        if(C->getRequestState(roomnum)==SLEEP||C->getRequestState(roomnum)==WAITING||C->getRequestState(roomnum)==RESPONSED)
        {
            if(rpkt->wake_up_event==NOTWAKEUP)
            {
                C->request[roomnum-1]->endtime =str;
                //shujuku
                endRequest(C->request[roomnum-1]);


                C->request[roomnum-1]->chargetemp=0;
                C->request[roomnum-1]->energytemp=0;
                 C->request[roomnum-1]->onceWakedUp=0;
                C->request[roomnum-1]->endtime="";
            }
        }

        if(C->getRequestState(roomnum)==DEAD||C->getRequestState(roomnum)==SLEEP)
        {

            if(C->getResponseNumber()<3)
            {

                if(roomTimer[roomnum-1]->used == 0)
                {
                    C->request[roomnum-1]->goaltemp = gtemp;
                    C->request[roomnum-1]->speed = gspeed;
                    C->request[roomnum-1]->state = RESPONSED;
                    C->request[roomnum-1]->settime = str;

                    if(rpkt->wake_up_event == NOTWAKEUP)
                    {
                        C->request[roomnum-1]->startttime = str;
                        //shujuku
                        addRequestDB(C->request[roomnum-1],C);
                        addStartTime(C->request[roomnum-1]);
                    }


                    roomTimer[roomnum-1]->used =1;

                    roomTimer[roomnum-1]->interval = (3-gspeed)*1000;



                }
            }
            else
            {
                C->request[roomnum-1]->goaltemp = gtemp;
                C->request[roomnum-1]->speed = gspeed;
                C->request[roomnum-1]->state = WAITING;
                roomTimer[roomnum-1]->interval =2000;
                C->request[roomnum-1]->settime = str;
                //shujuku
                if(rpkt->wake_up_event == NOTWAKEUP)
                {
                    addRequestDB(C->request[roomnum-1],C);
                }

            }
        }


        else if(C->getRequestState(roomnum)==RESPONSED||C->getRequestState(roomnum)==WAITING)
        {
            C->request[roomnum-1]->goaltemp = gtemp;
            C->request[roomnum-1]->speed = gspeed;
            roomTimer[roomnum-1]->interval =(3-gspeed)*1000;
        }


        if(rpkt->wake_up_event == NOTWAKEUP)
        {
            //C->request[roomnum-1]->startttime = str;
            //addRequestDB(C->request[roomnum-1],C);
            //addStartTime(C->request[roomnum-1]);
            roomtimer1 = new QTimer();
            roomtimer1->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer1->start();
            connect(roomtimer1, SIGNAL(timeout()), this, SLOT(tempchange1()));
        }else
        {
            roomtimer1->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer1->start();
        }

    }

    else if(rpkt->turn_on_event == SWITCHOFF)   //guanji!!!
    {
        int roomnum = rpkt->room_num;
        roomTimer[roomnum-1]->used =0;
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        C->request[roomnum-1]->endtime=str;
        //结上一次数据库
        endRequest(C->request[roomnum-1]);
        C->request[roomnum-1]->state = DEAD;
        C->request[roomnum-1]->chargetemp =0;
        C->request[roomnum-1]->energytemp =0;
         C->request[roomnum-1]->onceWakedUp=0;

    }
}

void MainWindow::recv2()
{
    RequestPkt *rpkt = new RequestPkt(0,0,0,0,0);
    QString qs = TcpSocketTable[2]->TcpSocket->readAll();

    qDebug()<<qs;

    string st = qs.toStdString();
    rpkt->getStringToClass(st);
    qDebug()<<rpkt->turn_on_event<<rpkt->goal_temp<<rpkt->room_num<<rpkt->goal_wind_speed<<rpkt->wake_up_event;

    if(rpkt->turn_on_event == RUNNING)    //请求
    {
        int gtemp = rpkt->goal_temp;

        int gspeed = rpkt->goal_wind_speed;

        int roomnum = rpkt->room_num;

        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        if(C->getRequestState(roomnum)==SLEEP||C->getRequestState(roomnum)==WAITING||C->getRequestState(roomnum)==RESPONSED)
        {
            if(rpkt->wake_up_event==NOTWAKEUP)
            {
                C->request[roomnum-1]->endtime =str;
                //shujuku
                endRequest(C->request[roomnum-1]);


                C->request[roomnum-1]->chargetemp=0;
                C->request[roomnum-1]->energytemp=0;
                 C->request[roomnum-1]->onceWakedUp=0;
                C->request[roomnum-1]->endtime="";
            }
        }

        if(C->getRequestState(roomnum)==DEAD||C->getRequestState(roomnum)==SLEEP)
        {

            if(C->getResponseNumber()<3)
            {

                if(roomTimer[roomnum-1]->used == 0)
                {
                    C->request[roomnum-1]->goaltemp = gtemp;
                    C->request[roomnum-1]->speed = gspeed;
                    C->request[roomnum-1]->state = RESPONSED;
                    C->request[roomnum-1]->settime = str;

                    if(rpkt->wake_up_event == NOTWAKEUP)
                    {
                        C->request[roomnum-1]->startttime = str;
                        //shujuku
                        addRequestDB(C->request[roomnum-1],C);
                        addStartTime(C->request[roomnum-1]);
                    }


                    roomTimer[roomnum-1]->used =1;

                    roomTimer[roomnum-1]->interval = (3-gspeed)*1000;



                }
            }
            else
            {
                C->request[roomnum-1]->goaltemp = gtemp;
                C->request[roomnum-1]->speed = gspeed;
                C->request[roomnum-1]->state = WAITING;
                roomTimer[roomnum-1]->interval =2000;
                C->request[roomnum-1]->settime = str;
                //shujuku
                if(rpkt->wake_up_event == NOTWAKEUP)
                {
                    addRequestDB(C->request[roomnum-1],C);
                }
            }
        }


        else if(C->getRequestState(roomnum)==RESPONSED||C->getRequestState(roomnum)==WAITING)
        {
            C->request[roomnum-1]->goaltemp = gtemp;
            C->request[roomnum-1]->speed = gspeed;
            roomTimer[roomnum-1]->interval =(3-gspeed)*1000;
        }


        if(rpkt->wake_up_event == NOTWAKEUP)
        {
            //C->request[roomnum-1]->startttime = str;
            //addRequestDB(C->request[roomnum-1],C);
            //addStartTime(C->request[roomnum-1]);
            roomtimer2 = new QTimer();
            roomtimer2->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer2->start();
            connect(roomtimer2, SIGNAL(timeout()), this, SLOT(tempchange2()));
        }else
        {
            roomtimer2->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer2->start();
        }

    }

    else if(rpkt->turn_on_event == SWITCHOFF)   //guanji!!!
    {
        int roomnum = rpkt->room_num;
        roomTimer[roomnum-1]->used =0;
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        C->request[roomnum-1]->endtime=str;
        //结上一次数据库
        endRequest(C->request[roomnum-1]);
        C->request[roomnum-1]->state = DEAD;
        C->request[roomnum-1]->chargetemp =0;
        C->request[roomnum-1]->energytemp =0;
         C->request[roomnum-1]->onceWakedUp=0;

    }
}

void MainWindow::recv3()
{
    RequestPkt *rpkt = new RequestPkt(0,0,0,0,0);
    QString qs = TcpSocketTable[3]->TcpSocket->readAll();

    qDebug()<<qs;

    string st = qs.toStdString();
    rpkt->getStringToClass(st);
    qDebug()<<rpkt->turn_on_event<<rpkt->goal_temp<<rpkt->room_num<<rpkt->goal_wind_speed<<rpkt->wake_up_event;

    if(rpkt->turn_on_event == RUNNING)    //请求
    {
        int gtemp = rpkt->goal_temp;

        int gspeed = rpkt->goal_wind_speed;

        int roomnum = rpkt->room_num;

        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        if(C->getRequestState(roomnum)==SLEEP||C->getRequestState(roomnum)==WAITING||C->getRequestState(roomnum)==RESPONSED)
        {
            if(rpkt->wake_up_event==NOTWAKEUP)
            {
                C->request[roomnum-1]->endtime =str;
                //shujuku
                endRequest(C->request[roomnum-1]);


                C->request[roomnum-1]->chargetemp=0;
                C->request[roomnum-1]->energytemp=0;
                 C->request[roomnum-1]->onceWakedUp=0;
                C->request[roomnum-1]->endtime="";
            }
        }

        if(C->getRequestState(roomnum)==DEAD||C->getRequestState(roomnum)==SLEEP)
        {

            if(C->getResponseNumber()<3)
            {

                if(roomTimer[roomnum-1]->used == 0)
                {
                    C->request[roomnum-1]->goaltemp = gtemp;
                    C->request[roomnum-1]->speed = gspeed;
                    C->request[roomnum-1]->state = RESPONSED;
                    C->request[roomnum-1]->settime = str;

                    if(rpkt->wake_up_event == NOTWAKEUP)
                    {
                        C->request[roomnum-1]->startttime = str;
                        //shujuku
                        addRequestDB(C->request[roomnum-1],C);
                        addStartTime(C->request[roomnum-1]);
                    }


                    roomTimer[roomnum-1]->used =1;

                    roomTimer[roomnum-1]->interval = (3-gspeed)*1000;



                }
            }
            else
            {
                C->request[roomnum-1]->goaltemp = gtemp;
                C->request[roomnum-1]->speed = gspeed;
                C->request[roomnum-1]->state = WAITING;
                roomTimer[roomnum-1]->interval =2000;
                C->request[roomnum-1]->settime = str;
                //shujuku
                if(rpkt->wake_up_event == NOTWAKEUP)
                {
                    addRequestDB(C->request[roomnum-1],C);
                }

            }
        }


        else if(C->getRequestState(roomnum)==RESPONSED||C->getRequestState(roomnum)==WAITING)
        {
            C->request[roomnum-1]->goaltemp = gtemp;
            C->request[roomnum-1]->speed = gspeed;
            roomTimer[roomnum-1]->interval =(3-gspeed)*1000;
        }


        if(rpkt->wake_up_event == NOTWAKEUP)
        {
            //C->request[roomnum-1]->startttime = str;
            //addRequestDB(C->request[roomnum-1],C);
            //addStartTime(C->request[roomnum-1]);
            roomtimer3 = new QTimer();
            roomtimer3->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer3->start();
            connect(roomtimer3, SIGNAL(timeout()), this, SLOT(tempchange3()));
        }else
        {
            roomtimer3->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer3->start();
        }

    }

    else if(rpkt->turn_on_event == SWITCHOFF)   //guanji!!!
    {
        int roomnum = rpkt->room_num;
        roomTimer[roomnum-1]->used =0;
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        C->request[roomnum-1]->endtime=str;
        //结上一次数据库
        endRequest(C->request[roomnum-1]);
        C->request[roomnum-1]->state = DEAD;
        C->request[roomnum-1]->chargetemp =0;
        C->request[roomnum-1]->energytemp =0;
         C->request[roomnum-1]->onceWakedUp=0;

    }
}

void MainWindow::recv4()
{
    RequestPkt *rpkt = new RequestPkt(0,0,0,0,0);
    QString qs = TcpSocketTable[4]->TcpSocket->readAll();

    qDebug()<<qs;

    string st = qs.toStdString();
    rpkt->getStringToClass(st);
    qDebug()<<rpkt->turn_on_event<<rpkt->goal_temp<<rpkt->room_num<<rpkt->goal_wind_speed<<rpkt->wake_up_event;

    if(rpkt->turn_on_event == RUNNING)    //请求
    {
        int gtemp = rpkt->goal_temp;

        int gspeed = rpkt->goal_wind_speed;

        int roomnum = rpkt->room_num;

        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        if(C->getRequestState(roomnum)==SLEEP||C->getRequestState(roomnum)==WAITING||C->getRequestState(roomnum)==RESPONSED)
        {
            if(rpkt->wake_up_event==NOTWAKEUP)
            {
                C->request[roomnum-1]->endtime =str;
                //shujuku
                endRequest(C->request[roomnum-1]);


                C->request[roomnum-1]->chargetemp=0;
                C->request[roomnum-1]->energytemp=0;
                 C->request[roomnum-1]->onceWakedUp=0;
                C->request[roomnum-1]->endtime="";
            }
        }

        if(C->getRequestState(roomnum)==DEAD||C->getRequestState(roomnum)==SLEEP)
        {

            if(C->getResponseNumber()<3)
            {

                if(roomTimer[roomnum-1]->used == 0)
                {
                    C->request[roomnum-1]->goaltemp = gtemp;
                    C->request[roomnum-1]->speed = gspeed;
                    C->request[roomnum-1]->state = RESPONSED;
                    C->request[roomnum-1]->settime = str;

                    if(rpkt->wake_up_event == NOTWAKEUP)
                    {
                        C->request[roomnum-1]->startttime = str;
                        //shujuku
                        addRequestDB(C->request[roomnum-1],C);
                        addStartTime(C->request[roomnum-1]);
                    }


                    roomTimer[roomnum-1]->used =1;

                    roomTimer[roomnum-1]->interval = (3-gspeed)*1000;



                }
            }
            else
            {
                C->request[roomnum-1]->goaltemp = gtemp;
                C->request[roomnum-1]->speed = gspeed;
                C->request[roomnum-1]->state = WAITING;
                roomTimer[roomnum-1]->interval =2000;
                C->request[roomnum-1]->settime = str;
                //shujuku
                if(rpkt->wake_up_event == NOTWAKEUP)
                {
                    addRequestDB(C->request[roomnum-1],C);
                }
            }
        }


        else if(C->getRequestState(roomnum)==RESPONSED||C->getRequestState(roomnum)==WAITING)
        {
            C->request[roomnum-1]->goaltemp = gtemp;
            C->request[roomnum-1]->speed = gspeed;
            roomTimer[roomnum-1]->interval =(3-gspeed)*1000;
        }


        if(rpkt->wake_up_event == NOTWAKEUP)
        {
            //C->request[roomnum-1]->startttime = str;
            //addRequestDB(C->request[roomnum-1],C);
            //addStartTime(C->request[roomnum-1]);
            roomtimer4 = new QTimer();
            roomtimer4->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer4->start();
            connect(roomtimer4, SIGNAL(timeout()), this, SLOT(tempchange4()));
        }else
        {
            roomtimer4->setInterval(roomTimer[roomnum-1]->interval);


            roomtimer4->start();
        }

    }

    else if(rpkt->turn_on_event == SWITCHOFF)   //guanji!!!
    {
        int roomnum = rpkt->room_num;
        roomTimer[roomnum-1]->used =0;
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        C->request[roomnum-1]->endtime=str;
        //结上一次数据库
        endRequest(C->request[roomnum-1]);
        C->request[roomnum-1]->state = DEAD;
        C->request[roomnum-1]->chargetemp =0;
        C->request[roomnum-1]->energytemp =0;
         C->request[roomnum-1]->onceWakedUp=0;

    }
}

void MainWindow::tempchange0()
{
    C->request[0]->TempChange();
    if( C->request[0]->getRequestState() == SLEEP)
    {
        send(0,SLEEPPKT);
        roomTimer[0]->used = 0;
        roomtimer0->setInterval(20000);

        C->scanRequest();
    }

}

void MainWindow::tempchange1()
{
    C->request[1]->TempChange();
    if( C->request[1]->getRequestState() == SLEEP)
    {
        send(1,SLEEPPKT);
        roomTimer[1]->used = 0;
        roomtimer0->setInterval(2000);

        C->scanRequest();
    }
}

void MainWindow::tempchange2()
{
    C->request[2]->TempChange();
    if( C->request[2]->getRequestState() == SLEEP)
    {
        send(2,SLEEPPKT);
        roomTimer[2]->used = 0;
        roomtimer0->setInterval(2000);

        C->scanRequest();
    }
}

void MainWindow::tempchange3()
{
    C->request[3]->TempChange();
    if( C->request[3]->getRequestState() == SLEEP)
    {
        send(3,SLEEPPKT);
        roomTimer[3]->used = 0;
        roomtimer0->setInterval(2000);

        C->scanRequest();
    }
}

void MainWindow::tempchange4()
{
    C->request[4]->TempChange();
    if( C->request[4]->getRequestState() == SLEEP)
    {
        send(4,SLEEPPKT);
        roomTimer[4]->used = 0;
        roomtimer0->setInterval(2000);

        C->scanRequest();
    }
}


void MainWindow::on_dayrecord_clicked()
{
    RecordView rv(DAY);
    rv.exec();
}

void MainWindow::on_weekrecord_clicked()
{
    RecordView rv(WEEK);
    rv.exec();
}

void MainWindow::on_monthrecord_clicked()
{
    RecordView rv(MONTH);
    rv.exec();
}
