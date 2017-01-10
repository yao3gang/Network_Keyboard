#include "custom_slider.h"

custom_slider::custom_slider(QWidget *parent) :
    QSlider(parent)
{
}

custom_slider::~custom_slider()
{

}


void custom_slider::mousePressEvent(QMouseEvent *ev)//重写QSlider的mousePressEvent事件
{
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    QSlider::mousePressEvent(ev);
    //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
    double pos = ev->pos().x() / (double)width();
    if (pos < 0) pos = 0;
    else if (pos > 1) pos = 1;

    int i_pos = pos * (maximum() - minimum()) + minimum();

    setValue(i_pos);
    //发送自定义的鼠标单击信号
    emit costomSliderPressed(i_pos);
}

void custom_slider::mouseReleaseEvent(QMouseEvent *ev)
{
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    QSlider::mousePressEvent(ev);
    //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
    double pos = ev->pos().x() / (double)width();
    if (pos < 0) pos = 0;
    else if (pos > 1) pos = 1;

    int i_pos = pos * (maximum() - minimum()) + minimum();

    setValue(i_pos);
    //发送自定义的鼠标单击信号
    emit costomSliderReleased(i_pos);
}

