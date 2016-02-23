#include "mainwindow.h"
#include "login.h"
#include "room.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login l;
    if(l.exec()==QDialog::Accepted)
        {
            QString rn;
            rn = l.getRoomNumber();
             Room r(rn);
            MainWindow w(r);
             w.show();
             return a.exec();
        }

    return 0;
}
