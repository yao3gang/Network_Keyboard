#include "mybutton.h"

myButton::myButton(QWidget *parent) :
    QPushButton(parent)
{
}

void myButton::enterEvent ( QEvent * event )
{
    emit eventSignal(tr("enterEvent"));
}

void myButton::leaveEvent ( QEvent * event )
{
    emit eventSignal(tr(""));
}
