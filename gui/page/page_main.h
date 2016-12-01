#ifndef PAGE_MAIN_H
#define PAGE_MAIN_H

#include <QWidget>
#include "types.h"
#include "biz_system_complex.h"

namespace Ui {
class page_main;
}

class page_main : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_main(QWidget *parent = 0);
    ~page_main();
    void setTimeFormat(u8 date_format, u8 time_format);

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
};

#endif // PAGE_MAIN_H
