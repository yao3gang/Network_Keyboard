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
        emit signalNotifyUpdateTime(dt);
    }
}

void Cbond::bondNotifyDevInfo(SGuiDev *pdev)
{
	SGuiDev dev = *pdev;

	QMutexLocker locker(&mutex);
	
	if (b_recv)
    {
        emit signalNotifyDevInfo(dev);
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
	gp_bond->bondNotifyDevInfo(pdev);
    return 0;
}


