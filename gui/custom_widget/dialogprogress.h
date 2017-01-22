#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include <QDialog>

namespace Ui {
class DialogProgress;
}

class DialogProgress : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogProgress(QWidget *parent = 0);
    ~DialogProgress();
    void setTitle(QString str);
    void setRange(int minimum, int maximum);

public slots:
    void slotSetProgressBarValue(int value);
    void slotClose(int flag); //0 success, 1 底层出错

private:
    void InitStyle();

public slots:
    void on_btnCancel_clicked();

private:
    Ui::DialogProgress *ui;
};

#endif // DIALOGPROGRESS_H
