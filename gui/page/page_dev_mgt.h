#ifndef PAGE_DEV_MGT_H
#define PAGE_DEV_MGT_H

#include <QWidget>

namespace Ui {
class page_dev_mgt;
}

class page_dev_mgt : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_dev_mgt(QWidget *parent = 0);
    ~page_dev_mgt();

protected:
    void showEvent( QShowEvent * event );

private slots:
    void on_btn_srh_clicked();
    void cmbSrhChange(int index);
    void cmbAddChange(int index);
    void tableWidgetSrhDBClicked(int row, int column);
    void on_btn_add_clicked();
    void on_btn_info_clicked();

private:
    Ui::page_dev_mgt *ui;
    QStringList strlist_devtype;
};

#endif // PAGE_DEV_MGT_H
