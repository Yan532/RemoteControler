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
    m_socket = new Socket;
    m_socket->bind(QHostAddress::Any, 43800);
    QThread *thread = new QThread;
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(m_socket, &Socket::connected, this, &Controller::connected);
    connect(m_socket, &Socket::disconnected, this, &Controller::disconnected);
    connect(m_socket, &Socket::hasScreenData, this, [this](const QByteArray &screenData) {
        QPixmap pixmap;
        pixmap.loadFromData(screenData);
        imagewidget->setlabel_pixmap(pixmap);
        emit needUpdate();
    });

    imagewidget = new ImageWidget;
    connect(imagewidget,&ImageWidget::move_send,this,&Controller::mouseMoved);
    connect(imagewidget,&ImageWidget::press_send,this,&Controller::mousePressed);
    connect(imagewidget,&ImageWidget::release_send,this,&Controller::mouseReleased);

    m_socket->moveToThread(thread);
    thread->start();
}

void Controller::finish()
{
    QMetaObject::invokeMethod(m_socket, "abort");
}

void Controller::mousePressed(const QPoint &position)
{
    sendRemoteEvent(RemoteEvent::EventType::Pressed, position);
}

void Controller::mouseReleased(const QPoint &position)
{
    sendRemoteEvent(RemoteEvent::EventType::Released, position);
}

void Controller::mouseMoved(const QPoint &position)
{
    sendRemoteEvent(RemoteEvent::EventType::Moved, position);
}

void Controller::requestNewConnection(const QString &address)
{
    QHostAddress hostAddress(address);
    //有效且不为本机地址
    if (!hostAddress.isNull() && !Localhost::isLocalAddress(hostAddress)) {
        QMetaObject::invokeMethod(m_socket, "abort");
        QMetaObject::invokeMethod(m_socket, "connectHost", Q_ARG(QHostAddress, hostAddress), Q_ARG(quint16, 43800));
    }
}

void Controller::sendRemoteEvent(RemoteEvent::EventType type, const QPoint &position)
{
    RemoteEvent event(type, position);
    QMetaObject::invokeMethod(m_socket, "writeToSocket", Q_ARG(RemoteEvent, event));
}
