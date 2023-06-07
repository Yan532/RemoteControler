#include "Controltee.h"
#include "remoteevent.h"
#include "windows_eventapi.h"
#include "socket.h"

#include <QBuffer>
#include <QApplication>
#include <QScreen>
#include <QTime>
#include <QThread>

Controltee::Controltee(QObject *parent)
    : QTcpServer (parent)
{
    listen(QHostAddress::Any, 43800);

}

Controltee::~Controltee()
{

}

void Controltee::finish()
{
    if (m_controlled)
        QMetaObject::invokeMethod(m_controlled, "abort");
}

void Controltee::processEvent(const RemoteEvent &ev)
{
    QRect screenRect = qApp->primaryScreen()->geometry();
    QPoint localPos(ev.position().x() * screenRect.width(),
                     ev.position().y() * screenRect.height());

    switch (ev.type())
    {
    case RemoteEvent::EventType::Pressed:
        Windows_eventapi::mousePress(localPos);
        break;
    case RemoteEvent::EventType::Released:
        Windows_eventapi::mouseRelease(localPos);
        break;
    case RemoteEvent::EventType::Moved:
        Windows_eventapi::mouseMove(localPos);
        break;
    case RemoteEvent::EventType::KeyInput:
        break;
    default:
        break;
    }
}

void Controltee::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (m_controlled) {
        QApplication::setOverrideCursor(Qt::BlankCursor);   //隐藏被控端鼠标指针
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QTime time = QTime::currentTime();
        QScreen *screen = QGuiApplication::primaryScreen();
        new_pixmap = screen->grabWindow(0).scaled(screen->size(),Qt::KeepAspectRatio,Qt::FastTransformation)
                                          .scaled(screen->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        QApplication::overrideCursor();                     //恢复被控端鼠标指针显示
        if(old_pixmap.toImage() != new_pixmap.toImage()){
            old_pixmap = new_pixmap;
            qDebug() << time.msecsTo(QTime::currentTime());
            new_pixmap.save(&buffer, "jepg", 20);
            BlockHeader header = { SCREEN_TYPE, qint32(buffer.size()) };
            DataBlock data = { header, buffer.data() };
            QMetaObject::invokeMethod(m_controlled, "writeToSocket", Q_ARG(DataBlock, data));
        }


    }
}

void Controltee::incomingConnection(qintptr socketDescriptor)
{
    if (!m_controlled) {
        QThread *thread = new QThread;
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        m_controlled = new Socket;
        connect(m_controlled, &Socket::stateChanged, this, [this](QAbstractSocket::SocketState socketState) {
            switch (socketState)
            {
            case QAbstractSocket::ConnectedState:
                emit connected();
                break;
            default:
                break;
            }
        });
        connect(m_controlled, &Socket::disconnected, this, [this]() {
            Socket *socket = m_controlled;
            m_controlled = nullptr;
            socket->deleteLater();
            killTimer(m_timerId);
            m_timerId = 0;
            emit disconnected();
        });
        connect(m_controlled, &Socket::hasEventData, this, [this](const RemoteEvent &event) {
            processEvent(event);
        });
        m_controlled->setSocketDescriptor(socketDescriptor);
        m_controlled->moveToThread(thread);
        thread->start();

        if (!m_timerId)
            m_timerId = startTimer(std::chrono::milliseconds(30));
    }
}
