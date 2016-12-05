#ifndef PAGE_CONFIG_H
#define PAGE_CONFIG_H

#include <QtGui>
#include "form_tvwall_config.h"

#include "biz_config.h"
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
    std::vector<SConfigTvWallParam> vtvwall_list;
    form_tvwall_config::OpenMode tvwall_open_mode;

private:
    Ui::page_config *ui;
    void init_form();
    void init_data();
    void refreshTableTvWall();

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
    void on_btn_tvWall_modify_clicked();
    void accept_tvwall_data(QByteArray data);

};

#endif // PAGE_CONFIG_H
