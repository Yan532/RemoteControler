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

    //void keyPressEvent(QKeyEvent *e);
    //void keyReleaseEvent(QKeyEvent *e);

    Q_INVOKABLE void finish();
    Q_INVOKABLE void mousePressed(const QPoint &position);
    Q_INVOKABLE void mouseReleased(const QPoint &position);
    Q_INVOKABLE void mouseMoved(const QPoint &position);
    Q_INVOKABLE void requestNewConnection(const QString &address);
    //Q_INVOKABLE void KeyInputed(int key);
    //int Translatekey(int key);

signals:
    void connected();
    void disconnected();
    void needUpdate();

private:
    inline void sendRemoteEvent(RemoteEvent::EventType type, const QPoint &position);

    Socket *m_socket;
    ImageWidget *imagewidget = nullptr;
};

#endif // CONTROLER_H
