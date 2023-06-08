#include "listener.h"
#include "Socket.h"

Listener::Listener(const quint16 &port,QObject *parent)
    : QTcpServer{parent}
{
    listen(QHostAddress::Any,port);
}

void Listener::incomingConnection(qintptr socketDescriptor){
    emit send_Descriptor(socketDescriptor);
}
