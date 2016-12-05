#include <QtGui>
#include <QDialog>
#include <QtDebug>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "page_main.h"
#include "page_manager.h"
#include "frmmessagebox.h"
#include "page_config.h"
#include "ui_page_config.h"
#include "wzd_tvwall_layout.h"
#include "form_tvwall_config.h"
#include "frmmessagebox.h"

#include "bond.h"
#include "biz.h"
#include "biz_config.h"
#include "biz_system_complex.h"

page_config::page_config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_config)
{
    ui->setupUi(this);
    init_form();
    init_data();
    ui->btn_date->click();
}

page_config::~page_config()
{
    delete pvalidator_mac;
    delete pvalidator_ipaddr;
    delete ui;
}

void page_config::init_form() //init stackwidget
{
    QList<QPushButton *> btns = ui->widget_left->findChildren<QPushButton *>();
    foreach (QPushButton * b, btns)
    {
        connect(b, SIGNAL(clicked()), this, SLOT(button_clicked()));
    }

    //time & date config
    ui->comboBox_date->addItem(QString::fromUtf8("年-月-日"));
    ui->comboBox_date->addItem(QString::fromUtf8("月-日-年"));
    ui->comboBox_date->addItem(QString::fromUtf8("日-月-年"));
    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setProperty("noinput", true);
    ui->timeEdit->setProperty("noinput", true);

    ui->comboBox_time->addItem(QString::fromUtf8("24小时"));
    ui->comboBox_time->addItem(QString::fromUtf8("12小时"));

    connect(ui->comboBox_date, SIGNAL(currentIndexChanged(int)), this, SLOT(setDateFormat(int)));
    connect(ui->comboBox_time, SIGNAL(currentIndexChanged(int)), this, SLOT(setTimeFormat(int)));

    //QLineEdit IP地址 格式设置
    ui->label_mac_warning->setText(QString::fromUtf8("务必谨慎修改"));
    QRegExp rx_mac("([A-Fa-f0-9]{2}\\:){5}[A-Fa-f0-9]{2}");
    pvalidator_mac = new QRegExpValidator(rx_mac, this);
    ui->lineEdit_ipaddr->setValidator(pvalidator_mac);

    QRegExp rx_ipaddr("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){4}");
    pvalidator_ipaddr = new QRegExpValidator(rx_ipaddr, this);
    ui->lineEdit_ipaddr->setValidator(pvalidator_ipaddr);
    ui->lineEdit_submask->setValidator(pvalidator_ipaddr);
    ui->lineEdit_gateway->setValidator(pvalidator_ipaddr);
    ui->lineEdit_dns1->setValidator(pvalidator_ipaddr);
    ui->lineEdit_dns2->setValidator(pvalidator_ipaddr);

    //tv wall
    ui->tableWidget_tvwall->setColumnCount(2);
    ui->tableWidget_tvwall->verticalHeader()->setVisible(false);//列表头不可见
    ui->tableWidget_tvwall->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    ui->tableWidget_tvwall->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    //ui->tableWidget_tvWall->setSelectionMode(QAbstractItemView::MultiSelection);//可以多选行
    ui->tableWidget_tvwall->setAlternatingRowColors(true);//奇偶行不同颜色显示
    ui->tableWidget_tvwall->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    ui->tableWidget_tvwall->setColumnWidth(0,150);

    //设置表头
    QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
    ui->tableWidget_tvwall->setHorizontalHeaderItem(0, __qtablewidgetitem);
    QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
    ui->tableWidget_tvwall->setHorizontalHeaderItem(1, __qtablewidgetitem1);

    QStringList header;
    header.append(QString::fromUtf8("条目"));
    header.append(QString::fromUtf8("描述"));
    ui->tableWidget_tvwall->setHorizontalHeaderLabels(header);
}

void page_config::init_data()
{
    if (BizConfigGetTvWallList(vtvwall_list))
    {
        ERR_PRINT("BizConfigGetTvWallList failed\n");
        return ;
    }

    if (vtvwall_list.empty())
    {
        ERR_PRINT("BizConfigGetTvWallList empty\n");
        return ;
    }

    if (vtvwall_list.size() > 1)
    {
        std::vector<SConfigTvWallParam> vtmp = vtvwall_list;

        //只保留第一个电视墙配置
        vtmp.erase(vtmp.begin());
        if (BizConfigDelTvWallParamList(vtmp))
        {
            ERR_PRINT("BizConfigDelTvWallParamList failed\n");
            return ;
        }

        vtvwall_list.erase(vtvwall_list.begin()+1, vtvwall_list.end());
    }

    refreshTableTvWall();
}

