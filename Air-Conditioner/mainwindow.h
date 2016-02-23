#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "room.h"
#include <QUdpSocket>
#include <QTcpSocket>
#include <string>
#include <sstream>
using namespace std;

#define INIT 1
#define NOTINIT 0
#define COOL 0
#define HOT 1
#define DEAD 1
#define WAITING 3
#define RESPONSED 0
#define SLEEP 2
#define SWITCHON 2
#define RUNNING 1
#define SWITCHOFF 0
#define WAKEUP 1
#define NOTWAKEUP 0
class RequestPkt
{
public:
    /* The default constructor */
    RequestPkt(int _room_num, double _goal_temp, int _goal_wind_speed, int _turn_on_event,
                    int _wake_up_event );

    /* Some parameter in the request*/
    int     room_num;

    int     goal_temp;

    int     goal_wind_speed;

    /* To show the client is trun on(true) or turn off(false)*/
    int    turn_on_event;

    /* To show the client is wake up(true) or sleep(false)*/
    int    wake_up_event;
    string getClassToString();
    void getStringToClass(string target);

};


class ControlPkt
{
public:
    ControlPkt(int _is_init, int _room_num,int _mode,int _cur_temp,int _def_temp,int _def_wind_speed, double  _charge, double  _charge_rate,double  _energy, int _state);

    int is_init;

    int     room_num;

    int    mode;
    //调节后房间温度
    int     cur_temp;
    //这两个可用来设置缺省的风速和目标温度
    int     def_temp;

    int     def_wind_speed;

    double  charge;

    double  charge_rate;

    double  energy;
    /* Show what state the client will become after control*/
    int     state;

    string getClassToString();

       void getStringToClass(string target);

       void printClass();


};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Room r, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onTimerOut();
    void sendrequest();

    void recv();
private slots:
    void on_power_clicked();
    void on_sendrequest_clicked();


private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *requesttimer;
    QTimer *wakeuptimer;
    QUdpSocket *udpSocket;
    QTcpSocket *tcpSocket;
    QString gspeedstring;
    string srequest;
};

#endif // MAINWINDOW_H
