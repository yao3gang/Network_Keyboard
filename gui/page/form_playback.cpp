#include <QtGui>
#include "form_playback.h"
#include "ui_form_playback.h"

#include "page_manager.h"
#include "page_main.h"
#include "page_dev_mgt.h"

#include "biz_system_complex.h"

form_playback::form_playback(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_playback)
{
    ui->setupUi(this);
    init_style();
    init_form();//控件
}

//初始化无边框窗体
void form_playback::init_style()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void form_playback::init_form() //控件
{
    setupWidgetLeft();
    setupWidgetBottom();
}

void form_playback::setupWidgetLeft()
{
    //nvr treeWidget
    setupTreeWidgetNvr();

    //search date and time
    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setProperty("noinput", true);

    SDateTime dt;
    memset(&dt, 0, sizeof(SDateTime));

    int ret = BizGetTime(&dt);
    if (ret)
    {
        ERR_PRINT("BizGetTime failed, ret: %d\n", ret);
        return ;
    }

    QDate qd(dt.nYear, dt.nMonth, dt.nDay);
    ui->dateEdit->setDate(qd);

    QTime qt_start(0, 0, 0);// 00:00:00
    ui->timeEdit_start->setTime(qt_start);

    QTime qt_end(23, 59, 59);// 23:59:59
    ui->timeEdit_end->setTime(qt_end);
}

void form_playback::setupTreeWidgetNvr()
{
    QMutexLocker locker(&mutex);
    //dev list
    page_dev_mgt *page_dev = (page_dev_mgt *)getPage(PAGE_DEV_MGT);
    if (NULL == page_dev)
    {
        ERR_PRINT("getPage(PAGE_DEV_MGT) failed\n");
        return;
    }
    connect(page_dev, SIGNAL(signalDevInfoChange(SGuiDev)), this, SLOT(refreshDevInfo(SGuiDev)));//更新设备 alive 状态

    QStringList strlist_devtype;
    if (page_dev->getDevTypeStrList(strlist_devtype))
    {
        ERR_PRINT("getDevTypeStrList failed\n");
        return;
    }

    ui->treeWidget_nvr->setColumnCount(1);//1列
    ui->treeWidget_nvr->setHeaderLabels(QStringList() << strlist_devtype.at(EM_NVR-EM_NVR));//EM_NVR-EM_NVR

    //get dev info
    std::list<u32> dev_ip_list;
    std::list<u32>::iterator list_iter;
    SGuiDev dev;
    QTreeWidgetItem *dev_item = NULL;
    QTreeWidgetItem *chn_item = NULL;
    QList<QTreeWidgetItem *> list_dev;
    QList<QTreeWidgetItem *> list_chn;
    struct in_addr in;
    QString qstr_ip, qstr_chn;
    u32 dev_ip;
    int i;
    int ret = SUCCESS;
    char chn_name[32];

    ui->treeWidget_nvr->clear();//清空，重新载入数据

    if (page_dev->getDevIPList(EM_NVR, dev_ip_list))
    {
        ERR_PRINT("getDevIPList failed, dev type: %d\n", EM_NVR);
        return;
    }

    for (list_iter = dev_ip_list.begin();
         list_iter != dev_ip_list.end();
         ++list_iter)
    {
        dev_ip = *list_iter;

        in.s_addr = dev_ip;
        qstr_ip = QString(inet_ntoa(in));
        if (qstr_ip.isEmpty())
        {
            ERR_PRINT("dev ip invalid\n");
            continue;
        }

        if (page_dev->getDevInfo(EM_NVR, dev_ip, dev))
        {
            ERR_PRINT("getDevInfo failed, dev type: %d, ip: %s\n", EM_NVR, inet_ntoa(in));
            continue;
        }

        dev_item = NULL;
        dev_item = new QTreeWidgetItem;
        if (NULL == dev_item)
        {
            ERR_PRINT("dev new QTreeWidgetItem failed\n");
            return;
        }

        //DBG_PRINT("dev type: %d, ip: %s\n", dev_type, inet_ntoa(in));
        dev_item->setText(0, qstr_ip);

        if (dev.b_alive)
        {
            dev_item->setIcon(0, QIcon(":/image/dev_online.png"));
        }
        else
        {
            dev_item->setIcon(0, QIcon(":/image/dev_offline.png"));
        }

        list_dev.append(dev_item);

    #if 1
        list_chn.clear();

        for (i = 0; i < dev.maxChnNum; ++i)
        {
            chn_item = new QTreeWidgetItem;
            if (NULL == chn_item)
            {
                ERR_PRINT("chn new QTreeWidgetItem failed\n");
                return;
            }

            qstr_chn = QString(QString::fromUtf8("通道%1").arg(i+1));

            ret = BizGetDevChnName(EM_NVR, dev_ip, i, chn_name, sizeof(chn_name));
            if (ret)
            {
                ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
            }
            else
            {
                //NVR:192.168.1.248:通道1:大门
                if (chn_name[0]) //不为空
                {
                    qstr_chn += QString(QString::fromUtf8(":%1").arg(QString::fromUtf8(chn_name)));
                }
            }

            chn_item->setText(0, qstr_chn);
            chn_item->setIcon(0, QIcon(":/image/chn.png"));

            list_chn.append(chn_item);
        }
        if (list_chn.isEmpty())
        {
            DBG_PRINT("dev: %s, list_chn.isEmpty()\n", qstr_ip.toUtf8().constData());
        }
        else
        {
            dev_item->addChildren(list_chn);
        }

    #endif
    }

    ui->treeWidget_nvr->addTopLevelItems(list_dev);
}

