#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTcpServer>
#include<QTcpSocket>
//#include "thread.h"
#include "DataPact.h"
#include <QListWidget>

namespace Ui {
class MainWindow;
}

#include <QThread>
#include <iostream>


class TcpSocket_Room
{
public:
    int bound;
    QTcpSocket* TcpSocket;

};
typedef TcpSocket_Room* TR;

class roomtimer
{
public:
    int used;
    int interval;
};

typedef roomtimer* rT;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int roomNumber, QWidget *parent = 0);
    ~MainWindow();


public slots:
     void onTimerOut();
     void response();

     void send(int socket_num,int ptype);
     void recv();
     void tempchange0();
     void tempchange1();
     void tempchange2();
     void tempchange3();
     void tempchange4();

     void recv0();
     void recv1();
     void recv2();
     void recv3();
     void recv4();
private slots:
     void on_power_clicked();

     void on_modeselect_activated(int index);

     void on_set_clicked();

     void on_pay_clicked();

     void get_connect();

     void on_dayrecord_clicked();

     void on_weekrecord_clicked();

     void on_monthrecord_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *timer2;
    QTimer *roomtimer0;
     QTimer *roomtimer1;
      QTimer *roomtimer2;
       QTimer *roomtimer3;
        QTimer *roomtimer4;

    int rNumber;
    int state;
    CentralCondition* C;
    QTcpServer* TcpServer;
    TR TcpSocketTable[5];
    QTcpSocket* connection;
    rT roomTimer[5];
};

void sendmsg(QByteArray m);
#endif // MAINWINDOW_H
