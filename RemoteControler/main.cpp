#include "Startwidget.h"
#include "Blockstruct.h"
#include "RemoteEvent.h"
#include "Controler.h"
#include "Controltee.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<DataBlock>("DataBlock");
    qRegisterMetaType<QAbstractSocket::SocketState>("SocketState");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<RemoteEvent>("RemoteEvent");
    qRegisterMetaType<QString>("QString");
    Controltee *controltee = new Controltee;
    Controller *controller = new Controller;
    StartWidget *widget = new StartWidget(controller,controltee);
    widget->show();

    return a.exec();
}
