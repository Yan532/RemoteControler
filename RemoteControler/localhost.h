#ifndef LOCALHOST_H
#define LOCALHOST_H

#include <QObject>

class QHostAddress;
class Localhost : public QObject
{
    Q_OBJECT

public:
    Localhost(QObject *parent = nullptr);

    static bool isLocalAddress(const QHostAddress &address);
    static QString getLocalIpAddress();
};

#endif // LOCALHOST_H
