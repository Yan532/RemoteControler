#ifndef CONTROLER_H
#define CONTROLER_H

#include "RemoteEvent.h"
#include <QObject>
#include <QKeyEvent>

class Socket;
class ImageWidget;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    Q_INVOKABLE void finish();
    Q_INVOKABLE void mouseLeftPressed(const QPoint &position);
    Q_INVOKABLE void mouseRightPressed(const QPoint &position);
    Q_INVOKABLE void mouseRightReleased(const QPoint &position);
    Q_INVOKABLE void mouseLeftReleased(const QPoint &position);
    Q_INVOKABLE void mouseMoved(const QPoint &position);
    Q_INVOKABLE void requestNewConnection(const QString &address);
    Q_INVOKABLE void KeyInputed(const int &key);
    Q_INVOKABLE void KeyReleased(const int &key);


signals:
    void connected();
    void disconnected();
    void needUpdate();

private:
    inline void sendRemoteEvent_m(RemoteEvent::EventType type, const QPoint &position);
    inline void sendRemoteEvent_k(RemoteEvent::EventType type, const int &key);

    Socket *pixmap_socket;
    Socket *event_socket;
    QThread *pixmap_thread;
    QThread *event_thread;
    ImageWidget *imagewidget = nullptr;
};

#endif // CONTROLER_H
