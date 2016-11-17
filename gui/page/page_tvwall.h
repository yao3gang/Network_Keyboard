#ifndef PAGE_TVWALL_H
#define PAGE_TVWALL_H

#include <QWidget>

namespace Ui {
class page_tvWall;
}

class page_tvWall : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_tvWall(QWidget *parent = 0);
    ~page_tvWall();
    
private:
    Ui::page_tvWall *ui;
};

#endif // PAGE_TVWALL_H
