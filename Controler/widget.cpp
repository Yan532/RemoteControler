#include "widget.h"
#include "ui_widget.h"
#include <QCompleter>
#include "recvImage.h"
#include "imagewidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , Server{new QTcpServer(this)}
{
    ui->setupUi(this);
    ImageWidget *imagewidget = new ImageWidget;
    imagewidget->show();

    ui->portLineEdit->setPlaceholderText(tr("6666")); // 设置默认提示
    QStringList portWordList;
    portWordList << tr("6666")<<tr("7777");
    QCompleter *portCompleter = new QCompleter(portWordList, this);
    ui->portLineEdit->setCompleter(portCompleter);

    connect(&Server, &QTcpServer::newConnection,this,[=](){
        Socket = Server.nextPendingConnection();
        connect(Socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(displayError(QAbstractSocket::SocketError)));
        ui->serverStatusLabel->setText(tr("接受连接"));
        recvImage *recvimage = new recvImage(Socket);
        connect(recvimage,&recvImage::recvOk,this,&Widget::getimagedata);
        recvimage->start();
    });

    connect(this,SIGNAL(labelsign()),this,SLOT(setLabel()));

    ui->imageLabel->show();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::start() {

    if (!Server.listen(QHostAddress::Any,ui->portLineEdit->text().toInt())) {
        qDebug() << Server.errorString();
        close();
        return;
    }
    ui->serverStatusLabel->setText(tr("正在监听"));
}

void Widget::getimagedata(QString *imagecontent){
    imagedatas.enqueue(imagecontent);
    qDebug()<<"add a image data";
    emit labelsign();
}

void Widget::setLabel(){
    QImage image;
    while (!imagedatas.empty()) {
        image = getImage();
        QPixmap resImage = QPixmap::fromImage(image);
        QPixmap *imgPointer = &resImage;
        imgPointer->scaled(1920,1080,Qt::KeepAspectRatio,Qt::SmoothTransformation); // 重新调整图像大小以适应窗口
        // imgPointer->scaled(ui->imageLabel->size(),
        // Qt::KeepAspectRatio);//设置pixmap缩放的尺寸
        ui->imageLabel->setScaledContents(true); // 设置label的属性,能够缩放pixmap充满整个可用的空间。
        ui->imageLabel->setPixmap(*imgPointer);
        ui->serverStatusLabel->setText(tr("接收文件成功"));
    }
}

QImage Widget::getImage(){
    QString *data = imagedatas.dequeue();
    QByteArray imageData = QByteArray::fromBase64(data->toLatin1());
    QImage image;
    image.loadFromData(imageData);
    return image;
}

void Widget::displayError(QAbstractSocket::SocketError socketError) {
    qDebug() << "errorString()" << Socket->errorString();
    Socket->close();

    ui->serverStatusLabel->setText(tr("服务端就绪"));
}

void Widget::on_startButton_clicked() {
  if (ui->startButton->text() == tr("监听")) {
    ui->startButton->setText(tr("断开"));
    start();
  } else {
    ui->startButton->setText(tr("监听"));
    Server.close();
    Socket->disconnectFromHost();
  }
}
