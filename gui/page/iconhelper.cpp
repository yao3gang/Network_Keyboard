#include <QPainter>

#include "iconhelper.h"
#include "appdef.h"

IconHelper *IconHelper::_instance = 0;
IconHelper::IconHelper(QObject *):
    QObject(qApp)
{
    int fontId = QFontDatabase::addApplicationFont(":/image/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontName);

    icoSize = QSize(16, 16);
    icoTextColor = Qt::red;
    icoTextSize = FontSize;
}

void IconHelper::SetIcon(QLabel *lab, QChar c)
{
    SetIcon(lab, c, FontSize);
}

void IconHelper::SetIcon(QAbstractButton *btn, QChar c)
{
    SetIcon(btn, c, FontSize);
}

void IconHelper::SetIcon(QLabel *lab, QChar c, int size)
{
    iconFont.setPointSize(size);
    lab->setFont(iconFont);
    lab->setText(c);
}

void IconHelper::SetIcon(QAbstractButton *btn, QChar c, int size)
{
    iconFont.setPointSize(size);
    btn->setFont(iconFont);
    btn->setText(c);
}

void IconHelper::SetIcoMain(QLabel *labMain)
{
    SetIcoMain(labMain, FontSize);
}

void IconHelper::SetIcoMenu(QAbstractButton *btnMenu)
{
    SetIcoMenu(btnMenu, FontSize);
}

void IconHelper::SetIcoMin(QAbstractButton *btnMin)
{
    SetIcoMin(btnMin, FontSize);
}

void IconHelper::SetIcoMax(QAbstractButton *btnMax)
{
    SetIcoMax(btnMax, FontSize);
}

void IconHelper::SetIcoNormal(QAbstractButton *btnNormal)
{
    SetIcoNormal(btnNormal, FontSize);
}

void IconHelper::SetIcoClose(QAbstractButton *btnClose)
{
    SetIcoClose(btnClose, FontSize);
}

void IconHelper::SetIcoMain(QLabel *labMain, int size)
{
    iconFont.setPointSize(size);
    labMain->setFont(iconFont);
    labMain->setText(QChar(ICOMAIN));
}

void IconHelper::SetIcoMenu(QAbstractButton *btnMenu, int size)
{
    iconFont.setPointSize(size);
    btnMenu->setFont(iconFont);
    btnMenu->setText(QChar(ICOMENU));
}

void IconHelper::SetIcoMin(QAbstractButton *btnMin, int size)
{
    iconFont.setPointSize(size);
    btnMin->setFont(iconFont);
    btnMin->setText(QChar(ICOMIN));
}

void IconHelper::SetIcoMax(QAbstractButton *btnMax, int size)
{
    iconFont.setPointSize(size);
    btnMax->setFont(iconFont);
    btnMax->setText(QChar(ICOMAX));
}

void IconHelper::SetIcoNormal(QAbstractButton *btnNormal, int size)
{
    iconFont.setPointSize(size);
    btnNormal->setFont(iconFont);
    btnNormal->setText(QChar(ICONORMAL));
}

void IconHelper::SetIcoClose(QAbstractButton *btnClose, int size)
{
    iconFont.setPointSize(size);
    btnClose->setFont(iconFont);
    btnClose->setText(QChar(ICOCLOSE));
}

QIcon IconHelper::IcoMenu()
{
    return Ico(QChar(ICOMENU), icoTextSize);
}

QIcon IconHelper::IcoMin()
{
    return Ico(QChar(ICOMIN), icoTextSize);
}

QIcon IconHelper::IcoMax()
{
    return Ico(QChar(ICOMAX), icoTextSize);
}

QIcon IconHelper::IcoNormal()
{
    return Ico(QChar(ICONORMAL), icoTextSize);
}

QIcon IconHelper::IcoClose()
{
    return Ico(QChar(ICOCLOSE), icoTextSize);
}

QIcon IconHelper::Ico(QChar c)
{
    return Ico(c, icoTextSize);
}

QIcon IconHelper::Ico(QChar c, int size)
{
    QImage image(icoSize, QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);

    QPen pen = painter.pen();
    pen.setColor(icoTextColor);
    iconFont.setPixelSize(size);
    painter.setPen(pen);
    painter.setFont(iconFont);

    painter.drawText(image.rect(), Qt::AlignCenter, c);
    return QIcon(QPixmap::fromImage(image));
}
