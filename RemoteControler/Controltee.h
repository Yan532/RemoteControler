#ifndef CONTROLTEE_H
#define CONTROLTEE_H

#include <QObject>
#include <QTcpServer>
#include <QPixmap>

class Socket;
class RemoteEvent;

class Controltee : public QTcpServer
{
    Q_OBJECT

public:
    explicit Controltee(QObject *parent = nullptr);
    ~Controltee();

    Q_INVOKABLE void finish();

signals:
    void connected();
    void disconnected();

public slots:
    void processEvent(const RemoteEvent &ev);

protected:
    void timerEvent(QTimerEvent *event);
    void incomingConnection(qintptr socketDescriptor);

private:
    Socket *m_controlled = nullptr;
    QPixmap new_pixmap;
    QPixmap old_pixmap;
    int m_timerId = 0;

};

#endif // CONTROLTEE_H
