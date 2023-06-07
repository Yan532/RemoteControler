#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

namespace Ui {
class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    ~ImageWidget();
    void setlabel_pixmap(const QPixmap &pixmap){
        if(!pixmap.isNull()){
            imagelabel->setPixmap(pixmap);
        }
    }

signals:
    void move_send(const QPointF &);
    void press_send(const QPointF &);
    void release_send(const QPointF &);

protected:
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;


private:
    Ui::ImageWidget *ui;
    QVBoxLayout *layout;
    QLabel *imagelabel;
};

#endif // IMAGEWIDGET_H
