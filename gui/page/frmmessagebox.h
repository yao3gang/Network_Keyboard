#ifndef FRMMESSAGEBOX_H
#define FRMMESSAGEBOX_H

#include <QDialog>

namespace Ui
{
    class frmMessageBox;
}

class frmMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit frmMessageBox(QWidget *parent = 0);
    ~frmMessageBox();

    void SetMessage(QString msg, int type);

private slots:
    void on_btnOk_clicked();

private:
    Ui::frmMessageBox *ui;

private:
    void InitStyle();

};

#ifdef __cplusplus
extern "C" {
#endif

//extern call function
//显示信息框,仅确定按钮
void ShowMessageBoxInfo(QString info);

//显示错误框,仅确定按钮
void ShowMessageBoxError(QString info);

//显示询问框,确定和取消按钮
int ShowMessageBoxQuesion(QString info);

#ifdef __cplusplus
}
#endif

#endif // FRMMESSAGEBOX_H
