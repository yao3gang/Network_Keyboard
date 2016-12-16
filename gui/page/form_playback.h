#ifndef FORM_PLAYBACK_H
#define FORM_PLAYBACK_H

#include <QtGui>

#include "gui_dev.h"

namespace Ui {
class form_playback;
}

class form_playback : public QWidget
{
    Q_OBJECT
    
public:
    explicit form_playback(QWidget *parent = 0);
    ~form_playback();

protected:
    void mousePressEvent(QMouseEvent *);

private:
    void init_style();
    void init_form();//控件
    void setupTreeWidgetNvr();
    void setupWidgetLeft();
    void setupWidgetBottom();


private:
    Ui::form_playback *ui;
    QMutex mutex;

private slots:
    void showTableWidget(bool b);
    void refreshDevInfo(SGuiDev dev);
};

#endif // FORM_PLAYBACK_H
