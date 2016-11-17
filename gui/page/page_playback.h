#ifndef PAGE_PLAYBACK_H
#define PAGE_PLAYBACK_H

#include <QWidget>

namespace Ui {
class page_playback;
}

class page_playback : public QWidget
{
    Q_OBJECT
    
public:
    explicit page_playback(QWidget *parent = 0);
    ~page_playback();
    
protected:
    void showEvent( QShowEvent * event );

private:
    Ui::page_playback *ui;
};

#endif // PAGE_PLAYBACK_H
