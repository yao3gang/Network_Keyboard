#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>

class myWidget : public QWidget
{
    Q_OBJECT

    QPushButton *but;
    QLabel *lab;

public:
    explicit myWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

signals:
    
public slots:
    void rcvMsg(const QString &);
};

#endif // MYWIDGET_H
