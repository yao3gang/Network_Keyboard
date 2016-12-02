#include "form_tvwall_config.h"
#include "ui_form_tvwall_config.h"

#include "biz_device.h"

form_tvwall_config::form_tvwall_config(QWidget *parent, OpenMode mode) :
    QWidget(parent),
    ui(new Ui::form_tvwall_config),
    open_mode(mode)
{
    ui->setupUi(this);
    init_form();
}

form_tvwall_config::~form_tvwall_config()
{
    delete ui;
}

void form_tvwall_config::init_form()//控件
{
    Qt::WindowFlags flag = windowFlags();
    flag &= ~(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);//去掉最大化/最小化按键
    //flag |= Qt::WindowStaysOnTopHint;
    setWindowFlags(flag);
    setWindowTitle(QString::fromUtf8("电视墙配置"));

    ui->tableWidget_tvwall->setupStyle(tablewidget_tvwall::style_screen);
    ui->tableWidget_tvwall->setAcceptDrops(true);
    //setup_switch_dec_tree();
    ui->tree_swich_dec->setupForm(treewidget_dev::TWO_LEVELS);//SINGLE_LEVEL单级：仅显示设备
    ui->tree_swich_dec->setupDevType(EM_SWITCH_DEC);
    ui->tree_swich_dec->setDragEnabled(true);
    ui->tree_swich_dec->setAcceptDrops(true);
}

void form_tvwall_config::on_btn_cancel_clicked()
{
    this->close();
}

void form_tvwall_config::on_btn_ok_clicked()
{
    this->close();
}
