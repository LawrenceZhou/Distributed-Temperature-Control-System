#include "room.h"
#include <QString>

Room::Room(QString rm)
{
    roomNumber= rm;
    roomState = OFF;
    default_set = NOT;
}

QString Room::getRoomNumber()
{
    return roomNumber;
}

int Room::getRoomState()
{
    return roomState;
}

void Room::setRoomState(int s)
{
    roomState = s;
}

/*void Room::tempUpChange(double initTemp)
{
    roomTemp = initTemp + 0.1;
}

void Room::tempDownChange(double initTemp)
{
    roomTemp = initTemp - 0.1;
}*/

double Room::getRoomTemp()
{
    return roomTemp;
}

void Room::setGoalSpeed(int gs)
{
    goalSpeed = gs;
}

void Room::setGoalTemp(int gt)
{
    goalTemp = gt;
}

void Room::setMode(int m)
{
    mode = m;
}

int Room::getRoomDefaultSet()
{
    return default_set;
}

void Room::setRoomDefaultSet(int ds)
{
    default_set = ds;
}
