#ifndef TABLEWIDGET_TVWALL_H
#define TABLEWIDGET_TVWALL_H

#include <QHeaderView>
#include <QTableWidget>
#include <QString>

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

signals:
    
public slots:

private:
    QString style;
};

#endif // TABLEWIDGET_TVWALL_H
