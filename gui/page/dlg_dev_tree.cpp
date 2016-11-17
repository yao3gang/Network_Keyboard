#include "dlg_dev_tree.h"
#include "ui_dlg_dev_tree.h"

dlg_dev_tree::dlg_dev_tree(e_dev_type dev_type, QString tooltip, bool b_Child, bool b_checkbox, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_dev_tree)
{
    ui->setupUi(this);
}

dlg_dev_tree::~dlg_dev_tree()
{
    delete ui;
}
