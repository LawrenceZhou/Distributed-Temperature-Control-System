#ifndef DATABASE_H
#define DATABASE_H
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
#include "DataPact.h"


bool createConnection();
bool addRequestDB(Request* R, CentralCondition* C);
bool addStartTime(Request* R);
bool endRequest(Request* R);
int returnRowNumber();

#endif // DATABASE_H
