#include "tablewidget_tvwall.h"

const QString tablewidget_tvwall::style_screen = QString::fromUtf8("screen_binding");
const QString tablewidget_tvwall::style_chn = QString::fromUtf8("chn_binding");;

tablewidget_tvwall::tablewidget_tvwall(QWidget *parent) :
    QTableWidget(parent)
{

}

tablewidget_tvwall::~tablewidget_tvwall()
{

}

void tablewidget_tvwall::setupStyle(QString str)
{
    QStringList header;
    style = str;

    if (style == style_screen)//
    {
        //设置表头
        setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        setHorizontalHeaderItem(2, __qtablewidgetitem2);

        header.append(QString::fromUtf8("屏幕"));
        header.append(QString::fromUtf8("解码器"));
        header.append(QString::fromUtf8("解除绑定"));

    }
    else if (style == style_chn)
    {
        //设置表头
        setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        setHorizontalHeaderItem(2, __qtablewidgetitem2);

        header.append(QString::fromUtf8("屏幕画面"));
        header.append(QString::fromUtf8("NVR通道"));
        header.append(QString::fromUtf8("解除绑定"));
    }
    else
    {
        ERR_PRINT("style(%s) not support\n", str.toUtf8().constData());
        return;
    }

    setHorizontalHeaderLabels(header);
    verticalHeader()->setVisible(false);//列表头不可见
    setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    setAlternatingRowColors(true);//奇偶行不同颜色显示
    setColumnWidth(0,100);
    setColumnWidth(1,200);

    setAcceptDrops(true);//拖拽效果 接受落下
}

void tablewidget_tvwall::dragEnterEvent(QDragEnterEvent *event)
{

}

void tablewidget_tvwall::dragLeaveEvent(QDragLeaveEvent *event)
{

}

void tablewidget_tvwall::dragMoveEvent(QDragMoveEvent *event)
{

}

void tablewidget_tvwall::dropEvent(QDropEvent *event)
{

}
