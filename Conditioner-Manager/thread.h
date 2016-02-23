#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <iostream>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();
    void setMessage(QString message);
    void stop();

protected:
    void run();
    void sendMessage();

 private:
     QString messageStr;
     volatile bool stopped;
 };

#endif // THREAD_H
