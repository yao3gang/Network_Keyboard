#ifndef PAGE_MAIN_H
#define PAGE_MAIN_H

#include <QDialog>
#include "types.h"
#include "biz_system_complex.h"

namespace Ui {
class page_main;
}

class page_main : public QDialog
{
    Q_OBJECT
    
public:
    explicit page_main(QWidget *parent = 0);
    ~page_main();
    void setTimeFormat(u8 date_format, u8 time_format);

protected:
#if 0
    //鼠标拖动
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    //鼠标拖动 end
#endif

private slots:
    void button_clicked();
    void update_time(SDateTime dt);

private:
    Ui::page_main *ui;
    u8 ndate_format;	//日期格式 0//年-月-日 1//月-日-年 2日-月-年
    u8 ntime_format;	//时间格式 0//24小时制 1//12小时制

private:
    void init_style();
    void init_form();

#if 0
    //鼠标拖动
    QPoint mousePoint;  //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;  //鼠标是否按下
    //鼠标拖动 end
#endif
};

#endif // PAGE_MAIN_H
