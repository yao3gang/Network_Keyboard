#include "bond.h"

QMutex Cbond::mutex;
Cbond *Cbond::_instance = NULL;

void Cbond::guiEnableRcvNotify()
{
	QMutexLocker locker(&mutex);
	b_recv = true;
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

void bondNotifyPlaybackInfo(SPlaybackNotify_t *para)
{
	SPlaybackNotify_t data = *pdev;

	QMutexLocker locker(&mutex);
	
	if (b_recv)
    {
        //DBG_PRINT("\n");
        emit signalNotifyPlaybackInfo(data);
    }
}




//extern function
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


