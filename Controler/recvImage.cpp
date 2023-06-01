#include "recvImage.h"

recvImage::recvImage(QTcpSocket *socket,QObject *parent)
    : QThread{parent}
    , Socket{socket}
    , totalBytes{0}
    , bytesReceived{0}
    , imageSize{0}
{

}

void recvImage::run(){
    qDebug()<<"run thread";
    connect(Socket,&QTcpSocket::readyRead,this,[=]{
        QDataStream in(Socket);
        in.setVersion(QDataStream::Qt_6_5);

        // 如果接收到的数据小于16个字节，保存到来的文件头结构
        if (bytesReceived <= sizeof(qint64) * 2) {
            if ((Socket->bytesAvailable() >= sizeof(qint64) * 2) &&
                (imageSize == 0)) {
                // 接收数据总大小信息和文件大小信息
                in >> totalBytes >> imageSize;
                bytesReceived += sizeof(qint64) * 2;

                qDebug() << "定位点0"
                         << "\n";
            }
            if ((Socket->bytesAvailable() >= imageSize) &&
                (imageSize != 0)) {

                // 读取图片数据
                in >> imageContent;
                QString *image = &imageContent;

                bytesReceived += imageSize;

                qDebug() << "定位1  bytesReceived: " << bytesReceived << "\n";

                if (bytesReceived == totalBytes) {
                    totalBytes = 0;
                    bytesReceived = 0;
                    imageSize = 0;
                    emit recvOk(image);
                }
            }
        }
    });
    exec();
}
