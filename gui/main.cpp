//sys header
#include <QApplication>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QDebug>
#include <QFile>
#include <QTextCodec>  //添加头文件
#include <QWSServer>
#include <QTranslator>

//local header
#include "bond.h"
#include "biz.h"
#include "hisi_sys.h"
#include "types.h"

#include "appdef.h"
#include "page_manager.h"
#include "page_dev_mgt.h"
#include "page_main.h"
#include "frminput.h"

#if 0
/****************************************************************

****************************************************************/
//确保程序在系统中单例运行
class myApplication : public QApplication
{
public:
    myApplication(int &argc,char **argv);
    ~myApplication();
    bool isRunning();
private:
    bool m_IsRun;
    QSharedMemory m_sharedMemory;
};

myApplication::myApplication(int &argc, char **argv)
    :QApplication(argc,argv)
{
    m_sharedMemory.setKey("NetworkKeyboard");
    if (m_sharedMemory.attach())
    {
        m_IsRun = true;
    }else
    {
        m_IsRun = false;
        if (!m_sharedMemory.create(1))
        {
            qDebug()<<"Unable to create single instance !";
            qDebug()<<m_sharedMemory.errorString();
            return;
        }
    }
}

myApplication:: ~myApplication()
{

}

bool myApplication::isRunning()
{
    return m_IsRun;
}

#endif
/*****************************************************************************/

int main(int argc, char *argv[])
{
    //逻辑层初始化
    if (HisiSysInit())
    {
        qDebug("HisiSysInit failed\n");
        return 0;
    }
    qDebug("HisiSysInit success\n");

    if (BizInit())
    {
        qDebug("BizInit failed\n");
        return 0;
    }
    qDebug("BizInit success\n");

    //while (1)
    //{
    //    sleep(1);
    //}
    //sleep(5);

    //应用层初始化
    QApplication app(argc, argv);
    QWSServer::setBackground(QColor(0, 0, 0, 0));

    QTranslator translator;
    if (translator.load("qt_zh_CN.qm", ":/translations") != true)
    {
        qDebug("load qt_zh_CN failed\n");
    }
    else
    {
        qDebug("load qt_zh_CN success\n");
    }
    app.installTranslator(&translator);

    QFont font;
    font.setPointSize(FontSize);
    font.setFamily("wenquanyi");
    app.setFont(font);


    //支持Tr中文
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    //确保单例运行
    //if (app.isRunning())
        //return 0;

    QFile file(":/qss/black.css");
    file.open(QFile::ReadOnly);
    app.setStyleSheet(file.readAll());

    frmInput::Instance()->Init("control", "black", FontSize + 1, FontSize);
    page_main w;
    w.show();
    if (registerPage(PAGE_MAIN, &w))
    {
        ERR_PRINT("registerPage PAGE_MAIN failed\n");
        return -FAILURE;
    }

    gp_bond->guiEnableRcvNotify();//初始化完成，是能接收下层通知信息

    //同步所有设备信息
    page_dev_mgt * page_dev = (page_dev_mgt *)getPage(PAGE_DEV_MGT);
    if (NULL == page_dev)
    {
        ERR_PRINT("getPage(PAGE_DEV_MGT) failed\n");
        return -FAILURE;
    }

    page_dev->syncAllDevInfo();

    return app.exec();
}
