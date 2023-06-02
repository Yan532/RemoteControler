#ifndef RECVIMAGE_H
#define RECVIMAGE_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QTcpSocket>

class recvImage : public QThread
{
    Q_OBJECT
public:
    explicit recvImage(QTcpSocket *socket,QObject *parent = nullptr);
    ~recvImage();
signals:
    void Done();
    void recvOk(QString *);
protected:
    void run() override;
private:
    QTcpSocket *Socket;
    qint64 totalBytes;     // 存放总大小信息
    qint64 bytesReceived;  // 已收到数据的大小
    qint64 imageSize;      //图片大小

    QByteArray inBlock;    // 数据缓冲区
    QString imageContent;

};

#endif // RECVIMAGE_H
