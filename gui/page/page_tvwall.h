#ifndef PAGE_TVWALL_H
#define PAGE_TVWALL_H

#include <QWidget>

#include "page_dev_mgt.h"

namespace Ui {
class page_tvWall;
}

class page_tvWall : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_tvWall(QWidget *parent = 0);
    ~page_tvWall();

protected:
    void showEvent(QShowEvent *);

private:
    void init_form();
    void setupTreeWidgetScreen();
    void refreshTreeWidgetScreen();
    void setupTreeWidgetNvr();
    void refreshTreeWidgetNvr();
    void setupTableWidget();
    void refreshTableWidget();

private slots:
    void refreshDevInfo(SGuiDev dev);

private:
    Ui::page_tvWall *ui;
    page_dev_mgt *page_dev;
};

#endif // PAGE_TVWALL_H
