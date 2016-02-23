#include "recordview.h"
#include "ui_recordview.h"
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "database.h"
#include "DataPact.h"

RecordView::RecordView(int type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordView)
{
    QString str;
    ui->setupUi(this);
    if(type==DAY)
    {
        setWindowTitle("日报表");
        QDateTime time = QDateTime::currentDateTime();
        str = time.toString("yyyy-MM-dd");
    }
    else if(type==WEEK)
    {
        setWindowTitle("周报表");
        QDateTime time = QDateTime::currentDateTime();
        QDateTime  time0 = time.addDays(-6);
        str = time0.toString("yyyy-MM-dd");
    }
    else if(type==MONTH)
    {
        setWindowTitle("月报表");
        QDateTime time = QDateTime::currentDateTime();
        str = time.toString("yyyy-MM");
    }


    setFixedSize(850,580);
    QSqlQueryModel *mode1 = new QSqlQueryModel(ui->tableView1);   //建立一个Model
    mode1->setQuery(QObject::tr("select * from request_table where roomNumber=%1 and setTime > '%2'").arg(1).arg(str) );
    mode1->setHeaderData(0,Qt::Horizontal,QObject::tr("编号"));
    mode1->setHeaderData(1,Qt::Horizontal,QObject::tr("房间号"));
    mode1->setHeaderData(2,Qt::Horizontal,QObject::tr("目标温度"));
    mode1->setHeaderData(3,Qt::Horizontal,QObject::tr("目标风速"));
    mode1->setHeaderData(4,Qt::Horizontal,QObject::tr("起始温度"));
    mode1->setHeaderData(5,Qt::Horizontal,QObject::tr("结束温度"));
    mode1->setHeaderData(6,Qt::Horizontal,QObject::tr("费用"));
    mode1->setHeaderData(7,Qt::Horizontal,QObject::tr("功耗"));
    mode1->setHeaderData(8,Qt::Horizontal,QObject::tr("费率"));
    mode1->setHeaderData(9,Qt::Horizontal,QObject::tr("建立时间"));
    mode1->setHeaderData(10,Qt::Horizontal,QObject::tr("开始时间"));
    mode1->setHeaderData(11,Qt::Horizontal,QObject::tr("结束时间"));
    mode1->setHeaderData(12,Qt::Horizontal,QObject::tr("模式"));
    mode1->setHeaderData(13,Qt::Horizontal,QObject::tr("状态"));
    ui->tableView1->setModel(mode1);

    QSqlQueryModel *mode2 = new QSqlQueryModel(ui->tableView2);   //建立一个Model
    mode2->setQuery(QObject::tr("select * from request_table where roomNumber=%1 and setTime > '%2'").arg(2).arg(str) );
    mode2->setHeaderData(0,Qt::Horizontal,QObject::tr("编号"));
    mode2->setHeaderData(1,Qt::Horizontal,QObject::tr("房间号"));
    mode2->setHeaderData(2,Qt::Horizontal,QObject::tr("目标温度"));
    mode2->setHeaderData(3,Qt::Horizontal,QObject::tr("目标风速"));
    mode2->setHeaderData(4,Qt::Horizontal,QObject::tr("起始温度"));
    mode2->setHeaderData(5,Qt::Horizontal,QObject::tr("结束温度"));
    mode2->setHeaderData(6,Qt::Horizontal,QObject::tr("费用"));
    mode2->setHeaderData(7,Qt::Horizontal,QObject::tr("功耗"));
    mode2->setHeaderData(8,Qt::Horizontal,QObject::tr("费率"));
    mode2->setHeaderData(9,Qt::Horizontal,QObject::tr("建立时间"));
    mode2->setHeaderData(10,Qt::Horizontal,QObject::tr("开始时间"));
    mode2->setHeaderData(11,Qt::Horizontal,QObject::tr("结束时间"));
    mode2->setHeaderData(12,Qt::Horizontal,QObject::tr("模式"));
    mode2->setHeaderData(13,Qt::Horizontal,QObject::tr("状态"));
    ui->tableView2->setModel(mode2);

    QSqlQueryModel *mode3  = new QSqlQueryModel(ui->tableView3);   //建立一个Model
    mode3->setQuery(QObject::tr("select * from request_table where roomNumber=%1 and setTime > '%2'").arg(3).arg(str)) ;
    mode3->setHeaderData(0,Qt::Horizontal,QObject::tr("编号"));
    mode3->setHeaderData(1,Qt::Horizontal,QObject::tr("房间号"));
    mode3->setHeaderData(2,Qt::Horizontal,QObject::tr("目标温度"));
    mode3->setHeaderData(3,Qt::Horizontal,QObject::tr("目标风速"));
    mode3->setHeaderData(4,Qt::Horizontal,QObject::tr("起始温度"));
    mode3->setHeaderData(5,Qt::Horizontal,QObject::tr("结束温度"));
    mode3->setHeaderData(6,Qt::Horizontal,QObject::tr("费用"));
    mode3->setHeaderData(7,Qt::Horizontal,QObject::tr("功耗"));
    mode3->setHeaderData(8,Qt::Horizontal,QObject::tr("费率"));
    mode3->setHeaderData(9,Qt::Horizontal,QObject::tr("建立时间"));
    mode3->setHeaderData(10,Qt::Horizontal,QObject::tr("开始时间"));
    mode3->setHeaderData(11,Qt::Horizontal,QObject::tr("结束时间"));
    mode3->setHeaderData(12,Qt::Horizontal,QObject::tr("模式"));
    mode3->setHeaderData(13,Qt::Horizontal,QObject::tr("状态"));
    ui->tableView3->setModel(mode3);

    QSqlQueryModel *mode4  = new QSqlQueryModel(ui->tableView4);   //建立一个Model
    mode4->setQuery(QObject::tr("select * from request_table where roomNumber=%1 and setTime > '%2'").arg(4).arg(str)) ;
    mode4->setHeaderData(0,Qt::Horizontal,QObject::tr("编号"));
    mode4->setHeaderData(1,Qt::Horizontal,QObject::tr("房间号"));
    mode4->setHeaderData(2,Qt::Horizontal,QObject::tr("目标温度"));
    mode4->setHeaderData(3,Qt::Horizontal,QObject::tr("目标风速"));
    mode4->setHeaderData(4,Qt::Horizontal,QObject::tr("起始温度"));
    mode4->setHeaderData(5,Qt::Horizontal,QObject::tr("结束温度"));
    mode4->setHeaderData(6,Qt::Horizontal,QObject::tr("费用"));
    mode4->setHeaderData(7,Qt::Horizontal,QObject::tr("功耗"));
    mode4->setHeaderData(8,Qt::Horizontal,QObject::tr("费率"));
    mode4->setHeaderData(9,Qt::Horizontal,QObject::tr("建立时间"));
    mode4->setHeaderData(10,Qt::Horizontal,QObject::tr("开始时间"));
    mode4->setHeaderData(11,Qt::Horizontal,QObject::tr("结束时间"));
    mode4->setHeaderData(12,Qt::Horizontal,QObject::tr("模式"));
    mode4->setHeaderData(13,Qt::Horizontal,QObject::tr("状态"));
    ui->tableView4->setModel(mode4);

    QSqlQueryModel *mode5  = new QSqlQueryModel(ui->tableView5);   //建立一个Model
    mode5->setQuery(QObject::tr("select * from request_table where roomNumber=%1 and setTime > '%2'").arg(5).arg(str)) ;
    mode5->setHeaderData(0,Qt::Horizontal,QObject::tr("编号"));
    mode5->setHeaderData(1,Qt::Horizontal,QObject::tr("房间号"));
    mode5->setHeaderData(2,Qt::Horizontal,QObject::tr("目标温度"));
    mode5->setHeaderData(3,Qt::Horizontal,QObject::tr("目标风速"));
    mode5->setHeaderData(4,Qt::Horizontal,QObject::tr("起始温度"));
    mode5->setHeaderData(5,Qt::Horizontal,QObject::tr("结束温度"));
    mode5->setHeaderData(6,Qt::Horizontal,QObject::tr("费用"));
    mode5->setHeaderData(7,Qt::Horizontal,QObject::tr("功耗"));
    mode5->setHeaderData(8,Qt::Horizontal,QObject::tr("费率"));
    mode5->setHeaderData(9,Qt::Horizontal,QObject::tr("建立时间"));
    mode5->setHeaderData(10,Qt::Horizontal,QObject::tr("开始时间"));
    mode5->setHeaderData(11,Qt::Horizontal,QObject::tr("结束时间"));
    mode5->setHeaderData(12,Qt::Horizontal,QObject::tr("模式"));
    mode5->setHeaderData(13,Qt::Horizontal,QObject::tr("状态"));
    ui->tableView5->setModel(mode5);


}

RecordView::~RecordView()
{
    delete ui;
}
