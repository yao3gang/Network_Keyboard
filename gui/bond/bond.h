#ifndef __BOND_H__
#define __BOND_H__

#include "biz.h"
#include "biz_config.h"
#include "biz_system_complex.h"

#include "gui_dev.h"

typedef struct
{
    int msg_type;//0 正常结束 1 接收出错
    u32 dev_ip;
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
