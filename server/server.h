#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include "worker.h"
#include "udpserver.h"
#include <QUuid>

struct REQUEST {
    REQUEST (QUuid id0, DATA_WORKER data0) {
        id = id0;
        data = data0;
    }
    REQUEST () {

    };
    QUuid id;
    DATA_WORKER data;
};

struct RESPONSE {
    RESPONSE (QUuid id0, QByteArray data0) {
        id = id0;
        res = data0;
        sost = true;
    }
    RESPONSE () {

    };
    QUuid id;
    bool sost = false;
    QByteArray res;
};

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);
    ~server();
    bool start();
    void checkReady(QMap<QString,QVector<QVector<QPair<REQUEST,RESPONSE>>>>::iterator iter1, QVector<QPair<REQUEST,RESPONSE>>* iter2);
signals:
    void stopall();
public slots:
    void dataRecieved(QByteArray msg);
    void inc_calculate();
    void getCalculate(QUuid id, QByteArray dataOut);
private:
    void addThread(DATA_WORKER data, QUuid id, qint64 counter);
    void stopThreads();
    QUuid uuid;
    int numOfCalculate = 0;
    QUdpSocket *m_socketSERV;
    QMap<QString,QVector<QVector<QPair<REQUEST,RESPONSE>>>> m_data;
};

#endif // SERVER_H
