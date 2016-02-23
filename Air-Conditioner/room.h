#ifndef ROOM_H
#define ROOM_H
#include <QString>

#define OFF 0
#define READY 1
#define ON 2
#define NOT 0
#define DONE 1

class Room
{

public:
    QString roomNumber;
    int roomState;
    int roomTemp;
    int goalSpeed;
    int goalTemp;
    int mode;
    int default_set;
    double money;


    Room(QString rm);
    QString getRoomNumber();
    int getRoomState();
    void setRoomState(int s);
    //void tempUpChange(double initTemp);
    //void tempDownChange(double initTemp);
    double getRoomTemp();
    void setGoalTemp(int gt);
    void setGoalSpeed(int gs);
    void setMode(int m);
    int getRoomDefaultSet();
    void setRoomDefaultSet(int ds);

};

#endif // ROOM_H
