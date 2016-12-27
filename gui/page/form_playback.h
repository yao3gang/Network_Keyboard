#ifndef FORM_PLAYBACK_H
#define FORM_PLAYBACK_H

#include <QtGui>

#include "gui_dev.h"
#include "types.h"
#include "ctrlprotocol.h"

namespace Ui {
class form_playback;
}

class form_playback : public QWidget
{
    Q_OBJECT
    
public:
    explicit form_playback(QWidget *parent = 0);
    ~form_playback();

protected:
    void mousePressEvent(QMouseEvent *);

private:
    void init_style();
    void init_form();//控件
    void setupTreeWidgetNvr();
    void setupWidgetLeft();
    void setupWidgetBottom();
    void setupTableWidgetResult();
    void refreshWidgetResult();
    void refreshWidgetResultLeft();//搜索文件前部分在此显示
    void refreshWidgetResultright();//搜索文件后部分在此显示
    int getCurrentNvrChn(u32 *pnvr_ip, u32 *pnvr_chn);//得到当前选中的NVR IP和通道号

private:
    Ui::form_playback *ui;
    QMutex mutex;
    ifly_recsearch_param_t search_para;
    ifly_search_file_result_t search_result;
    u32 search_nvr_ip;
    u32 search_nvr_chn;

private slots:
    void refreshDevInfo(SGuiDev dev);
	void slotNotifyPlaybackInfo(SPlaybackNotify_t playback_msg);
	
    void tableWidgetClicked(QTableWidgetItem * item);
    void tableWidgetDoubleClicked(QTableWidgetItem * item);
    void showTableWidget(bool b);
    void on_btn_srh_clicked();
    void on_btn_page_start_clicked();
    void on_btn_page_pre_clicked();
    void on_btn_page_next_clicked();
    void on_btn_page_end_clicked();
    void on_btn_to_dec_clicked();
    //play ctl
    void on_btn_stop_clicked();
};

#endif // FORM_PLAYBACK_H
