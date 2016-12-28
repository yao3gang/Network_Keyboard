#include "bond.h"

QMutex Cbond::mutex;
Cbond *Cbond::_instance = NULL;

void Cbond::guiEnableRcvNotify()
{
    QMutexLocker locker(&mutex);
    b_recv = true;
}

//是否准备好接收通知
VD_BOOL Cbond::bondNotifyIsReady()
{
    QMutexLocker locker(&mutex);
    return b_recv;
}

void Cbond::bondNotifyUpdateTime(SDateTime *pdt)
{
    SDateTime dt = *pdt;

    QMutexLocker locker(&mutex);

    if (b_recv)
    {
        //DBG_PRINT("\n");
        emit signalNotifyUpdateTime(dt);
    }
}

void Cbond::bondNotifyDevInfo(SGuiDev *pdev)
{
    SGuiDev dev = *pdev;

    QMutexLocker locker(&mutex);

    if (b_recv)
    {
        //DBG_PRINT("\n");
        emit signalNotifyDevInfo(dev);
    }
}

void Cbond::bondNotifyPlaybackInfo(SPlaybackNotify_t *para)
{
    SPlaybackNotify_t data = *para;

    QMutexLocker locker(&mutex);

    if (b_recv)
    {
        //DBG_PRINT("\n");
        emit signalNotifyPlaybackInfo(data);
    }
}




//extern function
//是否准备好接收通知
VD_BOOL notifyIsReady()
{
    return gp_bond->bondNotifyIsReady();
}

int notifyGuiUpdateTime(SDateTime *pdt)
{
    gp_bond->bondNotifyUpdateTime(pdt);
    return 0;
}

int notifyDevInfo(SGuiDev *pdev)//设备层将信息通知给上层
{
    //DBG_PRINT("\n");
    gp_bond->bondNotifyDevInfo(pdev);
    return 0;
}

int notifyPlaybackInfo(SPlaybackNotify_t *para)
{
    gp_bond->bondNotifyPlaybackInfo(para);
    return 0;
}




