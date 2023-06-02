#include "sendimagedata.h"
#include <QApplication>
#include <QBuffer>
#include


sendImagedata::sendImagedata(QObject *parent)
    : QObject{parent}
    ,isOk{false}
{}

sendImagedata::~sendImagedata(){
    Socket->close();
    Socket->deleteLater();
    this->deleteLater();
}

void sendImagedata::connect_Server(QString ip,unsigned short port){
    Socket = new QTcpSocket;
    Socket->connectToHost(QHostAddress(ip),port);
    connect(Socket,&QTcpSocket::connected,this,&sendImagedata::tcpConnected);
    connect(Socket,&QTcpSocket::disconnected,this,&sendImagedata::tcpDisconnected);
}

void sendImagedata::tcpConnected(){
    isOk = true;
    emit hasconnect();
}

void sendImagedata::tcpDisconnected(){
    isOk = false;
    Socket->abort();
    emit notconnect();
}

void sendImagedata::startTransfer(){
  QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
  sendOut.setVersion(QDataStream::Qt_6_5);

  // 获得图片数据
  screen = QGuiApplication::primaryScreen();
  screenshot = screen->grabWindow(0);
  screenshot = screenshot.scaled(screenshot.size(),Qt::KeepAspectRatio,Qt::FastTransformation)
               .scaled(screenshot.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
  QImage image = screenshot.toImage();
  QString imageData = getImageData(image);

  // 保留总大小信息空间、图像大小信息空间，然后输入图像信息
  sendOut << qint64(0) << qint64(0) << imageData;

  // 这里的总大小是总大小信息、图像大小信息和实际图像信息的总和
  totalBytes += outBlock.size();
  qDebug()<< totalBytes;
  sendOut.device()->seek(0);

  // 返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
  sendOut << totalBytes << qint64((outBlock.size() - sizeof(qint64) * 2));

  // 发出readyRead（）信号
  Socket->write(outBlock);

  qDebug() << "图片的内容大小: "
           << qint64((outBlock.size() - sizeof(qint64) * 2));
  qDebug() << "整个包的大小: " << totalBytes;
  //    qDebug() << "发送完文件头结构后剩余数据的大小(bytesToWrite): " <<
  //    bytesToWrite <<endl;

  outBlock.clear();

  totalBytes = 0;
  //    bytesToWrite = 0;
}

QByteArray sendImagedata::getImageData(const QImage &image) {
  QByteArray imageData;
  QBuffer buffer(&imageData);
  image.save(&buffer, "JPEG",20);
  imageData = imageData.toBase64();

  return imageData;
}

void sendImagedata::displayError(QAbstractSocket::SocketError) {
  qDebug() << Socket->errorString();
  Socket->close();
}
