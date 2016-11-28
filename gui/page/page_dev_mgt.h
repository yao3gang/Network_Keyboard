#ifndef PAGE_DEV_MGT_H
#define PAGE_DEV_MGT_H

#include <list>

#include <QWidget>
#include <QMutex>
#include <QStringList>

#include "biz_config.h"
#include "biz_device.h"
#include "gui_dev.h"
#include "bond.h"

namespace Ui {
class page_dev_mgt;
}

class page_dev_mgt : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_dev_mgt(QWidget *parent = 0);
    ~page_dev_mgt();

    int getDevList(EM_DEV_TYPE dev_type, std::list<s32> &dev_ip_list);//同级其他模块调用
    int getDevInfo(EM_DEV_TYPE dev_type, u32 ip, SGuiDev *p);//同级其他模块调用
    void syncAllDevInfo();//从下层 BIZ_DEV 同步设备信息

protected:
    void showEvent( QShowEvent * event );

private slots:
    void on_btn_srh_clicked();
    void on_btn_add_clicked();
    void on_btn_info_clicked();
    void on_btn_del_clicked();
    void cmbSrhChange(int index);
    void cmbAddChange(int index);
    void tableWidgetSrhDBClicked(int row, int column);
    void tableWidgetDevDBClicked(int row, int column);
    void updateDevInfo(SGuiDev dev);

private:
    void init_form();//控件
    void init_data();//设备信息
    void addRowTableSrh(int row, const SBiz_DeviceInfo_t *pdev); //搜索表 添加一行
    void addRowTableDev(int row, const SGuiDev *pdev);
    void modifyRowTableDev(int row, const SGuiDev *pdev);
    int addDev(SGuiDev *pdev);

private:
    Ui::page_dev_mgt *ui;
    bool b_inited;
    QMutex mutex;
    QStringList strlist_devtype;
    MAP_IP_DEV map_dev[EM_DEV_TYPE_MAX - EM_NVR];
    //MAP_IP_DEV map_nvr;
    //MAP_IP_DEV map_patrol_dec;
    //MAP_IP_DEV map_switch_dec;
};

#endif // PAGE_DEV_MGT_H
