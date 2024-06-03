#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class UdpClient : public QObject
{
    Q_OBJECT
public:
    UdpClient();

public slots:
    void process();
    void stop();
    void read();

signals:
    void finished();
    void dataRecieved(QString data);
    void receivingPort(quint16 port);

private:
    QUdpSocket *m_socket;
};

#endif // UDPCLIENT_H
