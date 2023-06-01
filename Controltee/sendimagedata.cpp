#include "sendimagedata.h"
#include <QApplication>
#include <QBuffer>


sendImagedata::sendImagedata(QObject *parent)
    : QObject{parent}
    ,isOk{false}
{}

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
  screenshot = screenshot.scaled(1920,1080,Qt::KeepAspectRatio,Qt::FastTransformation)
               .scaled(1280,720,Qt::KeepAspectRatio,Qt::SmoothTransformation);
  QImage image = screenshot.toImage();
  QString imageData = getImageData(image);

  qDebug() << "fileName: " << fileName << "\n";
  //    qDebug() << "imageData" << imageData << endl;

  // 保留总大小信息空间、图像大小信息空间，然后输入图像信息
  sendOut << qint64(0) << qint64(0) << imageData;

  // 这里的总大小是总大小信息、图像大小信息和实际图像信息的总和
  totalBytes += outBlock.size();
  sendOut.device()->seek(0);

  // 返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
  sendOut << totalBytes << qint64((outBlock.size() - sizeof(qint64) * 2));


  // 发出readyRead（）信号
  Socket->write(outBlock);

  qDebug() << "图片的内容大小: "
           << qint64((outBlock.size() - sizeof(qint64) * 2)) << "\n";
  qDebug() << "整个包的大小: " << totalBytes << "\n";
  //    qDebug() << "发送完文件头结构后剩余数据的大小(bytesToWrite): " <<
  //    bytesToWrite <<endl;

  outBlock.resize(0);

  totalBytes = 0;
  //    bytesToWrite = 0;
}

QByteArray sendImagedata::getImageData(const QImage &image) {
  QByteArray imageData;
  QBuffer buffer(&imageData);
  image.save(&buffer, "JPEG",25);
  imageData = imageData.toBase64();

  return imageData;
}

void sendImagedata::displayError(QAbstractSocket::SocketError) {
  qDebug() << Socket->errorString();
  Socket->close();
}
