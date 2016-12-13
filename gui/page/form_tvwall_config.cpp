#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "form_tvwall_config.h"
#include "ui_form_tvwall_config.h"

#include "page_manager.h"
#include "page_dev_mgt.h"
#include "frmmessagebox.h"

#include "biz_device.h"
#include "biz_config.h"

form_tvwall_config::form_tvwall_config(OpenMode mode, int _rows, int _cols, std::vector<u32> *pvdev, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_tvwall_config),
    open_mode(mode),
    rows(_rows),
    cols(_cols),
    nTvwall_nums(0),
    nTvwall_devs_max(0),
    nTvwall_devs_per_line(0),
    nTvwall_devs_per_col(0),
    pvalidator(NULL)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);//窗口关闭时销毁

    if (NULL == pvdev)
    {
        if (rows != 0 || cols != 0)
        {
            ERR_PRINT("rows(%d) cols(%d), but vdev == NULL\n", rows, cols);
        }
    }
    else
    {
        vdev = *pvdev;
        if (rows * cols != vdev.size())
        {
            ERR_PRINT("rows(%d) * cols(%d) != vdev.size (%d)\n", rows, cols, vdev.size());
        }
    }

    init_form();

    //限制值
    SConfigDvrProperty sdev_property;
    int ret = BizConfigGetDvrProperty(sdev_property);
    if (ret)
    {
        ERR_PRINT("BizConfigGetDvrProperty failed\n");
        return;
    }

    nTvwall_nums = sdev_property.nTvwall_nums;
    nTvwall_devs_max = sdev_property.nTvwall_devs_max;
    nTvwall_devs_per_line = sdev_property.nTvwall_devs_per_line;
    nTvwall_devs_per_col = sdev_property.nTvwall_devs_per_col;
}

form_tvwall_config::~form_tvwall_config()
{
    if (pvalidator)
    {
        delete pvalidator;
    }

    delete ui;
}

void form_tvwall_config::rowsChanged(QString text)
{
    bool ok = true;

    if (text.isEmpty())
        return;

    u32 _rows = text.toInt(&ok);

    if (!ok)
    {
        ERR_PRINT("rows: text.toInt failed, text: %s\n", text.toUtf8().constData());

        ShowMessageBoxInfo(QString(QString::fromUtf8("格式错误，请输入数字")));
        ui->lineEdit_tvwall_rows->setText(QString::fromUtf8("0"));

        return ;
    }

    if (_rows == 0)
    {
        ERR_PRINT("_rows == 0\n");
        return ;
    }

    if (_rows >= nTvwall_devs_per_line)
    {
        ERR_PRINT("_rows(%d) >= nTvwall_devs_per_line(%d)\n", _rows, nTvwall_devs_per_line);

        ShowMessageBoxError(QString(QString::fromUtf8("列数超所限制值(%1)")).arg(nTvwall_devs_per_line));
        ui->lineEdit_tvwall_rows->setText(QString::fromUtf8("0"));

        return ;
    }

    u32 cnt = _rows * cols;
    if (cnt >= nTvwall_devs_max)
    {
        ERR_PRINT("cnt(%d) >= nTvwall_devs_max(%d)\n", cnt, nTvwall_devs_max);

        ShowMessageBoxError(QString(QString::fromUtf8("行数*列数超所限制值(%1)")).arg(nTvwall_devs_max));
        ui->lineEdit_tvwall_rows->setText(QString::fromUtf8("0"));

        return ;
    }

    rowsOrColsChanged(_rows, cols);
}

