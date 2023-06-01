#ifndef CONTROLTEE_H
#define CONTROLTEE_H

#include <QObject>

class Controltee : public QObject
{
    Q_OBJECT
public:
    explicit Controltee(QObject *parent = nullptr);

signals:

};

#endif // CONTROLTEE_H
