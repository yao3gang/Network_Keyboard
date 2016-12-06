#ifndef TREEWIDGET_DEV_H
#define TREEWIDGET_DEV_H

#include <QTreeWidget>
#include "biz_device.h"
#include "gui_dev.h"

class treewidget_dev : public QTreeWidget
{
    Q_OBJECT
public:

public:
    explicit treewidget_dev(QWidget *parent = 0);
    ~treewidget_dev();

protected:
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    
private slots:
    //void refreshDevInfo(SGuiDev dev);

private:

};

#endif // TREEWIDGET_DEV_H
