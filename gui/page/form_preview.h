#ifndef FORM_PREVIEW_H
#define FORM_PREVIEW_H

#include <QWidget>

namespace Ui {
class form_preview;
}

class form_preview : public QWidget
{
    Q_OBJECT
    
public:
    explicit form_preview(QWidget *parent = 0);
    ~form_preview();

private:
    void init_style();

private:
    Ui::form_preview *ui;
};

#endif // FORM_PREVIEW_H
