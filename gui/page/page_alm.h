#ifndef PAGE_ALM_H
#define PAGE_ALM_H

#include <QWidget>

namespace Ui {
class page_alm;
}

class page_alm : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_alm(QWidget *parent = 0);
    ~page_alm();
    
private:
    Ui::page_alm *ui;
    void init_form();

private slots:
    void button_clicked();

    void on_btn_clr_clicked();
};

#endif // PAGE_ALM_H