void page_config::refreshTableTvWall()
{
    ui->tableWidget_tvwall->clearContents();
    ui->tableWidget_tvwall->setRowCount(0);
    QTableWidgetItem *pitem = NULL;

    std::vector<SConfigTvWallParam>::iterator iter = vtvwall_list.begin();
    if (iter != vtvwall_list.end())
    {
        u32 cnt = iter->ndevs_per_line * iter->ndevs_per_col;
        if (cnt != iter->devlist.size())
        {
            ERR_PRINT("rows: %d, cols: %d, devlist size: %d\n",
                      iter->ndevs_per_line,
                      iter->ndevs_per_col,
                      iter->devlist.size());
            return ;
        }

        ui->tableWidget_tvwall->setRowCount(cnt + 2);

        pitem = new QTableWidgetItem;
        pitem->setText(QString::fromUtf8("行数"));
        ui->tableWidget_tvwall->setItem(0, 0, pitem);

        pitem = new QTableWidgetItem;
        pitem->setText(QString("%1").arg(iter->ndevs_per_line));
        ui->tableWidget_tvwall->setItem(0, 1, pitem);

        pitem = new QTableWidgetItem;
        pitem->setText(QString::fromUtf8("列数"));
        ui->tableWidget_tvwall->setItem(1, 0, pitem);

        pitem = new QTableWidgetItem;
        pitem->setText(QString("%1").arg(iter->ndevs_per_col));
        ui->tableWidget_tvwall->setItem(1, 1, pitem);

        QString tmp = QString::fromUtf8("屏幕");
        QString screen;
        QString ip_str;
        struct in_addr in;
        u32 dev_ip;
        u32 i=0;
        for (i=0; i<cnt; ++i)
        {
            screen = tmp + QString("%1").arg(i+1);
            pitem = new QTableWidgetItem;
            pitem->setText(screen);
            ui->tableWidget_tvwall->setItem(i+2, 0, pitem);

            dev_ip = iter->devlist.at(i);
            if (dev_ip == 0)
            {
                ip_str = QString("");
            }
            else
            {
                in.s_addr = dev_ip;
                if (NULL == inet_ntoa(in))
                {
                    ERR_PRINT("device ip invalid, index: %d\n", i);
                    ip_str = QString("");
                }
                else
                {
                    ip_str = QString::fromUtf8(inet_ntoa(in));
                }
            }

            pitem = new QTableWidgetItem;
            pitem->setText(ip_str);
            ui->tableWidget_tvwall->setItem(i+2, 1, pitem);
        }
    }
}

void page_config::button_clicked()
{
    int ret = 0;
    VD_BOOL b_ret = FALSE;
    QPushButton *btn = (QPushButton *)sender();
    QList<QPushButton *> btns = ui->widget_left->findChildren<QPushButton *>();
    foreach (QPushButton * b, btns)
    {
        b->setChecked(false);
    }
    btn->setChecked(true);

    QString name = btn->objectName();    

    if (ui->btn_date->objectName() == name)
    {
#if 1
        SConfigTimeParam stime_param;
        b_ret = BizConfigGetTimeParam(stime_param);
        if (b_ret)
        {
            qDebug("BizConfigGetTimeParam failed\n");
            //return 1;
        }
        else
        {
            ui->comboBox_date->setCurrentIndex(stime_param.ndate_format);
            setDateFormat(stime_param.ndate_format);

            ui->comboBox_time->setCurrentIndex(stime_param.ntime_format);
            setTimeFormat(stime_param.ntime_format);

            ui->checkBox_auto_sync->setChecked(stime_param.btime_sync);

            ui->lineEdit_server->setText(QString::fromUtf8(stime_param.ntp_svr_ip.c_str()));

            SDateTime dt;
            ret = BizGetTime(&dt);
            if (ret == 0)
            {
                QDate qd(dt.nYear, dt.nMonth, dt.nDay);
                ui->dateEdit->setDate(qd);

                QTime qt(dt.nHour, dt.nMinute, dt.nSecode);
                ui->timeEdit->setTime(qt);
            }
        }
#endif
        ui->stackedWidget_config->setCurrentIndex(0);
    }
    else if (ui->btn_network->objectName() == name)
    {
#if 1
        SConfigNetParam snet_param;
        struct in_addr in;

        b_ret = BizConfigGetNetParam(snet_param);
        if (b_ret)
        {
            qDebug("BizGetNetParam failed\n");
        }
        else
        {
            in.s_addr = snet_param.nhost_ip;
            ui->lineEdit_ipaddr->setText(inet_ntoa(in));

            in.s_addr = snet_param.nsubmask;
            ui->lineEdit_submask->setText(inet_ntoa(in));

            in.s_addr = snet_param.ngateway;
            ui->lineEdit_gateway->setText(inet_ntoa(in));

            in.s_addr = snet_param.ndns1;
            ui->lineEdit_dns1->setText(inet_ntoa(in));

            in.s_addr = snet_param.ndns2;
            ui->lineEdit_dns2->setText(inet_ntoa(in));

            ui->lineEdit_mac->setText(snet_param.mac.c_str());
        }
#endif
        ui->stackedWidget_config->setCurrentIndex(1);
    }
    else if (ui->btn_tv_wall->objectName() == name)
    {
        ui->stackedWidget_config->setCurrentIndex(2);
    }
    else if (ui->btn_upgrade->objectName() == name)
    {
        ui->stackedWidget_config->setCurrentIndex(3);
    }
    else if (ui->btn_sysinfo->objectName() == name)
    {
        ui->stackedWidget_config->setCurrentIndex(4);
    }
}

