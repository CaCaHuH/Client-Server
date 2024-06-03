#include "udpclient.h"
#include <QDataStream>
#include <QTime>

UdpClient::UdpClient() : QObject(nullptr)
{
    m_socket = nullptr;
}

void UdpClient::process()
{
    m_socket = new QUdpSocket(this);
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpClient::read);
    m_socket->connectToHost(QHostAddress::LocalHost,0);
    emit receivingPort(m_socket->localPort());
    if (m_socket->localPort()) qDebug() << "Client got a receiving port: " << QVariant(m_socket->localPort()).toString();
    else qDebug() << "Error. Client not got a receiviing port";
}

void UdpClient::stop()
{
    delete m_socket;
    emit finished();
}

void UdpClient::read()
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
    else return;

    qDebug()<< "size" << size;
    if (in.device()->size() - sizeof(qint64) < size)
        return;

    qint8 type = 0;
    in >> type;
    QString msg;
    in >> msg;
    qDebug()<< "type" << type << "message from server" << msg;
    qDebug() <<"------------------------------------------------------------------------";
    emit dataRecieved(msg);
}
