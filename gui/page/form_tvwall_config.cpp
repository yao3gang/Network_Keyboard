#include "form_tvwall_config.h"
#include "ui_form_tvwall_config.h"

form_tvwall_config::form_tvwall_config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_tvwall_config)
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
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle(QString::fromUtf8("电视墙配置"));
    ui->tableWidget_tvwall->verticalHeader()->setVisible(false);//列表头不可见
    ui->tableWidget_tvwall->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    ui->tableWidget_tvwall->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    ui->tableWidget_tvwall->setAlternatingRowColors(true);//奇偶行不同颜色显示
    ui->tableWidget_tvwall->setColumnWidth(0,100);
    ui->tableWidget_tvwall->setColumnWidth(1,200);
}

void form_tvwall_config::on_btn_cancel_clicked()
{
    this->close();
}

void form_tvwall_config::on_btn_ok_clicked()
{
    this->close();
}
