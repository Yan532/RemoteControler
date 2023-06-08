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
    setMouseTracking(true);
    this->imagelabel->setMouseTracking(true);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e){
    //static QPoint lastpos;
    //QPoint currentPos = e->pos();
    //int distanceThreshold = 50;
    //int distance = (currentPos - lastpos).manhattanLength();
    //if(distance >= distanceThreshold){
        //lastpos = currentPos;
        emit move_send(e->pos());
    //}
}

void ImageWidget::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        emit leftPress_send(e->pos());
    }
    if(e->button() == Qt::RightButton){
        emit rightPress_send(e->pos());
    }
    ImageWidget::mouseMoveEvent(e);
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        emit leftRelease_send(e->pos());
    }
    if(e->button() == Qt::RightButton){
        emit rightRelease_send(e->pos());
    }
    ImageWidget::mouseMoveEvent(e);
}

void ImageWidget::keyPressEvent(QKeyEvent *e){
    emit keypress_send(e->key());
}

void ImageWidget::keyReleaseEvent(QKeyEvent *e){
    emit keyrelease_send(e->key());
}