void form_tvwall_config::colsChanged(QString text)
{
    bool ok = true;

    if (text.isEmpty())
        return;

    u32 _cols = text.toInt(&ok);

    if (!ok)
    {
        ERR_PRINT("_cols: text.toInt failed, text: %s\n", text.toUtf8().constData());

        ShowMessageBoxInfo(QString(QString::fromUtf8("格式错误，请输入数字")));
        ui->lineEdit_tvwall_cols->setText(QString::fromUtf8("0"));

        return ;
    }

    if (_cols == 0)
    {
        ERR_PRINT("_cols == 0\n");
        return ;
    }

    if (_cols >= nTvwall_devs_per_col)
    {
        ERR_PRINT("_cols(%d) >= nTvwall_devs_per_col(%d)\n", _cols, nTvwall_devs_per_col);

        ShowMessageBoxError(QString(QString::fromUtf8("列数超所限制值(%1)")).arg(nTvwall_devs_per_col));
        ui->lineEdit_tvwall_cols->setText(QString::fromUtf8("0"));

        return ;
    }

    u32 cnt = _cols * rows;
    if (cnt >= nTvwall_devs_max)
    {
        ERR_PRINT("cnt(%d) >= nTvwall_devs_max(%d)\n", cnt, nTvwall_devs_max);

        ShowMessageBoxError(QString(QString::fromUtf8("行数*列数超所限制值(%1)")).arg(nTvwall_devs_max));
        ui->lineEdit_tvwall_cols->setText(QString::fromUtf8("0"));

        return ;
    }

    rowsOrColsChanged(rows, _cols);
}

void form_tvwall_config::rowsOrColsChanged(u32 _rows, u32 _cols)
{
    u32 _cnt = _rows * _cols;
    u32 cnt  = rows * cols;
    int i = 0;

    DBG_PRINT("_rows: %d, _cols: %d\n", _rows, _cols);
    DBG_PRINT("rows: %d, cols: %d\n", rows, cols);

    rows = _rows;
    cols = _cols;

    if (_cnt == cnt)
    {
        DBG_PRINT("_cnt == cnt, %d\n", _cnt);
        return ;
    }
    else if (_cnt > cnt)
    {
        DBG_PRINT("_cnt(%d) > cnt(%d)\n", _cnt, cnt);
        for (i=cnt; i<_cnt; ++i)
        {
            vdev.push_back(0);//添加一个无效IP
        }
    }
    else //_cnt < cnt
    {
        DBG_PRINT("_cnt(%d) < cnt(%d)\n", _cnt, cnt);
        //do nothing
    }

    refreshTable();
}

void form_tvwall_config::init_form()//控件
{
    QString title;
    Qt::WindowFlags flag = windowFlags();
    flag &= ~(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);//去掉最大化/最小化按键
    //flag |= Qt::WindowStaysOnTopHint;
    setWindowFlags(flag);

    //限制输入两位整数
    QRegExp rx("[0-9]{2}");
    pvalidator = new QRegExpValidator(rx, this);
    ui->lineEdit_tvwall_rows->setValidator(pvalidator);
    ui->lineEdit_tvwall_cols->setValidator(pvalidator);

    title = QString::fromUtf8("电视墙配置");
    if (open_mode == OpenModeNew)
    {
        title += QString::fromUtf8("--添加");
    }
    else
    {
        title += QString::fromUtf8("--修改");
    }
    setWindowTitle(title);

    ui->lineEdit_tvwall_rows->setText(QString("%1").arg(rows));
    connect(ui->lineEdit_tvwall_rows, SIGNAL(textEdited(QString )), this, SLOT(rowsChanged(QString )));
    ui->lineEdit_tvwall_cols->setText(QString("%1").arg(cols));
    connect(ui->lineEdit_tvwall_cols, SIGNAL(textEdited(QString )), this, SLOT(colsChanged(QString )));

    setupTableWidget();
    setupTreeWidget();
}

