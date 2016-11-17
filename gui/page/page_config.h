#ifndef PAGE_CONFIG_H
#define PAGE_CONFIG_H

#include <QWidget>
#include <QRegExpValidator>

#include "biz_system_complex.h"

namespace Ui {
class page_config;
}

class page_config : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_config(QWidget *parent = 0);
    ~page_config();

private:
    QRegExpValidator *pvalidator_ipaddr;
    QRegExpValidator *pvalidator_mac;

private:
    Ui::page_config *ui;
    void init_form();

private slots:
    void button_clicked();
    //日期和时间
    void setDateFormat(int index);//设置日期格式
    void setTimeFormat(int index);//设置时间格式
    void on_btn_date_def_clicked();
    void on_btn_date_apply_clicked();
    void on_btn_sync_clicked();//时间同步
    void on_btn_net_def_clicked();
    void on_btn_net_apply_clicked();

    void on_btn_tvWall_add_clicked();
    void on_btn_tvWall_del_clicked();

};

#endif // PAGE_CONFIG_H
