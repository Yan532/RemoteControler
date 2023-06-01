#ifndef SENDIMAGEDATA_H
#define SENDIMAGEDATA_H

#include <QObject>
#include <QThread>
#include <QScreen>
#include <QPixmap>
#include <QImage>
#include <QTcpSocket>

class sendImagedata : public QObject
{
    Q_OBJECT
public:
    explicit sendImagedata(QObject *parent = nullptr);

signals:
    void hasconnect();
    void notconnect();

private slots:
    void displayError(QAbstractSocket::SocketError);//处理错误函数
    void tcpConnected();
    void tcpDisconnected();//断开连接处理的事件

    //图片转base64字符串
    QByteArray getImageData(const QImage&);
public slots:
    void connect_Server(QString,unsigned short);
    void startTransfer();//发送图片数据

private:
    QTcpSocket *Socket;
    qint64 totalBytes;    // 发送数据的总大小
    QString fileName;     // 保存文件路径
    QByteArray outBlock;  // 数据缓冲区，即存放每次要发送的数据块

    QScreen *screen = nullptr;
    QPixmap screenshot;
    QImage image;//图片

    volatile bool isOk;

};

#endif // SENDIMAGEDATA_H
