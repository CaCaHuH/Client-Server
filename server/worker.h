#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QUuid>
#include <calculate.h>

struct DATA_WORKER {
    DATA_WORKER(float one,float two, float three, float four, float five, float six, float seven, float eight, float nine, float ten, float eleven, float twelve)
    {
        h = one;
        y0 = two;
        v0 = three;
        tetta0 = four;
        T = five;
        m0 = six;
        cx0 = seven;
        cya = eight;
        A = nine;
        tr = ten;
        mc = eleven;
        d = twelve;
        m_validate = true;
    }
    DATA_WORKER() {

    }
    float h;
    float y0;
    float v0;
    float tetta0;
    float T;
    float m0;
    float cx0;
    float cya;
    float A;
    float tr;
    float mc;
    float d;
    bool m_validate = false;
};

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(DATA_WORKER data, QUuid id, qint64 counter);
    ~Worker();

public slots:
    void process();
    void stop();

signals:
    void finished();
    void resultCalculate(QUuid id,QByteArray data);

private:
    QUuid m_id;
    DATA_WORKER m_data;
    qint64 m_counter;
};

#endif // WORKER_H
