#ifndef CONTROLTEE_H
#define CONTROLTEE_H

#include <QObject>
#include <QTcpServer>
#include <QPixmap>

class Socket;
class RemoteEvent;
class Listener;

class Controltee : public QObject
{
    Q_OBJECT

public:
    explicit Controltee(QObject *parent = nullptr);
    ~Controltee();

    Q_INVOKABLE void finish();
    int Translatekey(int k);

signals:
    void connected();
    void disconnected();

public slots:
    void processEvent(const RemoteEvent &ev);

protected:
    void timerEvent(QTimerEvent *event);

private:
    Listener *screenshot_listener;
    Listener *event_listener;
    Socket *screenshot_socket;
    Socket *event_socket;
    QPixmap new_pixmap;
    QPixmap old_pixmap;
    int m_timerId = 0;

};

#endif // CONTROLTEE_H
