#include "RemoteEvent.h"
#include "localhost.h"
#include "controler.h"
#include "socket.h"
#include "Imagewidget.h"

#include <QGuiApplication>
#include <QHostAddress>
#include <QThread>
#include <QPixmap>

Controller::Controller(QObject *parent)
    : QObject(parent)
{
    pixmap_socket = new Socket;
    pixmap_socket->bind(QHostAddress::Any, 43801);
    event_socket = new Socket;
    event_socket->bind(QHostAddress::Any, 43802);
    pixmap_thread = new QThread;
    event_thread = new QThread;
    connect(pixmap_thread, &QThread::finished, pixmap_thread, &QThread::deleteLater);
    connect(event_thread, &QThread::finished, event_thread, &QThread::deleteLater);
    connect(pixmap_socket, &Socket::connected, this, &Controller::connected);
    connect(pixmap_socket, &Socket::disconnected, this, &Controller::disconnected);
    connect(event_socket, &Socket::connected, this, &Controller::connected);
    connect(event_socket, &Socket::disconnected, this, &Controller::disconnected);
    connect(pixmap_socket, &Socket::hasScreenData, this, [this](const QByteArray &screenData) {
        QPixmap pixmap;
        pixmap.loadFromData(screenData);
        imagewidget->setlabel_pixmap(pixmap);
        emit needUpdate();
    });

    imagewidget = new ImageWidget;
    connect(imagewidget,&ImageWidget::leftPress_send,this,&Controller::mouseLeftPressed);
    connect(imagewidget,&ImageWidget::rightPress_send,this,&Controller::mouseRightPressed);
    connect(imagewidget,&ImageWidget::leftRelease_send,this,&Controller::mouseLeftReleased);
    connect(imagewidget,&ImageWidget::rightRelease_send,this,&Controller::mouseRightReleased);
    connect(imagewidget,&ImageWidget::keypress_send,this,&Controller::KeyInputed);
    connect(imagewidget,&ImageWidget::keyrelease_send,this,&Controller::KeyReleased);

    pixmap_socket->moveToThread(pixmap_thread);
    event_socket->moveToThread(event_thread);

    pixmap_thread->start();
    event_thread->start();
}

void Controller::finish()
{
    QMetaObject::invokeMethod(pixmap_socket, "abort");
    QMetaObject::invokeMethod(event_socket, "abort");
}

void Controller::mouseLeftPressed(const QPoint &position)
{
    sendRemoteEvent_m(RemoteEvent::EventType::LeftPressed, position);
}

void Controller::mouseRightPressed(const QPoint &position)
{
    sendRemoteEvent_m(RemoteEvent::EventType::RightPressed, position);
}

void Controller::mouseLeftReleased(const QPoint &position)
{
    sendRemoteEvent_m(RemoteEvent::EventType::LeftReleased, position);
}

void Controller::mouseRightReleased(const QPoint &position)
{
    sendRemoteEvent_m(RemoteEvent::EventType::RightReleased, position);
}

void Controller::mouseMoved(const QPoint &position)
{
    sendRemoteEvent_m(RemoteEvent::EventType::Moved, position);
}

void Controller::KeyInputed(const int &key){
    sendRemoteEvent_k(RemoteEvent::EventType::KeyInput,key);
}

void Controller::KeyReleased(const int &key){
    sendRemoteEvent_k(RemoteEvent::EventType::KeyUP,key);
}

void Controller::requestNewConnection(const QString &address)
{
    QHostAddress hostAddress(address);
    //有效且不为本机地址
    if (!hostAddress.isNull() && !Localhost::isLocalAddress(hostAddress)) {
        imagewidget->show();
        imagewidget->showMaximized();
        QMetaObject::invokeMethod(pixmap_socket, "abort");
        QMetaObject::invokeMethod(pixmap_socket, "connectHost", Q_ARG(QHostAddress, hostAddress), Q_ARG(quint16, 43801));
        QMetaObject::invokeMethod(event_socket, "abort");
        QMetaObject::invokeMethod(event_socket, "connectHost", Q_ARG(QHostAddress, hostAddress), Q_ARG(quint16, 43802));
    }
}

void Controller::sendRemoteEvent_m(RemoteEvent::EventType type, const QPoint &position)
{
    RemoteEvent event(type, position);
    QMetaObject::invokeMethod(event_socket, "writeToSocket", Q_ARG(RemoteEvent, event));
}

void Controller::sendRemoteEvent_k(RemoteEvent::EventType type, const int &key){
    RemoteEvent event(type, key);
    QMetaObject::invokeMethod(event_socket,"writeToSocket",Q_ARG(RemoteEvent, event));
}