void form_playback::setupWidgetBottom()
{
    //播放控制
    //btn play/pause
    ui->btn_play->setIcon(QIcon(QString::fromUtf8(":/image/pctl_play.bmp")));
    ui->btn_play->setToolTip(QString::fromUtf8("播放/暂停"));

    //btn stop
    ui->btn_stop->setToolTip(QString::fromUtf8("停止"));

    //btn frame
    ui->btn_next_frame->setToolTip(QString::fromUtf8("单帧前进"));

    //搜索文件 控制
    //btn extra
    ui->btn_extra->setIcon(QIcon(QString::fromUtf8(":/image/up.bmp")));
    ui->btn_extra->setToolTip(QString::fromUtf8("显示文件列表"));

    ui->btn_extra->setCheckable(true);
    ui->btn_extra->setAutoDefault(false);

    ui->tableWidget->hide();

    connect(ui->btn_extra, SIGNAL(toggled(bool)), this, SLOT(showTableWidget(bool)));
}

void form_playback::showTableWidget(bool b)
{
    ui->tableWidget->setVisible(b);

    QIcon icon;
    QString icon_name;
    QString tooltip;
    if (b)
    {
        icon_name = QString::fromUtf8(":/image/down.bmp");
        tooltip = QString::fromUtf8("隐藏文件列表");
    }
    else
    {
        icon_name = QString::fromUtf8(":/image/up.bmp");
        tooltip = QString::fromUtf8("显示文件列表");
    }
    icon.addFile(icon_name, QSize(), QIcon::Normal, QIcon::Off);
    ui->btn_extra->setIcon(icon);
    ui->btn_extra->setToolTip(tooltip);
}

form_playback::~form_playback()
{
    delete ui;
}

void form_playback::refreshDevInfo(SGuiDev dev)
{
    QMutexLocker locker(&mutex);
    struct in_addr in;
    QString qstr_ip;
    QList<QTreeWidgetItem *> list_item;

    in.s_addr = dev.deviceIP;
    qstr_ip = QString(inet_ntoa(in));
    if (qstr_ip.isEmpty())
    {
        ERR_PRINT("dev ip invalid\n");
        return ;
    }
    DBG_PRINT("dev type: %d, ip: %s\n", dev.devicetype, qstr_ip.toUtf8().constData());

    if (dev.devicetype == EM_NVR)
    {
        list_item = ui->treeWidget_nvr->findItems(qstr_ip, Qt::MatchContains);

        if (list_item.size() != 1)//查找结果必须 == 1
        {
            DBG_PRINT("findItems size(%d) != 1, dev ip: %s\n", list_item.size(), qstr_ip.toUtf8().constData());

            return ;
        }
        QTreeWidgetItem *item = list_item.at(0);

        ui->treeWidget_nvr->deleteItemChildren(item);//销毁所有通道条目

        if (dev.b_alive)
        {
            item->setIcon(0, QIcon(":/image/dev_online.png"));

            //设备在线 添加所有通道条目
            QList<QTreeWidgetItem *> list_chn;
            QTreeWidgetItem * chn_item;
            QString qstr_chn;
            int i;
            for (i = 0; i < dev.maxChnNum; ++i)
            {
                chn_item = new QTreeWidgetItem;
                if (NULL == chn_item)
                {
                    ERR_PRINT("chn new QTreeWidgetItem failed\n");
                    return;
                }

                qstr_chn = QString("chn%1").arg(i+1);
                chn_item->setText(0, qstr_chn);
                chn_item->setIcon(0, QIcon(":/image/chn.png"));

                list_chn.append(chn_item);
            }
            if (list_chn.isEmpty())
            {
                ERR_PRINT("dev: %s, list_chn.isEmpty()\n", qstr_ip.toUtf8().constData());
            }
            else
            {
                item->addChildren(list_chn);
            }
        }
        else
        {
            item->setIcon(0, QIcon(":/image/dev_offline.png"));
        }
    }
}

void form_playback::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        this->close();

        page_main* pmain = (page_main*)getPage(PAGE_MAIN);
        if (NULL == pmain)
        {
            ERR_PRINT("getPage(PAGE_MAIN) failed\n");
            return;
        }
        pmain->show();
    }
}


