#include "widget.h"
#include "ui_widget.h"
#include "sendimagedata.h"
#include <QCompleter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,timer{new QTimer}
{
    ui->setupUi(this);
    // 地址和端口自动补全以及默认提示
    QStringList hostWordList, portWordList;
    hostWordList << tr("127.0.0.1");
    portWordList << tr("6666");
    QCompleter *completerHost = new QCompleter(hostWordList, this);
    QCompleter *completerPort = new QCompleter(portWordList, this);

    ui->hostLineEdit->setCompleter(completerHost);
    ui->portLineEdit->setCompleter(completerPort);
    ui->hostLineEdit->setPlaceholderText(tr("127.0.0.1"));
    ui->portLineEdit->setPlaceholderText(tr("6666"));

    send_thread = new QThread(this);
    sendImagedata *sendimagedata = new sendImagedata;
    sendimagedata->moveToThread(send_thread);

    connect(this,&Widget::sendip_port,sendimagedata,&sendImagedata::connect_Server);
    connect(sendimagedata,&sendImagedata::hasconnect,this,&Widget::hasconnect);
    connect(sendimagedata,&sendImagedata::notconnect,this,&Widget::notconnect);
    connect(timer,&QTimer::timeout,sendimagedata,&sendImagedata::startTransfer);
}

Widget::~Widget()
{
    delete ui;
    timer->stop();
    send_thread->quit();
    send_thread->wait();
    send_thread->deleteLater();
}

void Widget::hasconnect(){
    ui->LinkButton->setText(tr("断开"));

    ui->clientStatusLabel->setText(tr("已连接"));
}

void Widget::notconnect(){
    ui->LinkButton->setText(tr("连接"));

    ui->clientStatusLabel->setText(tr("连接已断开"));
}


void Widget::on_LinkButton_clicked()
{
  if (ui->LinkButton->text() == tr("连接")) {
      QString ip = ui->hostLineEdit->text();
      unsigned short port = ui->portLineEdit->text().toUShort();
      emit sendip_port(ip,port);
  }else{
      timer->stop();
      send_thread->quit();
      send_thread->wait();
      send_thread->deleteLater();
  }
}


void Widget::on_ShareButton_clicked()
{
    send_thread->start();
    timer->start(5000);
}

