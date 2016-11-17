#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class myButton : public QPushButton
{
    Q_OBJECT
public:
    explicit myButton(QWidget *parent = 0);

protected:
    void enterEvent ( QEvent * event );
    void leaveEvent ( QEvent * event );

signals:
    void eventSignal (const QString &);
public slots:
    
};

#endif // MYBUTTON_H
