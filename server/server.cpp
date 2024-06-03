#include "server.h"
#include <QThread>
#include <QDebug>
#include <QDataStream>
#include <QLocalSocket>
#include <QTime>
#include <QUuid>

server::server(QObject *parent) : QObject(parent)
{
    m_socketSERV = nullptr;
}

server::~server()
{
    stopThreads();
}

bool server::start()
{
    UdpServer* serv = new UdpServer();
    QThread* thread = new QThread;
    if (!thread) return false;
    serv->moveToThread(thread);

    connect(thread,SIGNAL(started()),serv,SLOT(process()));

    connect(serv,SIGNAL(finished()),thread,SLOT(quit()));

    connect(this,SIGNAL(stopall()),serv,SLOT(stop()));

    connect(serv,SIGNAL(finished()),serv,SLOT(deleteLater()));

    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));

    connect(serv,SIGNAL(dataRecieved(QByteArray)),this,SLOT(dataRecieved(QByteArray)));

    thread->start();

    return true;
}

void server::checkReady(QMap<QString,QVector<QVector<QPair<REQUEST,RESPONSE>>>>::iterator iter1, QVector<QPair<REQUEST,RESPONSE>>* iter2)
{
    if (m_socketSERV == nullptr) m_socketSERV = new QUdpSocket(this);
    auto send = new QVector<QByteArray>;
    auto it = iter2->begin();
    while (it != iter2->end()) {
        if (it->second.sost) {
            send->push_back(it->second.res);
        }
        else {
            delete send;
            return;
        }
        it++;
    }
    auto it2 = send->begin();
    QString port;
    auto iterator = iter1.key().end()-5;
    for (int i = 0; i < 5; i++) {
        port.push_back(*iterator);
        iterator++;
    }
    while (it2 != send->end()) {
        m_socketSERV->writeDatagram(*it2, QHostAddress::LocalHost, quint16(port.toInt()));
        it2++;
    }
    delete send;
    iter1.value().erase(iter2);
    if (iter1.value().size() == 0) m_data.erase(iter1);
    qDebug() << "checkReady";
}

void server::dataRecieved(QByteArray msg)
{
    qDebug()<< QTime::currentTime().toString("hh:mm:ss:zzz") << "dataRecieve";
    if (msg.size()>0) {
        qDebug()<< QTime::currentTime().toString("hh:mm:ss:zzz") << "Start handle";

        QDataStream in(&msg, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_0);
        qint64 size;
        in >> size;
        qint8 type;
        in >> type;
        quint16 clientPort;
        in >> clientPort;
        if (type == 3) {
            if (m_socketSERV == nullptr)
                m_socketSERV = new QUdpSocket(this);
            QByteArray dataOut;
            QDataStream out(&dataOut, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_0);
            QString msg = "The server has performed " + QVariant(numOfCalculate).toString() + " calculations.";
            out << qint64(0);
            out << qint8(2);
            out << msg;
            out.device()->seek(qint64(0));
            out << qint64(dataOut.size() - sizeof(qint64));
            qDebug()<< QTime::currentTime().toString("hh:mm:ss:zzz") << " The server received a request of type 3. Reply letter size: " << dataOut.size();
            m_socketSERV->writeDatagram(dataOut, QHostAddress::LocalHost, clientPort);
            return;
        }
        QString address = "127.0.0.1:" + QVariant(clientPort).toString();
        auto iter = m_data.begin();
        for (iter; iter != m_data.end(); iter++) {
            if (address == iter.key()) {
                break;
            }
        }
        if (iter == m_data.end()) {
            QVector<QVector<QPair<REQUEST,RESPONSE>>> m_vec;
            m_data.insert(address,m_vec);
            iter = --m_data.end();
        }
        quint64 counter_zap;
        in >> counter_zap;
        float h, y0, v0, tetta0, T, m0, cx0, cya, A, tr, mc, d;
        qint8 kolvo;
        in >> h;
        in >> y0;
        in >> v0;
        in >> tetta0;
        in >> T;
        in >> m0;
        in >> cx0;
        in >> cya;
        in >> A;
        in >> tr;
        in >> mc;
        in >> d;
        in >> kolvo;
        QString msg;
        in >> msg;
        qDebug()<< QTime::currentTime().toString("hh:mm:ss:zzz");
        qDebug()<< " type = " << type
                << " size = " << size
                << " h = " << h
                << " y0 = " << y0
                << " v0 = " << v0
                << " tetta0 = " << tetta0
                << " T = " << T
                << " m0 = " << m0
                << " cx0 = " << cx0
                << " cya = " << cya
                << " A = " << A
                << " tr = " << tr
                << " mc = " << mc
                << " d = " << d
                << " kolvo = " << kolvo
                << " message from server" << msg;
        DATA_WORKER data(h, y0, v0, tetta0, T, m0, cx0, cya, A, tr, mc, d);
        QVector<QPair<REQUEST,RESPONSE>> vec_calc;
                for (int i = 0; i <= (kolvo - 1); i++)
        {
            QUuid id = uuid.createUuid();
            REQUEST m_req = REQUEST(id,data);
            RESPONSE m_res = RESPONSE();
            QPair<REQUEST,RESPONSE> calc = qMakePair(m_req, m_res);
            vec_calc.push_back(calc);
            addThread(data,id,counter_zap);
        }
        iter.value().push_back(vec_calc);
        qDebug()<< QTime::currentTime().toString("hh:mm:ss:zzz") << "Finish handle";
    }
}

void server::inc_calculate()
{
    numOfCalculate++;
}

void server::getCalculate(QUuid id, QByteArray dataOut)
{
    for (auto iter1 = m_data.begin(); iter1 != m_data.end(); iter1++) {
        for (auto iter2 = iter1.value().begin(); iter2 != iter1.value().end(); iter2++) {
            for (auto iter3 = iter2->begin(); iter3 !=iter2->end(); iter3++) {
                if (iter3->first.id == id) {
                    iter3->second = RESPONSE(id,dataOut);
                    checkReady(iter1,iter2);
                    return;
                }
            }
        }
    }
    qDebug() << "getCalculate";
}

void server::addThread(DATA_WORKER data, QUuid id, qint64 counter)
{
    Worker* worker = new Worker(data, id, counter);
    QThread* thread = new QThread;
    worker->moveToThread(thread);

    connect(thread,SIGNAL(started()),worker,SLOT(process()));

    connect(worker,SIGNAL(finished()),thread,SLOT(quit()));

    connect(this,SIGNAL(stopall()),worker,SLOT(stop()));

    connect(worker,SIGNAL(finished()),worker,SLOT(deleteLater()));

    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));

    connect(worker,SIGNAL(finished()),this,SLOT(inc_calculate()));

    connect(worker,SIGNAL(resultCalculate(QUuid, QByteArray)),this,SLOT(getCalculate(QUuid, QByteArray)));

    thread->start();
}

void server::stopThreads()
{
    emit stopall();
}
