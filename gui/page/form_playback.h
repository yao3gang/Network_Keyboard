#ifndef FORM_PLAYBACK_H
#define FORM_PLAYBACK_H

#include <QtGui>

#include "types.h"
#include "ctrlprotocol.h"
#include "gui_dev.h"
#include "biz_playback.h"
#include "bond.h"

namespace Ui {
class form_playback;
}

class form_playback : public QWidget
{
    Q_OBJECT
    
    typedef enum{
        EM_PLAY_STATUS_STOP,
        EM_PLAY_STATUS_PLAYING,
        EM_PLAY_STATUS_PAUSE,
        EM_PLAY_STATUS_STEP,
    } EM_PLAY_STATUS_T;

public:
    explicit form_playback(QWidget *parent = 0);
    ~form_playback();

protected:
    void mousePressEvent(QMouseEvent *);
    void showEvent ( QShowEvent * event );
    void hideEvent ( QHideEvent * event );

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
    QLabel *m_displayLabel;
    QMutex mutex;
    ifly_recsearch_param_t search_para;
    ifly_search_file_result_t search_result;
    u32 search_nvr_ip;//搜索操作相关数据
    u32 search_nvr_chn;
    u32 play_nvr_ip;//当前播放相关数据
    u32 play_nvr_chn;
    ifly_recfileinfo_t play_file;
    EM_PLAY_STATUS_T play_status;
    bool b_slider_mover;
    int slider_pressed_pos;
    int play_pos;
    int play_rate_index;

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
    void on_btn_play_clicked();
    void on_btn_step_clicked();
    void set_play_rate(int index);

    void slider_pressed(int pos);
    void slider_moved(int pos);
    void slider_released(int pos);
};

#endif // FORM_PLAYBACK_H
