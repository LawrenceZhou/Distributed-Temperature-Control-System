#include "DataPact.h"
#include <QString>
#include "database.h"
#include "mainwindow.h"
#include <string>
#include <sstream>
using namespace std;

Request::Request(int rn, double gt, int sp, QString t,int s)
{
    room_num = rn;
    //address = a;
    //port = p;
    cur_temp = ENVIROTEMP;
    goaltemp = gt;
    speed = sp;
    //consumepower = cp;
    //consumemoney = cm;
    settime = t;
    state = s;
    charge =0;
    energy =0;
    def_temp=0;
    def_wind_speed=0;
    charge_rate = 1.5;

    chargetemp =0;
    energytemp =0;
    settime="";
    startttime="";
    endtime="";
    onceWakedUp=0;
}

int Request::TempChange()
{
    if(getRequestState() == RESPONSED)
    {
        if(cur_temp == goaltemp)
        {
           state = SLEEP;
            return 1;
        }
        else if(cur_temp > goaltemp)
        {
            cur_temp = cur_temp - 1;

        }
        else
        {
            cur_temp = cur_temp + 1;

        }

        PowerChange();
        MoneyChange();//1 need to revise
    }
    else
    {
         if(ENVIROTEMP == cur_temp)
         {}
         else if(ENVIROTEMP < cur_temp)
         {
             cur_temp -= 1;

         }
         else
         {
             cur_temp += 1;
         }

    }
    return 0;
    //更新数据库curtemp值
   //modifyEndTemp(room_num,  cur_temp);----shujuku
}

void Request::PowerChange()
{

   energy = energy + 0.5 * (speed + 1);
   energytemp = energytemp + 0.5*(speed+1);
    //更新数据库consumepower值
    //modifyPower(room_num,energy);  ------shujuku

}

void Request::MoneyChange()
{


            charge = charge_rate*energy;
            chargetemp = charge_rate*energytemp;

        //更新数据库money值
       // modifyMoney(room_num, charge);  ------shujuku
}

int Request::getRequestState()
{
    return state;
}


void Request::setState(int st)
{
    state = st;
}

void Request::setCurTemp(double ct)
{
    cur_temp = ct;
}

double Request::getCurTemp()
{
    return cur_temp;
}

int Request::getRequestNumber()
{
    return room_num;
}

double Request::getRequestGTemp()
{
    return goaltemp;
}

int Request::getRequestGSpeed()
{
    return speed;
}

QString Request::getRequestSetTime()
{
    return settime;
}

double Request::getCurMoney()
{
    return charge;
}


CentralCondition::CentralCondition(int rn, int m, int r, int lt, int ut)
{
    roomNumber = rn;
    mode = m;
    rule = r;
    lowtemp = lt;
    uptemp = ut;
    for(int i=0;i<5;i++)
    {
    request[i] = new Request(i,ENVIROTEMP,0,"2100-00-00 00:00:00",DEAD);

    }
    responsingNumber=0;
}

void CentralCondition::resetCCondition(int rn, int m, int r, int lt, int ut)
{
    roomNumber = rn;
    mode = m;
    rule = r;
    lowtemp = lt;
    uptemp = ut;
}


int CentralCondition::getMode()
{
    return mode;
}

int CentralCondition::getRule()
{
    return rule;
}

int CentralCondition::getLowtemp()
{
    return lowtemp;
}

int CentralCondition::getUptemp()
{
    return uptemp;
}

void CentralCondition::setRequestCurrTemp(int rn, double ct)
{
    request[rn-1]->setCurTemp(ct);
}

int CentralCondition::getRequestState(int rn)
{
    return request[rn-1]->getRequestState();
}


void CentralCondition::setRequestState(int rn, int s)
{
    request[rn-1]->setState(s);
}

Request CentralCondition::getRequest(int rn)
{
    return *request[rn-1];
}

void CentralCondition::setMode(int i)
{
    mode =i;
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
    qDebug()<<is_init<<" "<<charge<<" "<<charge_rate<<" "<<cur_temp<<" "<<def_temp<<" "<<def_wind_speed<<" "<<energy<<" "<<mode<<" "<<room_num<<" "<<state<<endl;
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
    //qDebug()<<target;
    string array[15];
    int len = 0;
    string temp = "";
    for (int i=0; i<(int)target.length(); i++) {
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

int CentralCondition::scanRequest()
{
    int toberesponsed1 = 5;
    QString earliest1 = "";
    int fastest=-1;

    for(int i= 0;i<5;i++)
    {
        //find waiting for the longest one, switch its state to responsed
        if(request[i]->getRequestState() == WAITING && earliest1 == "")
        {
            toberesponsed1 = i;
            earliest1 = request[i]->getRequestSetTime();
            fastest= request[i]->speed;
        }
        else if(request[i]->getRequestState()==WAITING) //
        {
            if(request[i]->speed > fastest)
            {
                toberesponsed1 = i;
                earliest1 = request[i]->getRequestSetTime();
                fastest = request[i]->speed;
            }
            else if(request[i]->speed == fastest)
            {
                if(request[i]->getRequestSetTime()<earliest1)
                {
                    toberesponsed1 = i;
                    earliest1 = request[i]->getRequestSetTime();
                    fastest = request[i]->speed;
                }

            }
        }
    }

    if(toberesponsed1 != 5)
    {
        if(getResponseNumber()<3)
        {
        request[toberesponsed1]->setState(RESPONSED);
//if(request[toberesponsed1]->onceWakedUp == 0)//starttime bug
//{
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        request[toberesponsed1]->startttime =str;
        addStartTime(request[toberesponsed1]);
//}

        }
        return 1;
    }
    return 0;
}

int CentralCondition::getResponseNumber()
{
    int num=0;
    for(int i=0;i<5;i++)
    {
        if(request[i]->state==RESPONSED)
            num++;
    }
    return num;
}
