#ifndef __BOND_H__
#define __BOND_H__

#include "biz.h"
#include "biz_config.h"
#include "biz_system_complex.h"

#include "gui_dev.h"

typedef struct
{
	int msg_type;//0 Õı³£½áÊø1 ÍøÂç´íÎó
	u32 dev_ip;
} SPlaybackNotify_t;


/**********************************************************/
#ifdef GUI_BOND_CLASS_DECLARE   //ä»… gui éƒ¨åˆ†éœ€è¦çœ‹åˆ°ç±»çš„å£°æ˜

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

int notifyGuiUpdateTime(SDateTime *pdt);

int notifyDevInfo(SGuiDev *pdev); //Éè±¸²ã½«ĞÅÏ¢Í¨Öª¸øÉÏ²ã

int notifyPlaybackInfo(SPlaybackNotify_t *para);

#ifdef __cplusplus
}
#endif

#endif
