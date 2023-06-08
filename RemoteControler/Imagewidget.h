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
            imagelabel->setScaledContents(true);
            imagelabel->setPixmap(pixmap);
        }
    }

signals:
    void move_send(const QPoint &);
    void leftPress_send(const QPoint &);
    void rightPress_send(const QPoint &);
    void leftRelease_send(const QPoint &);
    void rightRelease_send(const QPoint &);
    void keypress_send(int key);
    void keyrelease_send(int key);


protected:
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;


private:
    Ui::ImageWidget *ui;
    QVBoxLayout *layout;
    QLabel *imagelabel;
};

#endif // IMAGEWIDGET_H
