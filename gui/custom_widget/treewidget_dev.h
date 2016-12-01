#ifndef TREEWIDGET_DEV_H
#define TREEWIDGET_DEV_H

#include <QTreeWidget>
#include "biz_device.h"

class treewidget_dev : public QTreeWidget
{
    Q_OBJECT
public:
    explicit treewidget_dev(QWidget *parent = 0);
    ~treewidget_dev();

    void setupStyle(EM_DEV_TYPE _dev_type);
signals:
    
public slots:
    
private:
    EM_DEV_TYPE dev_type;
};

#endif // TREEWIDGET_DEV_H