void page_config::on_btn_tvWall_add_clicked()
{
    if (!vtvwall_list.empty())
    {
        ShowMessageBoxInfo(QString::fromUtf8("当前已有电视墙配置"));
        return ;
    }

    tvwall_open_mode = form_tvwall_config::OpenModeNew;
    form_tvwall_config *page_tvwall_config = new form_tvwall_config(tvwall_open_mode);

    connect(page_tvwall_config, SIGNAL(signal_tvwall_data(QByteArray)),
            this, SLOT(accept_tvwall_data(QByteArray)));

    page_tvwall_config->show();
}

void page_config::accept_tvwall_data(QByteArray data)
{
    int rows = 0;
    int cols = 0;
    u32 dev_ip = 0;
    std::vector<u32> devlist;
    struct in_addr in;
    QDataStream::Status ret = QDataStream::Ok;
    QDataStream dataStream(&data, QIODevice::ReadOnly);

    DBG_PRINT("data size: %d\n", data.size());

    dataStream >> rows >> cols;

    ret = dataStream.status();
    if (ret)
    {
        DBG_PRINT("dataStream status: %d\n", ret);
        return ;
    }
    DBG_PRINT("rows: %d, cols: %d\n", rows, cols);

    int i = 0;
    int cnt = rows * cols;
    for (i=0; i<cnt; ++i)
    {
        dev_ip = 0;
        dataStream >> dev_ip;
        ret = dataStream.status();
        if (ret)
        {
            DBG_PRINT("dataStream status: %d, dev index: %d\n", ret, i);
            break;
        }

        in.s_addr = dev_ip;
        DBG_PRINT("dev index: %d, ip: %s\n", dev_ip, inet_ntoa(in));

        devlist.push_back(dev_ip);
    }

    if (i != cnt)//数目不对
    {
        return;
    }

    SConfigTvWallParam param;
    param.ndevs_per_line = rows;
    param.ndevs_per_col = cols;
    param.devlist = devlist;

    if (!vtvwall_list.empty())
    {
        vtvwall_list.clear();
    }
    vtvwall_list.push_back(param);

    refreshTableTvWall();

    if (tvwall_open_mode == form_tvwall_config::OpenModeNew)
    {
        if (BizConfigAddTvWallParam(param))
        {
            ERR_PRINT("BizConfigAddTvWallParam failed\n");
        }
    }
    else if(tvwall_open_mode == form_tvwall_config::OpenModeModfiy)
    {
        if (BizConfigModifyTvWallParam(0, param))
        {
            ERR_PRINT("BizConfigModifyTvWallParam failed\n");
        }
    }
}

