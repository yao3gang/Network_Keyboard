#ifndef WZD_TVWALL_LAYOUT_H
#define WZD_TVWALL_LAYOUT_H

#include <QWizard>
#include <QMutex>

namespace Ui {
class wzd_tvWall_layout;
}

class wzd_tvWall_layout : public QWizard
{
    Q_OBJECT
    
public:
    //单例模式,保证一个程序只存在一个输入法实例对象
    static wzd_tvWall_layout *Instance() {
        if (!_instance) {
            static QMutex mutex;
            QMutexLocker locker(&mutex);
            if (!_instance) {
                _instance = new wzd_tvWall_layout;
            }
        }
        return _instance;
    }

    explicit wzd_tvWall_layout(QWidget *parent = 0);
    ~wzd_tvWall_layout();
    
private:
    Ui::wzd_tvWall_layout *ui;
    static wzd_tvWall_layout *_instance;     //实例对象
};

#endif // WZD_TVWALL_LAYOUT_H
