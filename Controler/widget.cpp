#include "widget.h"
#include "ui_widget.h"
#include <QCompleter>
#include "recvImage.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , Server{new QTcpServer(this)}
    , imagelabel{new QLabel}
    , layout{new QVBoxLayout}
{
    ui->setupUi(this);
    ui->portLineEdit->setPlaceholderText(tr("6666")); // 设置默认提示
    QStringList portWordList;
    portWordList << tr("6666")<<tr("7777");
    QCompleter *portCompleter = new QCompleter(portWordList, this);
    ui->portLineEdit->setCompleter(portCompleter);
    ui->startButton->setEnabled(true);

    layout->addWidget(imagelabel);
    this->setLayout(layout);
    imagelabel->hide();

    connect(&Server, &QTcpServer::newConnection,this,[=](){
        Socket = Server.nextPendingConnection();
        connect(Socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(displayError(QAbstractSocket::SocketError)));
        recvImage *recvimage = new recvImage(Socket);
        connect(recvimage,&recvImage::recvOk,this,&Widget::getimagedata);
        recvimage->start();
        ui->startButton->hide();
        ui->portLineEdit->hide();
        ui->label_3->hide();
        imagelabel->show();
    });

    connect(this,SIGNAL(labelsign()),this,SLOT(setLabel()));
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
        imgPointer->scaled(imgPointer->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation); // 重新调整图像大小以适应窗口
        // imgPointer->scaled(ui->imageLabel->size(),
        // Qt::KeepAspectRatio);//设置pixmap缩放的尺寸
        imagelabel->setScaledContents(true); // 设置label的属性,能够缩放pixmap充满整个可用的空间。
        imagelabel->setPixmap(*imgPointer);
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
