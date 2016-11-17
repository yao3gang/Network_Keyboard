#ifndef PAGE_PREVIEW_H
#define PAGE_PREVIEW_H

#include <QWidget>

namespace Ui {
class page_preview;
}

class page_preview : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_preview(QWidget *parent = 0);
    ~page_preview();

protected:
    void	hideEvent ( QHideEvent * event );

private:
    Ui::page_preview *ui;
    void init_form();
};

#endif // PAGE_PREVIEW_H
