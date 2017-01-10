#ifndef CUSTOM_SLIDER_H
#define CUSTOM_SLIDER_H

#include <QtGui>

class custom_slider : public QSlider
{
    Q_OBJECT
public:
    explicit custom_slider(QWidget *parent = 0);
    ~custom_slider();
    
protected:
    void mousePressEvent(QMouseEvent *ev);//重写QSlider的mousePressEvent事件
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void costomSliderPressed(int pos);//自定义的鼠标单击信号，用于捕获并处理
    void costomSliderReleased(int pos);
};

#endif // CUSTOM_SLIDER_H
