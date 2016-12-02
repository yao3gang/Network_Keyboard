#ifndef TREEWIDGET_DEV_H
#define TREEWIDGET_DEV_H

#include <QTreeWidget>
#include "biz_device.h"
#include "gui_dev.h"

class treewidget_dev : public QTreeWidget
{
    Q_OBJECT
public:
    //单级：仅显示设备  两级：还显示通道
    enum LEVELNUMS
    {
        NONE_LEVEL,
        SINGLE_LEVEL,
        TWO_LEVELS,
        MAX_LEVELS,
    };

public:
    explicit treewidget_dev(QWidget *parent = 0);
    ~treewidget_dev();

    void setupForm(LEVELNUMS levels);
    void setupDevType(EM_DEV_TYPE _dev_type);

protected:
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    
private slots:
    void refreshDevInfo(SGuiDev dev);

private:
    EM_DEV_TYPE dev_type;
    LEVELNUMS level_nums;
    QStringList strlist_devtype;
};

#endif // TREEWIDGET_DEV_H
