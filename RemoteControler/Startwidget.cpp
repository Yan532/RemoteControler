#include "Startwidget.h"
#include "localhost.h"
#include "ui_Startwidget.h"
#include "Controler.h"
#include "Controltee.h"

#include <QThread>
#include <QApplication>

StartWidget::StartWidget(Controller *ctr,Controltee *cte,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    ui->Local_iplabel->setText(Localhost::getLocalIpAddress());

    controller = ctr;
    controltee = cte;


}

StartWidget::~StartWidget()
{
    delete ui;
}


void StartWidget::on_LinkButton_clicked()
{
    QMetaObject::invokeMethod(controller,"requestNewConnection",Q_ARG(QString,ui->Linklineedit->text()));
}

