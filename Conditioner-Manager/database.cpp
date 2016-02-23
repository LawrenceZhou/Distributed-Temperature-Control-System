#include "database.h"
#include <QtCore>
#include <QtDebug>
#include <QTextCodec>
#include <QMessageBox>
#include <QTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QObject>
#include "DataPact.h"
#include <QDateTime>
#include <QDebug>

bool createConnection()
{
    QFile::remove("request.db");
    bool IsExist = QFile::exists("request.db");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("request.db");
    db.open();

    if (!IsExist)
    {
        QSqlQuery query;
        qDebug() << "Start to create table...";
        bool success=query.exec("create table request_table(requestNumber int primary key,roomNumber int, goalTemp vchar, goalSpeed vchar, initialTemp int, endTemp int, charge double, energy double, chargeRate double ,setTime vchar, startTime vchar, endTime vchar,mode vchar,databaseState int )");
        if (success)
        {
            qDebug() << "数据库表创建成功！";
            return true;
        }
        else
        {
            qDebug()<< query.lastError();
            qDebug() << "数据库表创建失败！";
        }
        return false;
    }
    else
    {
        qDebug()<<"数据库已存在！";
    }

    return true;
}

bool addRequestDB( Request *R, CentralCondition* C)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("request.db");
    if (!db.isOpen())
    {
        createConnection();
    }


    QSqlQuery query;
    qDebug() << "start to insert data";
    query.prepare("insert into request_table(requestNumber,roomNumber, goalTemp, goalSpeed, initialTemp, endTemp, charge, energy,chargeRate,setTime, startTime, endTime,mode,databaseState)"" VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(returnRowNumber()+1);
    query.addBindValue(R->room_num+1);
    query.addBindValue(R->goaltemp);
    if(R->speed==0)
    {
        query.addBindValue("低速");
    }
    else if(R->speed==1)
    {
        query.addBindValue("中速");
    }
    else if(R->speed==2)
    {
        query.addBindValue("高速");
    }
    query.addBindValue(R->cur_temp);
    query.addBindValue(0);//endTemp
    query.addBindValue(0);//charge
    query.addBindValue(0);//energy
    query.addBindValue(CHARGERATE);
    query.addBindValue(R->settime);
    query.addBindValue("");
    query.addBindValue("");
    if(C->getMode()==COOL){
        query.addBindValue("制冷");
    }
    else
    {
        query.addBindValue("加热");
    }
    query.addBindValue(NOSTARTTIME);
    bool success=query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        qDebug()<<lastError.driverText()<<"charushibai";
        return false;
    }
    return true;
}

bool addStartTime(Request *R)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("request.db");
    if (!db.isOpen())
    {
        createConnection();
    }
    QSqlTableModel model;
    model.setTable("request_table");
    model.setFilter(QObject::tr("roomNumber=%1 and databaseState =%2 ").arg(R->room_num+1).arg(NOSTARTTIME));
    model.select();


    model.setData(model.index(0,10),R->startttime);
    model.setData(model.index(0,13),NOEND);

    qDebug()<<model.rowCount()<<model.record(0);
    if(model.submitAll())
    {
        qDebug()<<"addStartTime信息更改成功！";
        return true;
    }

    else
    {
        qDebug()<<"addStartTime信息更改失败！";
        return false;
    }
}

bool endRequest(Request *R)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("request.db");
    if (!db.isOpen())
    {
        createConnection();
    }
    QSqlTableModel model;
    model.setTable("request_table");
    model.setFilter(QObject::tr("roomNumber=%1 and databaseState =%2 ").arg(R->room_num+1).arg(NOEND));
    model.select();

    model.setData(model.index(0,5),R->cur_temp);
    model.setData(model.index(0,6),R->chargetemp);
    model.setData(model.index(0,7),R->energytemp);
    model.setData(model.index(0,11),R->endtime);
    model.setData(model.index(0,13),COMPLETE);

    qDebug()<<model.record(0);

    if(model.submitAll())
    {
        qDebug()<<"endRequest信息更改成功！";
        return true;
    }

    else
    {
        qDebug()<<"endRequest信息更改失败！";
        return false;
    }
}

int returnRowNumber()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("request.db");
    if (!db.isOpen())
    {
        createConnection();
    }
        QSqlQuery query;
        query.exec("SELECT * FROM request_table");
        query.last();
        return query.value(0).toInt();
}
