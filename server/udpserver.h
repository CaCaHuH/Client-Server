#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>

class UdpServer : public QObject
{
    Q_OBJECT
public:
    UdpServer();

public slots:
    void process();
    void stop();
    void read();

signals:
    void finished();
    void dataRecieved(QByteArray data);

private:
    QUdpSocket *m_socket;
};

#endif // UDPSERVER_H
