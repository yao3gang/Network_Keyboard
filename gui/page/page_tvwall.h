#ifndef PAGE_TVWALL_H
#define PAGE_TVWALL_H

#include <QtGui>

#include "page_dev_mgt.h"
#include "ctrlprotocol.h"

namespace Ui {
class page_tvWall;
}

class page_tvWall : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_tvWall(QWidget *parent = 0);
    ~page_tvWall();

protected:
    void showEvent(QShowEvent *);

private:
    void init_form();
    void setupTreeWidgetScreen();
    void refreshTreeWidgetScreen();
    void setupTreeWidgetNvr();
    void refreshTreeWidgetNvr();
    void setupTableWidget();
    void clearTableWidget();
    void refreshTableWidget(ifly_ipc_info_t * pipc_info, s32 ipc_nums);

private slots:
    void refreshDevInfo(SGuiDev dev);
    void screenItemDoubleClicked(QTreeWidgetItem* item, int col);
    void btn_unbind_clicked();

private:
    Ui::page_tvWall *ui;
    page_dev_mgt *page_dev;
    u32 screen_cur_dec;//当前正在配置通道的解码器
};

#endif // PAGE_TVWALL_H
