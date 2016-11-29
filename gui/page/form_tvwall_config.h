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
    
private:
    Ui::form_tvwall_config *ui;
};

#endif // FORM_TVWALL_CONFIG_H
