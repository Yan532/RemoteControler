#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QTcpServer>

class Socket;

class Listener : public QTcpServer
{
    Q_OBJECT
public:
    explicit Listener(const quint16 &port,QObject *parent = nullptr);

signals:
    void send_Descriptor(qintptr socketDescriptor);

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // LISTENER_H
