#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QDebug>
#include <QCursor>
#include <string>

#include "page_dev_mgt.h"
#include "ui_page_dev_mgt.h"
#include "biz_device.h"

page_dev_mgt::page_dev_mgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_dev_mgt)
{
    ui->setupUi(this);

    strlist_devtype.append(QString::fromUtf8("NVR"));
    strlist_devtype.append(QString::fromUtf8("轮巡型解码器"));
    strlist_devtype.append(QString::fromUtf8("切换型解码器"));

    ui->cmb_srh_type->addItems(strlist_devtype);
    ui->cmb_srh_type->setCurrentIndex(0);

    ui->cmb_added_type->addItems(strlist_devtype);
    ui->cmb_added_type->setCurrentIndex(0);

    //ui->tableWidget_srh->verticalHeader()->setVisible(false);//列表头不可见
    ui->tableWidget_srh->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    ui->tableWidget_srh->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    ui->tableWidget_srh->setSelectionMode(QAbstractItemView::MultiSelection);//可以多选行
    ui->tableWidget_srh->setAlternatingRowColors(true);//奇偶行不同颜色显示
    ui->tableWidget_srh->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    ui->tableWidget_srh->setColumnWidth(0,50);
    ui->tableWidget_srh->setColumnWidth(1,200);
    ui->tableWidget_srh->setColumnWidth(2,200);
    ui->tableWidget_srh->setDragEnabled(true);//设置内部部件支持拖拽
    ui->tableWidget_srh->setAcceptDrops(true);
    ui->tableWidget_srh->setDropIndicatorShown(false);

    //ui->tableWidget_dev->verticalHeader()->setVisible(false);//列表头不可见
    ui->tableWidget_dev->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，删除第一行
    ui->tableWidget_dev->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_dev->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableWidget_dev->setAlternatingRowColors(true);
    ui->tableWidget_dev->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    ui->tableWidget_dev->setColumnWidth(0,50);
    ui->tableWidget_dev->setColumnWidth(1,200);
    ui->tableWidget_dev->setColumnWidth(2,200);
    ui->tableWidget_dev->setColumnWidth(3,100);

    connect(ui->cmb_srh_type, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbSrhChange(int)));
    connect(ui->cmb_added_type, SIGNAL(currentIndexChanged(int)), this, SLOT(cmdAddChange(int)));

    connect(ui->tableWidget_srh, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetSrhDBClicked(int, int)));
}

page_dev_mgt::~page_dev_mgt()
{
    delete ui;
}

void page_dev_mgt::on_btn_srh_clicked()
{
    VD_BOOL b_ret = FALSE;
    struct in_addr in;

    std::vector<SBiz_DeviceInfo_t> vnvr_list;
    std::vector<SBiz_DeviceInfo_t> vpatrol_dec_list;
    std::vector<SBiz_DeviceInfo_t> vswitch_dec_list;
    std::vector<SBiz_DeviceInfo_t> *pvdev_list = NULL;
    std::vector<SBiz_DeviceInfo_t>::iterator iter;

    int line = 0;
    int dev_type_idx = ui->cmb_srh_type->currentIndex();

    ui->tableWidget_srh->setRowCount(0);
    ui->tableWidget_srh->clearContents();

    switch (dev_type_idx)
    {
        case 0: //NVR
            pvdev_list = &vnvr_list;
            break;
        case 1: //"轮巡型解码器"
            pvdev_list = &vpatrol_dec_list;
            break;
        case 2: //"切换型解码器"
            pvdev_list = &vswitch_dec_list;
            break;
        default:
            DBG_PRINT("dev type not support\n");
            return;
    }


    //::setShape(Qt::WaitCursor);
    //setCursor(QCursor(Qt::WaitCursor));
    b_ret = BizDevSearch(&vnvr_list, &vpatrol_dec_list, &vswitch_dec_list);
    //::setShape(Qt::ArrowCursor);
    //setCursor(QCursor(Qt::WaitCursor));
    if (!b_ret)
    {
        ui->tableWidget_srh->setRowCount(pvdev_list->size());//行数，该类设备总数

        for (line = 0, iter = pvdev_list->begin();
             iter != pvdev_list->end();
             ++line, ++iter)
        {
            in.s_addr = iter->deviceIP;
            DBG_PRINT("devtype: %d(%s) line: %d, list deviceIP: %s\n",
                      iter->devicetype,
                      strlist_devtype.at(iter->devicetype-1).toUtf8().constData(),
                      line,
                      inet_ntoa(in));
            QTableWidgetItem *pitem = new QTableWidgetItem;
            pitem->setCheckState(Qt::Unchecked);
            //ui->tableWidget_srh->setItem(line, 0, new QTableWidgetItem(QString("%1").arg(line+1)));
            ui->tableWidget_srh->setItem(line, 0, pitem);
            ui->tableWidget_srh->setItem(line, 1, new QTableWidgetItem(strlist_devtype.at(iter->devicetype-1)));
            ui->tableWidget_srh->setItem(line, 2, new QTableWidgetItem(QString(inet_ntoa(in))));
            ui->tableWidget_srh->setItem(line, 3, new QTableWidgetItem(QString("%1").arg(iter->maxChnNum)));
        }
    }
}

void page_dev_mgt::cmbSrhChange(int index)
{
    ui->tableWidget_srh->setRowCount(0);
    ui->tableWidget_srh->clearContents();
}

void page_dev_mgt::cmbAddChange(int index)
{

}

void page_dev_mgt::tableWidgetSrhDBClicked(int row, int column)
{
    //ui->tableWidget_srh->setCurrentCell(row, column, QItemSelectionModel::Select);//设置该行为选中
    ui->tableWidget_srh->selectRow(row);
    DBG_PRINT("DoubleClicked row: %d, col: %d\n", row, column);
}

void page_dev_mgt::showEvent( QShowEvent * event )
{
    ui->tableWidget_srh->setRowCount(0);
    ui->tableWidget_srh->clearContents();
}

void page_dev_mgt::on_btn_add_clicked()
{
    //DBG_PRINT("currentRow: %d\n", ui->tableWidget_srh->currentRow());
    //ui->tableWidget_srh->clearSelection();//取消选中状态
    //DBG_PRINT("rowCount: %d\n", ui->tableWidget_srh->rowCount());
    u32 srh_row = ui->tableWidget_srh->rowCount();
    u32 add_row = 0;
    u32 i=0;

    for (i=0; i<srh_row; ++i)//扫描搜索表格所有行
    {
        if (ui->tableWidget_srh->item(i, 0)->checkState())
        {
            add_row = ui->tableWidget_dev->rowCount();
            ui->tableWidget_dev->insertRow(add_row);

            QTableWidgetItem *pitem = new QTableWidgetItem;
            pitem->setCheckState(Qt::Unchecked);
            ui->tableWidget_dev->setItem(add_row, 0, pitem);
            ui->tableWidget_dev->setItem(add_row, 1, ui->tableWidget_srh->item(i, 1)->clone());
            ui->tableWidget_dev->setItem(add_row, 2, ui->tableWidget_srh->item(i, 2)->clone());
            ui->tableWidget_dev->setItem(add_row, 3, ui->tableWidget_srh->item(i, 3)->clone());
            ui->tableWidget_dev->setItem(add_row, 4, new QTableWidgetItem(QString::fromUtf8("在线")));
        }
    }

    ui->tableWidget_dev->sortByColumn(2, Qt::AscendingOrder);
}


