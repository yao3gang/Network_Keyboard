#ifndef __BOND_H__
#define __BOND_H__

#include "types.h"
#include "biz_4_gui.h"
#include "biz.h"
#include "biz_net.h"
#include "biz_msg_type.h"
#include "biz_types.h"
#include "biz_config.h"
#include "biz_remote_stream.h"
#include "biz_device.h"
#include "biz_system_complex.h"

#include "gui_dev.h"

typedef struct
{
    EMBIZEVENT type;
    u32 playback_chn;
    s32 stream_errno; //GLB_ERROR_NUM

    //回放、文件下载进度
    struct
    {
        u32 cur_pos;
        u32 total_size;
    } stream_progress;

} SPlaybackNotify_t;


/**********************************************************/
#ifdef GUI_BOND_CLASS_DECLARE   //仅 gui 部分需要看到类的声明

#include <QObject>
#include <QMutex>

class Cbond: public QObject
{
    Q_OBJECT

public:
    static Cbond *Instance()
    {
        //static QMutex mutex;
        if (!_instance)
        {
            QMutexLocker locker(&mutex);
            if (!_instance)
            {
                _instance = new Cbond;
            }
        }
        return _instance;
    }

    ~Cbond() {}
    void guiEnableRcvNotify();
    //是否准备好接收通知
    VD_BOOL bondNotifyIsReady();

    void bondNotifyUpdateTime(SDateTime *pdt);
    void bondNotifyDevInfo(SGuiDev *pdev);
    void bondNotifyPlaybackInfo(SPlaybackNotify_t *para);

public:
    bool b_recv;//main.cpp set

private:
    explicit Cbond(QObject *parent = 0): b_recv(false){}

private:
    static Cbond *_instance;
    static QMutex mutex;

signals:
    void signalNotifyUpdateTime(SDateTime dt);
    void signalNotifyDevInfo(SGuiDev dev);
    void signalNotifyPlaybackInfo(SPlaybackNotify_t para);
};

#define gp_bond (Cbond::Instance())

#endif

/**********************************************************/

//extern function
#ifdef __cplusplus
extern "C" {
#endif

//是否准备好接收通知
VD_BOOL notifyIsReady();

int notifyGuiUpdateTime(SDateTime *pdt);

int notifyDevInfo(SGuiDev *pdev);

int notifyPlaybackInfo(SPlaybackNotify_t *para);

#ifdef __cplusplus
}
#endif

#endif
