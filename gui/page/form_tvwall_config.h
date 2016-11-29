#ifndef FORM_TVWALL_CONFIG_H
#define FORM_TVWALL_CONFIG_H

#include <QWidget>

namespace Ui {
class form_tvwall_config;
}

class form_tvwall_config : public QWidget
{
    Q_OBJECT
    
public:
    explicit form_tvwall_config(QWidget *parent = 0);
    ~form_tvwall_config();

private slots:
    void on_btn_cancel_clicked();
    void on_btn_ok_clicked();

private:
    void init_form();//控件

private:
    Ui::form_tvwall_config *ui;
};

#endif // FORM_TVWALL_CONFIG_H
