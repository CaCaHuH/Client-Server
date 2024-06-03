#include "udpserver.h"
#include <QTime>
#include <QDataStream>

UdpServer::UdpServer() : QObject(nullptr)
{
    m_socket = nullptr;
}

void UdpServer::process()
{
    m_socket = new QUdpSocket(this);
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpServer::read);
    if (!m_socket->bind(QHostAddress::LocalHost, 36001))
        qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") << "Error could not start sever.";
    else qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") << "Server connected to listening port.";
}

void UdpServer::stop()
{
    delete m_socket;
    emit finished();
}

void UdpServer::read()
{
    QByteArray datagram;
    datagram.resize(m_socket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;

    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") << "Message recieved:";
    qDebug() <<"------------------------------------------------------------------------";
    m_socket->readDatagram(datagram.data(), datagram.size(), &sender,&senderPort);
    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << datagram << " size" <<datagram.size();

    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_0);
    qint64 size = -1;

    if(in.device()->size() > sizeof(qint64)) {
        in >> size;
    }
    else
        return;

    qDebug()<<"size" << size;
    if (in.device()->size() - sizeof(qint64) < size)
        return;
    qDebug() <<"------------------------------------------------------------------------";
    emit dataRecieved(datagram);
}