void page_config::on_btn_tvWall_del_clicked()
{
    if (vtvwall_list.empty())
    {
        ShowMessageBoxError(QString::fromUtf8("当前还没有电视墙配置"));
        return ;
    }

    ui->tableWidget_tvwall->clearContents();
    ui->tableWidget_tvwall->setRowCount(0);

    SConfigTvWallParam param = *vtvwall_list.begin();
    if (BizConfigDelTvWallParam(param))
    {
        ERR_PRINT("BizConfigDelTvWallParam failed\n");
    }
    vtvwall_list.clear();
}

void page_config::on_btn_tvWall_modify_clicked()
{
    if (vtvwall_list.empty())
    {
        ShowMessageBoxError(QString::fromUtf8("当前还没有电视墙配置"));
        return ;
    }

    SConfigTvWallParam param = *vtvwall_list.begin();
    tvwall_open_mode = form_tvwall_config::OpenModeModfiy;
    form_tvwall_config *page_tvwall_config = new form_tvwall_config(tvwall_open_mode, param.ndevs_per_line, param.ndevs_per_col, &param.devlist);

    connect(page_tvwall_config, SIGNAL(signal_tvwall_data(QByteArray)),
            this, SLOT(accept_tvwall_data(QByteArray)));

    page_tvwall_config->show();
}

void page_config::on_btn_date_def_clicked()
{
#if 0 //for test
    qDebug() << "on_btn_def";
    qDebug() << ui->dateEdit->date().toString(QString::fromUtf8("yyyy-MM-dd"));
    qDebug() << ui->timeEdit->time().toString(QString::fromUtf8("hh:mm:ss AP"));
#else
    SConfigTimeParam stime_param;

    if (BizConfigGetDefaultTimeParam(stime_param))
    {
        ShowMessageBoxError(QString::fromUtf8("参数获取失败！"));
        return;
    }

    ui->comboBox_date->setCurrentIndex(stime_param.ndate_format);
    setDateFormat(stime_param.ndate_format);

    ui->comboBox_time->setCurrentIndex(stime_param.ntime_format);
    setTimeFormat(stime_param.ntime_format);

    ui->checkBox_auto_sync->setChecked(stime_param.btime_sync);

    ui->lineEdit_server->setText(QString::fromUtf8(stime_param.ntp_svr_ip.c_str()));
#endif
}

void page_config::on_btn_date_apply_clicked()
{
    SConfigTimeParam stime_param;
    if (BizConfigGetTimeParam(stime_param))
    {
         ShowMessageBoxError(QString::fromUtf8("参数获取失败！"));
         return;
    }

    stime_param.ndate_format = ui->comboBox_date->currentIndex();
    stime_param.ntime_format = ui->comboBox_time->currentIndex();
    stime_param.btime_sync = ui->checkBox_auto_sync->isChecked() ? 1:0;
    stime_param.ntp_svr_ip = ui->lineEdit_server->text().toUtf8().constData();

    if (BizConfigSetTimeParam(stime_param))
    {
        ShowMessageBoxError(QString::fromUtf8("参数设置失败！"));
        return;
    }
/*
    qDebug() << ui->comboBox_date->currentIndex();
    qDebug() << ui->comboBox_time->currentIndex();
    qDebug() << ui->checkBox_auto_sync->isChecked();
    qDebug() << ui->lineEdit_server->text().toUtf8().constData();
*/
    SDateTime dt;
    QDate qd = ui->dateEdit->date();
    QTime qt = ui->timeEdit->time();

    dt.nYear = qd.year();
    dt.nMonth = qd.month();
    dt.nDay = qd.day();
    dt.nHour = qt.hour();
    dt.nMinute = qt.minute();
    dt.nSecode = qt.second();

    if (BizSetTime(&dt))
    {
        ShowMessageBoxError(QString::fromUtf8("系统时间设置失败！"));
        return;
    }

    //notify page_main
    page_main* pmain = (page_main*)getPage(PAGE_MAIN);
    if (NULL == pmain)
    {
        ERR_PRINT("getPage(PAGE_MAIN) failed\n");
        return;
    }
    pmain->setTimeFormat(stime_param.ndate_format, stime_param.ntime_format);
}

