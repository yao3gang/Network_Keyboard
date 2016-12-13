#ifndef TABLEWIDGET_TVWALL_H
#define TABLEWIDGET_TVWALL_H

#include <QtGui>

#include "types.h"

class tablewidget_tvwall : public QTableWidget
{
    Q_OBJECT
public:
    const static QString style_screen;//屏幕和解码器绑定
    const static QString style_chn;//解码器通道和NVR通道绑定，即解码器通道申请NVR通道的码流

public:
    explicit tablewidget_tvwall(QWidget *parent = 0);
    ~tablewidget_tvwall();
    void setupStyle(QString str);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void signalDropEvent(int row, int col, QString data);

public slots:

private:
    QString style;
};

#endif // TABLEWIDGET_TVWALL_H
