#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void sendip_port(QString,unsigned short);

private slots:
    void hasconnect();
    void notconnect();

    void on_LinkButton_clicked();

    void on_ShareButton_clicked();

private:
    Ui::Widget *ui;
    QThread *send_thread = nullptr;
    QTimer *timer;
};
#endif // WIDGET_H
