#ifndef DLG_DEV_TREE_H
#define DLG_DEV_TREE_H

#include <QDialog>

namespace Ui {
class dlg_dev_tree;
}

enum e_dev_type
{
    dev_dec,
    dev_nvr,
};

class dlg_dev_tree : public QDialog
{
    Q_OBJECT
    
public:
    explicit dlg_dev_tree(e_dev_type dev_type, QString tooltip, bool b_Child, bool b_checkbox, QWidget *parent = 0);
    ~dlg_dev_tree();
    
private:

    dlg_dev_tree();
    Ui::dlg_dev_tree *ui;
};

#endif // DLG_DEV_TREE_H
