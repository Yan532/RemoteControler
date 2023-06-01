#ifndef WIDGET_H
#define WIDGET_H

#include "imagewidget.h"
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QQueue>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    QImage getImage();
    ~Widget();

signals:
    void labelsign();

private slots:
    void start();//监听的事件
    void getimagedata(QString *);
    void displayError(QAbstractSocket::SocketError socketError);//错误处理
    void setLabel();
    void on_startButton_clicked();

private:
    Ui::Widget *ui;
    ImageWidget *imagewidget;

    QTcpServer Server;
    QTcpSocket *Socket = nullptr;

    QQueue<QString *> imagedatas;
};
#endif // WIDGET_H
