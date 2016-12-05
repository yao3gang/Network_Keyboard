#ifndef FORM_TVWALL_CONFIG_H
#define FORM_TVWALL_CONFIG_H

#include <QtGui>
#include <vector>

#include "types.h"

namespace Ui {
class form_tvwall_config;
}

class form_tvwall_config : public QWidget
{
    Q_OBJECT
    
public:
    enum OpenMode
    {
        OpenModeNew = 1,
        OpenModeModfiy,
    };

    explicit form_tvwall_config(OpenMode mode=OpenModeNew, int _rows=0, int _cols=0, std::vector<u32> *pvdev=NULL, QWidget *parent=0);
    ~form_tvwall_config();

private slots:
    void on_btn_clr_clicked();
    void on_btn_cancel_clicked();
    void on_btn_ok_clicked();
    void btn_unbind_clicked();
    void rowsChanged(QString text);
    void colsChanged(QString text);

private:
    void init_form();//控件
    void setupTreeWidget();
    void setupTableWidget();
    void rowsOrColsChanged(u32 _rows, u32 _cols);
    void refreshTable();

private:
    Ui::form_tvwall_config *ui;
    OpenMode open_mode;
    u32 rows;
    u32 cols;
    u32 nTvwall_nums;
    u32 nTvwall_devs_max;
    u32 nTvwall_devs_per_line;
    u32 nTvwall_devs_per_col;
    std::vector<u32> vdev;
    QRegExpValidator *pvalidator;

signals:
    void signal_tvwall_data(QByteArray data);
};

#endif // FORM_TVWALL_CONFIG_H
