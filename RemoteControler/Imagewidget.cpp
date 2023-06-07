#include "Imagewidget.h"
#include "ui_Imagewidget.h"
#include <QCursor>


ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
    imagelabel = new QLabel;
    layout = new QVBoxLayout;
    layout->addWidget(imagelabel);
    this->setLayout(layout);
    this->setMouseTracking(true);
    this->imagelabel->setMouseTracking(true);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e){
    emit move_send(e->pos());
}

void ImageWidget::mousePressEvent(QMouseEvent *e){
    emit press_send(e->pos());
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e){
    emit release_send(e->pos());
}
