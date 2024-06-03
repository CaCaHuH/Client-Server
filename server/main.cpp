#include <QCoreApplication>
#include <QDebug>
#include "server.h"
#include <QTime>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    server ser;
    if (ser.start())
    {
        qDebug()<< QTime::currentTime().toString("hh:mm:ss:zzz") <<"Server starting.";
    }
    return a.exec();
}
