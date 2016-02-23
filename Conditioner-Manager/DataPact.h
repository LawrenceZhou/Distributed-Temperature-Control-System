#ifndef DATAPACT_H
#define DATAPACT_H

#include <QString>
#include <QDateTime>
#define DEFAULT_MODE 0
#define DEFAULT_RULE 0
#define DEFAULT_C_LTEMP 18
#define DEFAULT_C_UTEMP 30
#define DEFAULT_H_LTEMP 22
#define DEFAULT_H_UTEMP 30
#define OFF 0
#define READY 1
#define ON 2
#define DEAD 1
#define WAITING 3
#define RESPONSED 0
#define SLEEP 2
#define ENVIROTEMP 28
#define PAID 1
#define UNPAID 0
#define INIT 1
#define NOTINIT 0
#define COOL 1
#define HOT 0
#define INITPKT 0
#define NORMALPKT 1
#define OFFPKT 2
#define SLEEPPKT 3
#define SWITCHON 2
#define RUNNING 1
#define SWITCHOFF 0
#define WAKEUP 1
#define NOTWAKEUP 0
#define CHARGERATE 1.5
#define DEFAULTTEMP 24
#define DEFAULTSPEED 1

#define NOSTARTTIME 0
#define NOEND 1
#define COMPLETE 2

#define DAY 0
#define WEEK 1
#define MONTH 2

#include <string>
#include <sstream>
using namespace std;

class Request
{
public:
    int room_num;
    //QString address;
    //int port;
    bool mode;
    int cur_temp;
    int goaltemp;
    int def_temp;
    int def_wind_speed;
    int speed;
    double charge;
    double chargetemp;
    double charge_rate;
    double energy;
    double energytemp;
    int state;
    QString settime;
    QString startttime;
    QString endtime;
    int onceWakedUp;



    Request(int rn, double gt, int sp, QString t,int s);
    void PowerChange();
    void MoneyChange();
    int getRequestState();
    //void reSet(int gt, int sp, int st, QString t);
    void setState(int st);
    void setCurTemp(double ct);
    double getCurTemp();
    int getRequestNumber();
    double getRequestGTemp();
    int getRequestGSpeed();
    QString getRequestSetTime();
    double getCurMoney();

    int TempChange();
};

typedef Request* RP;

class CentralCondition
{
private:
    int roomNumber;
    int mode;
    int rule;
    int lowtemp;
    int uptemp;


    //QMutex mutex;


public:
    int responsingNumber;
    RP request[5];
    CentralCondition(int rn, int m, int r, int lt, int ut);
    void resetCCondition(int rn, int m, int r, int lt, int ut);
    //void addRequest(int rn, double gt, int sp, QDateTime t);
    int getMode();
    int getRule();
    int getLowtemp();
    int getUptemp();
    void setRequestCurrTemp(int rn, double ct);
    int getRequestState(int rn);
    //void requestPowerChange(int rn);
    //void requestTempChange();
    //void requestMoneyChange(int rn, int r);
    void setRequestState(int rn, int s);
    Request getRequest(int rn);
    void setMode(int i);
    int scanRequest();
    int getResponseNumber();
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

#endif // DATAPACT_H
