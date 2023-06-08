#include "Controltee.h"
#include "remoteevent.h"
#include "windows_eventapi.h"
#include "socket.h"
#include "listener.h"

#include <QBuffer>
#include <QApplication>
#include <QScreen>
#include <QTime>
#include <QThread>

Controltee::Controltee(QObject *parent)
    : QObject (parent)
{
    screenshot_listener = new Listener(43801);
    event_listener = new Listener(43802);

    screenshot_socket = new Socket;
    event_socket = new Socket;

    connect(screenshot_listener,&Listener::send_Descriptor,this,[this](qintptr socketDescriptor){
        screenshot_socket->setSocketDescriptor(socketDescriptor);
        QThread *screenshot_thread = new QThread;
        screenshot_socket->moveToThread(screenshot_thread);
        connect(screenshot_thread, &QThread::finished, screenshot_thread, &QThread::deleteLater);
        screenshot_thread->start();
        if (!m_timerId)
            m_timerId = startTimer(std::chrono::milliseconds(200));
    });

    connect(event_listener,&Listener::send_Descriptor,this,[this](qintptr socketDescriptor){
        event_socket->setSocketDescriptor(socketDescriptor);
        QThread *event_thread = new QThread;
        event_socket->moveToThread(event_thread);
        connect(event_thread, &QThread::finished, event_thread, &QThread::deleteLater);
        event_thread->start();
    });

    connect(screenshot_socket, &Socket::stateChanged, this, [this](QAbstractSocket::SocketState socketState) {
        switch (socketState)
        {
        case QAbstractSocket::ConnectedState:
            emit connected();
            break;
        default:
            break;
        }
    });
    connect(event_socket, &Socket::stateChanged, this, [this](QAbstractSocket::SocketState socketState) {
        switch (socketState)
        {
        case QAbstractSocket::ConnectedState:
            emit connected();
            break;
        default:
            break;
        }
    });

    connect(screenshot_socket, &Socket::disconnected, this, [this]() {
        Socket *socket = screenshot_socket;
        screenshot_socket = nullptr;
        socket->deleteLater();
        killTimer(m_timerId);
        m_timerId = 0;
        emit disconnected();
    });
    connect(event_socket, &Socket::disconnected, this, [this]() {
        Socket *socket = event_socket;
        event_socket = nullptr;
        socket->deleteLater();
        emit disconnected();
    });

    connect(event_socket, &Socket::hasEventData, this, [this](const RemoteEvent &event) {
        processEvent(event);
    });
}

Controltee::~Controltee()
{

}

void Controltee::finish()
{
    if (screenshot_socket)
        QMetaObject::invokeMethod(screenshot_socket, "abort");
    if (event_socket)
        QMetaObject::invokeMethod(event_socket, "abort");
}

int Controltee::Translatekey(int k){
    int key = k;
    bool flag = true;
    if(key >= Qt::Key_0 && key <= Qt::Key_9)
    {
    }
    else if(key >= Qt::Key_A && key <= Qt::Key_Z)
    {
    }
    else if(key >= Qt::Key_F1 && key <= Qt::Key_F24)
    {
        key &= 0x000000ff;
        key += 0x40;
    }
    else if(key == Qt::Key_Tab)
    {
        key = 0x09;
    }
    else if(k == Qt::Key_Backspace)
    {
        key = 0x08;
    }
    else if(k == Qt::Key_Return)
    {
        key = 0x0d;
    }
    else if(key <= Qt::Key_Down && key >= Qt::Key_Left)
    {
        int off = key - Qt::Key_Left;
        key = 0x25 + off;
    }
    else if(key == Qt::Key_Shift)
    {
        key = 0x10;
    }
    else if(key == Qt::Key_Control)
    {
        key = 0x11;
    }
    else if(key == Qt::Key_Alt)
    {
        key = 0x12;
    }
    else if(key == Qt::Key_Meta)
    {
        key = 0x5b;
    }
    else if(key == Qt::Key_Insert)
    {
        key = 0x2d;
    }
    else if(key == Qt::Key_Delete)
    {
        key = 0x2e;
    }
    else if(key == Qt::Key_Home)
    {
        key = 0x24;
    }
    else if(key == Qt::Key_End)
    {
        key = 0x23;
    }
    else if(key == Qt::Key_PageUp)
    {
        key = 0x21;
    }
    else if(key == Qt::Key_Down)
    {
        key = 0x22;
    }
    else if(key == Qt::Key_CapsLock)
    {
        key = 0x14;
    }
    else if(key == Qt::Key_NumLock)
    {
        key = 0x90;
    }
    else if(key == Qt::Key_Space)
    {
        key = 0x20;
    }
    else
        flag = false;

    if(!flag){
        return 0;
    }else{
        return key;
    }
}

void Controltee::processEvent(const RemoteEvent &ev)
{
    QPoint localPos(ev.position().x(),
                     ev.position().y());
    int key = Translatekey(ev.getkey());

    switch (ev.type())
    {
    case RemoteEvent::EventType::LeftPressed:
        Windows_eventapi::mouseLeftPress(localPos);
        break;
    case RemoteEvent::EventType::RightPressed:
        Windows_eventapi::mouseRightPress(localPos);
        break;
    case RemoteEvent::EventType::LeftReleased:
        Windows_eventapi::mouseLeftRelease(localPos);
        break;
    case RemoteEvent::EventType::RightReleased:
        Windows_eventapi::mouseRightRelease(localPos);
        break;
    case RemoteEvent::EventType::Moved:
        Windows_eventapi::mouseMove(localPos);
        break;
    case RemoteEvent::EventType::KeyInput:
        Windows_eventapi::keyPress(key);
        break;
    case RemoteEvent::EventType::KeyUP:
        Windows_eventapi::keyRelease(key);
        break;
    default:
        break;
    }
}

void Controltee::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (screenshot_socket) {
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
            new_pixmap.save(&buffer, "jpeg", 25);
            BlockHeader header = { SCREEN_TYPE, qint32(buffer.size()) };
            DataBlock data = { header, buffer.data() };
            QMetaObject::invokeMethod(screenshot_socket, "writeToSocket", Q_ARG(DataBlock, data));
        }
    }
}
