#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>

namespace Ui {
class StartWidget;
}

class Controltee;
class Controller;
class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(Controller * ,Controltee *,QWidget *parent = nullptr);
    ~StartWidget();

signals:
    void send_localaddr();

private slots:
    void on_LinkButton_clicked();

private:
    Ui::StartWidget *ui;
    Controller *controller;
    Controltee *controltee;
};

#endif // STARTWIDGET_H
