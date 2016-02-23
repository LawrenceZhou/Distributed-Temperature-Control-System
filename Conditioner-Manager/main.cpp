#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include "database.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login l;
    if(l.exec() == QDialog::Accepted)
        {
            createConnection();
            int rn = l.getRoomNumber();
            MainWindow w(rn);
             w.show();
             return a.exec();
        }

    return 0;
}
