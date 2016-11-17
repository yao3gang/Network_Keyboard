#include "mywidget.h"
#include "mybutton.h"
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPen>
#include <QtGui/QFont>

myWidget::myWidget(QWidget *parent) :
    QWidget(parent)
{
    but = new myButton(this);
    but->setObjectName(QString::fromUtf8("pushButton"));
    but->setGeometry(QRect(0, 60, 60, 30));
    but->setText(tr("哈哈"));

    lab = new QLabel(this);
    lab->setObjectName(QString::fromUtf8("lab"));
    lab->setGeometry(QRect(0, 100, 100, 30));

    //connect(but, SIGNAL(eventSignal(QString)), lab, SLOT(setText(QString)));
    connect(but, SIGNAL(eventSignal(QString)), this, SLOT(rcvMsg(QString)));
}
void myWidget::rcvMsg(const QString &msg)
{
    lab->setText(msg);
}

void myWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 背景设置透明
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.fillRect(this->rect(), QBrush(Qt::transparent));

    // 画圆角边框
    painter.save();
    painter.setBrush(QColor(196, 203, 213));
    painter.drawRoundedRect(QRect(0, 0, width() - 1, height() - 1), 10, 10);
    painter.restore();

    // 转换窗口坐标，否则左上角的圆弧画不出来
    painter.save();
    painter.setWindow(0, 30, 600, 230);
    // 画左上角部分，此处简单处理，用直线组合的
    QPainterPath path;
    path.moveTo(10, 30);
    path.arcTo(0, 30, 20, 20, 90, 90);
    path.lineTo(0, 60);
    path.lineTo(100, 60);
    path.arcTo(70, 0, 60, 60, 270, 90);// 不转换窗口坐标，这个圆弧画不出来

    // 填充左上角区域
    painter.setBrush(QColor(174, 182, 197));
    painter.drawPath(path);
    painter.restore();

    // 写文字标题
    painter.save();
    painter.setWindow(0, 0, 600, 200);
    QPen pen;
    pen.setColor(QColor(Qt::black));
    pen.setWidth(0);
    painter.setPen(pen);

    QFont font;
    //font.setBold(true);
    font.setFamily("wenquanyi");
    font.setPixelSize(16);
    painter.setFont(font);
    painter.drawText(QRect(10, 5, 80, 20), Qt::AlignLeft | Qt::AlignVCenter, tr("基本功能"));
    painter.restore();

}