void form_tvwall_config::setupTreeWidget()
{
    std::list<u32> dev_ip_list;
    std::list<u32>::iterator list_iter;
    SGuiDev dev;
    page_dev_mgt * page_dev = NULL;
    QTreeWidgetItem *dev_item = NULL;
    QList<QTreeWidgetItem *> list_dev;
    QStringList strlist_devtype;
    struct in_addr in;
    QString qstr;
    u32 dev_ip;

    page_dev = (page_dev_mgt *)getPage(PAGE_DEV_MGT);
    if (NULL == page_dev)
    {
        ERR_PRINT("getPage(PAGE_DEV_MGT) failed\n");
        return;
    }

    if (page_dev->getDevTypeStrList(strlist_devtype))
    {
        ERR_PRINT("getDevTypeStrList failed\n");
        return;
    }

    ui->tree_swich_dec->setColumnCount(1);//1列
    ui->tree_swich_dec->setHeaderLabels(QStringList() << strlist_devtype.at(EM_SWITCH_DEC-EM_NVR));

    if (page_dev->getDevIPList(EM_SWITCH_DEC, dev_ip_list))
    {
        ERR_PRINT("getDevIPList failed, dev type: %d\n", EM_SWITCH_DEC);
        return;
    }

    for (list_iter = dev_ip_list.begin();
         list_iter != dev_ip_list.end();
         ++list_iter)
    {
        dev_ip = *list_iter;

        in.s_addr = dev_ip;
        qstr = QString(inet_ntoa(in));
        if (qstr.isEmpty())
        {
            ERR_PRINT("dev ip invalid\n");
            continue;
        }

        if (page_dev->getDevInfo(EM_SWITCH_DEC, dev_ip, dev))
        {
            ERR_PRINT("getDevInfo failed, dev type: %d, ip: %s\n", EM_SWITCH_DEC, inet_ntoa(in));
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
        dev_item->setText(0, qstr);

        if (dev.b_alive)
        {
            dev_item->setIcon(0, QIcon(":/image/dev_online.png"));
        }
        else
        {
            dev_item->setIcon(0, QIcon(":/image/dev_offline.png"));
        }

        list_dev.append(dev_item);
    #if 0
        if (level_nums == TWO_LEVELS)
        {
            list_chn.clear();
            for (i = 0; i < dev.maxChnNum; ++i)
            {
                chn_item = new QTreeWidgetItem;
                if (NULL == chn_item)
                {
                    ERR_PRINT("chn new QTreeWidgetItem failed\n");
                    return;
                }

                qstr = QString("chn%1").arg(i+1);
                chn_item->setText(0, qstr);
                chn_item->setIcon(0, QIcon(":/image/chn.png"));

                list_chn.append(chn_item);
            }
            if (list_chn.isEmpty())
            {
                DBG_PRINT("list_chn.isEmpty() \n");
            }
            else
            {
                dev_item->addChildren(list_chn);
            }
        }
    #endif
    }

    ui->tree_swich_dec->addTopLevelItems(list_dev);

    ui->tree_swich_dec->setDragEnabled(true);
    ui->tree_swich_dec->setAcceptDrops(true);

    connect(page_dev, SIGNAL(signalDevInfoChange(SGuiDev)), this, SLOT(refreshDevInfo(SGuiDev)));//更新设备 alive 状态
}

void form_tvwall_config::refreshDevInfo(SGuiDev dev)
{
    struct in_addr in;
    QString qstr_ip;
    QList<QTreeWidgetItem *> list_item;
    int cnt = 0;

    in.s_addr = dev.deviceIP;
    qstr_ip = QString(inet_ntoa(in));
    if (qstr_ip.isEmpty())
    {
        ERR_PRINT("dev ip invalid\n");
        return ;
    }

    if (dev.devicetype != EM_SWITCH_DEC)
    {
        DBG_PRINT("dev.devicetype(%d) != EM_SWITCH_DEC\n", dev.devicetype);
        return ;
    }

    list_item = ui->tree_swich_dec->findItems(qstr_ip, Qt::MatchContains);
    cnt = list_item.count();
    if (cnt == 0 || cnt > 1)
    {
        ERR_PRINT("findItems failed\n, list count: %d, dev ip: %s\n", cnt, qstr_ip.toUtf8().constData());

#if 1
        QTreeWidgetItemIterator it(ui->tree_swich_dec);
        while (*it)
        {
            DBG_PRINT("item text: %s\n", (*it)->text(0).toUtf8().constData());
            ++it;
        }
#endif

        return ;
    }

    if (dev.b_alive)
    {
        (*list_item.begin())->setIcon(0, QIcon(":/image/dev_online.png"));
    }
    else
    {
        (*list_item.begin())->setIcon(0, QIcon(":/image/dev_offline.png"));
    }
}

void form_tvwall_config::slotDropEvent(int row, int col, QString data)
{
    DBG_PRINT("row: %d, col: %d, data: %s\n", row, col, data.toUtf8().constData());

    QTableWidgetItem *item = ui->tableWidget_tvwall->item(row, 1); //得到 解码器列 item
    if (NULL == item)
    {
        ERR_PRINT("NULL == item\n");
        return ;
    }

    //解除绑定
    if (data.isEmpty())
    {
        item->setText(QString::fromUtf8("")); //清空

        return ;
    }

    //check QString data
    QStringList str_list = data.split(QString::fromUtf8(":"));
    if (str_list.size() != 1)
    {
        ERR_PRINT("str_list size(%d) != 1, invalid\n", str_list.size());
        return ;
    }

    u32 dec_ip = inet_addr(data.toUtf8().constData());
    if (INADDR_NONE == dec_ip)
    {
        ERR_PRINT("dec ip invalid, str_nvr_ip: %s\n", data.toUtf8().constData());
        return ;
    }

    item->setText(data);
}

void form_tvwall_config::setupTableWidget()
{
    QStringList header;

    ui->tableWidget_tvwall->setupStyle(tablewidget_tvwall::style_screen);

    ui->tableWidget_tvwall->setColumnCount(3);
    ui->tableWidget_tvwall->setColumnWidth(0,100);
    ui->tableWidget_tvwall->setColumnWidth(1,200);
    ui->tableWidget_tvwall->verticalHeader()->setVisible(false);//列表头不可见
    ui->tableWidget_tvwall->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    ui->tableWidget_tvwall->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    ui->tableWidget_tvwall->setAlternatingRowColors(true);//奇偶行不同颜色显示
    ui->tableWidget_tvwall->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑

    //设置表头
    QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
    ui->tableWidget_tvwall->setHorizontalHeaderItem(0, __qtablewidgetitem);
    QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
    ui->tableWidget_tvwall->setHorizontalHeaderItem(1, __qtablewidgetitem1);
    QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
    ui->tableWidget_tvwall->setHorizontalHeaderItem(2, __qtablewidgetitem2);

    header.append(QString::fromUtf8("屏幕"));
    header.append(QString::fromUtf8("解码器"));
    header.append(QString::fromUtf8("操作"));
    ui->tableWidget_tvwall->setHorizontalHeaderLabels(header);

    ui->tableWidget_tvwall->setAcceptDrops(true);//拖拽效果 接受落下

    refreshTable();

    connect(ui->tableWidget_tvwall, SIGNAL(signalDropEvent(int,int,QString)),
            this, SLOT(slotDropEvent(int,int,QString)));
}

void form_tvwall_config::refreshTable()
{
    QTableWidgetItem *ptable_item = NULL;
    u32 cnt = rows * cols;

    DBG_PRINT("rows: %d, cols: %d, vdev.size: %d\n", rows, cols, vdev.size());

    ui->tableWidget_tvwall->clearContents();
    ui->tableWidget_tvwall->setRowCount(cnt);

    QString screen_text = QString::fromUtf8("屏幕");
    QString btn_text = QString::fromUtf8("解除绑定");
    QString ip_text;
    QPushButton *btn_unbind = NULL;
    struct in_addr in;
    u32 i;
    for (i=0; i<cnt; ++i)
    {
        if (i >= vdev.size())
        {
            ERR_PRINT("i(%d) >= vdev size(%d)\n", i, vdev.size());
            continue;
        }

        ptable_item = new QTableWidgetItem();
        ptable_item->setText(screen_text + QString("%1").arg(i+1));
        ui->tableWidget_tvwall->setItem(i, 0, ptable_item);

        ptable_item = new QTableWidgetItem();
        if (vdev.at(i) != 0)
        {
            in.s_addr = vdev.at(i);
            ip_text = QString::fromUtf8(inet_ntoa(in));
        }
        else
        {
            ip_text = QString("");//空
        }
        ptable_item->setText(ip_text);
        ui->tableWidget_tvwall->setItem(i, 1, ptable_item);

        btn_unbind = new QPushButton;
        btn_unbind->setText(btn_text);

        connect(btn_unbind, SIGNAL(clicked()), this, SLOT(btn_unbind_clicked()));

        ui->tableWidget_tvwall->setCellWidget(i, 2, btn_unbind);
    }
}

void form_tvwall_config::btn_unbind_clicked()
{
    QPushButton *btn = dynamic_cast<QPushButton *>(QObject::sender());//找到信号发送者
    QModelIndex index = ui->tableWidget_tvwall->indexAt(btn->pos());//定位按钮

    int row = index.row();
    int col = index.column();
    DBG_PRINT("btn at row: %d, col: %d\n", row, col);

#if 1
    slotDropEvent(row, col, QString::fromUtf8(""));

#else
    QTableWidgetItem *item = ui->tableWidget_tvwall->item(index.row(), index.column()-1); //得到 解码器列 item
    if (item)
    {
        item->setText(QString::fromUtf8(""));//清空
    }
#endif
}

void form_tvwall_config::on_btn_clr_clicked()
{
    QTableWidgetItem *pitem = NULL;
    int _rows = ui->tableWidget_tvwall->rowCount();
    int i = 0;

    for (i=0; i<_rows; ++i)
    {
        pitem = NULL;
        pitem = ui->tableWidget_tvwall->item(i, 1);
        if (pitem)
        {
            pitem->setText(QString::fromUtf8(("")));
        }
    }
}

void form_tvwall_config::on_btn_cancel_clicked()
{
    this->close();
}

void form_tvwall_config::on_btn_ok_clicked()
{    
    //数据格式说明：
    //行数|列数|解码器IP1 解码器IP2 ... 解码器IPn

    QTableWidgetItem *pitem = NULL;
    u32 dev_ip = 0;
    int i =0;
    int cnt = rows * cols;

    if (cnt <= 0)
    {
        ERR_PRINT("rows(%d) * cols(%d) = %d\n", rows, cols, cnt);
        return ;
    }

    if (cnt != ui->tableWidget_tvwall->rowCount())
    {
        ERR_PRINT("cnt(%d) != ui->tableWidget_tvwall->rowCount(): %d\n", cnt, ui->tableWidget_tvwall->rowCount());
        return ;
    }

    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly);

    dataStream << rows << cols;

    QString item_text;
    for (i=0; i<cnt; ++i)
    {
        pitem = NULL;
        pitem = ui->tableWidget_tvwall->item(i, 1);
        if (pitem)
        {
            item_text = pitem->text();
            if (item_text.isEmpty())
            {
                dev_ip = 0;
            }
            else
            {
                DBG_PRINT("i: %d, %s\n", i, item_text.toUtf8().constData());
                dev_ip = inet_addr(item_text.toUtf8().constData());
                if (dev_ip == INADDR_NONE)
                {
                    ERR_PRINT("line: %d, ip invalid\n", i);
                    dev_ip = 0;
                }
            }
        }
        else
        {
            dev_ip = 0;
        }

        dataStream << dev_ip;
    }

    emit signal_tvwall_data(data);
    DBG_PRINT("emit\n");
    //sleep(1);
    this->close();
}