void page_config::on_btn_net_def_clicked()
{
    SConfigNetParam snet_param;
    struct in_addr in;

    if (BizConfigGetDefaultNetParam(snet_param))
    {
        ShowMessageBoxError(QString::fromUtf8("参数获取失败！"));
        return;
    }

    ui->lineEdit_mac->setText(snet_param.mac.c_str());

    in.s_addr = snet_param.nhost_ip;
    ui->lineEdit_ipaddr->setText(inet_ntoa(in));

    in.s_addr = snet_param.nsubmask;
    ui->lineEdit_submask->setText(inet_ntoa(in));

    in.s_addr = snet_param.ngateway;
    ui->lineEdit_gateway->setText(inet_ntoa(in));

    in.s_addr = snet_param.ndns1;
    ui->lineEdit_dns1->setText(inet_ntoa(in));

    in.s_addr = snet_param.ndns2;
    ui->lineEdit_dns2->setText(inet_ntoa(in));
}

void page_config::on_btn_net_apply_clicked()
{
    SConfigNetParam snet_param;
    struct in_addr in;

    if (BizConfigGetNetParam(snet_param))
    {
        ShowMessageBoxError(QString::fromUtf8("参数获取失败！"));
        return;
    }

    snet_param.mac = ui->lineEdit_mac->text().toUtf8().constData();

    if (0 == inet_aton(ui->lineEdit_ipaddr->text().toUtf8().constData(), &in))
    {
        ShowMessageBoxError(QString::fromUtf8("IP地址输入有误！"));
        return;
    }
    snet_param.nhost_ip = in.s_addr;

    if (0 == inet_aton(ui->lineEdit_submask->text().toUtf8().constData(), &in))
    {
        ShowMessageBoxError(QString::fromUtf8("子网掩码输入有误！"));
        return;
    }
    snet_param.nsubmask = in.s_addr;

    if (0 == inet_aton(ui->lineEdit_gateway->text().toUtf8().constData(), &in))
    {
        ShowMessageBoxError(QString::fromUtf8("默认网关输入有误！"));
        return;
    }
    snet_param.ngateway = in.s_addr;

    if (0 == inet_aton(ui->lineEdit_dns1->text().toUtf8().constData(), &in))
    {
        ShowMessageBoxError(QString::fromUtf8("首选DNS输入有误！"));
        return;
    }
    snet_param.ndns1 = in.s_addr;

    if (0 == inet_aton(ui->lineEdit_dns2->text().toUtf8().constData(), &in))
    {
        ShowMessageBoxError(QString::fromUtf8("备选DNS输入有误！"));
        return;
    }
    snet_param.ndns2 = in.s_addr;

    if (BizSetNetParam(snet_param))
    {
        ShowMessageBoxError(QString::fromUtf8("参数设置失败！"));
        return;
    }

    ShowMessageBoxInfo(QString::fromUtf8("参数设置成功！"));
}

void page_config::on_btn_sync_clicked()
{
    if (BizNtpSyncTime(ui->lineEdit_server->text().toUtf8().constData()))
    {
        ShowMessageBoxInfo(QString::fromUtf8("时间同步失败！"));
        return;
    }

    ShowMessageBoxInfo(QString::fromUtf8("时间同步成功！"));
}

void page_config::setDateFormat(int index)//设置日期格式
{
    switch (index)
    {
        case 0: //年-月-日
            ui->dateEdit->setDisplayFormat(QString::fromUtf8("yyyy-MM-dd"));
        break;
        case 1: //月-日-年
            ui->dateEdit->setDisplayFormat(QString::fromUtf8("MM-dd-yyyy"));
        break;
        case 2: //日-月-年
            ui->dateEdit->setDisplayFormat(QString::fromUtf8("dd-MM-yyyy"));
        break;
        default:    //年-月-日
            ui->dateEdit->setDisplayFormat(QString::fromUtf8("yyyy-MM-dd"));
    }
}

void page_config::setTimeFormat(int index)//设置时间格式
{
    if (index == 0)//24小时制
    {
        ui->timeEdit->setDisplayFormat(QString::fromUtf8("hh:mm:ss"));
    }
    else if (index == 1)//12小时制
    {
        ui->timeEdit->setDisplayFormat(QString::fromUtf8("AP hh:mm:ss"));
    }
    else    //默认24小时制
    {
        ui->timeEdit->setDisplayFormat(QString::fromUtf8("hh:mm:ss"));
    }
}
#if 0
void page_config::update_time(SDateTime dt)
{
    QDate qd(dt.nYear, dt.nMonth, dt.nDay);
    ui->dateEdit->setDate(qd);

    QTime qt(dt.nHour, dt.nMinute, dt.nSecode);
    ui->timeEdit->setTime(qt);
}
#endif
