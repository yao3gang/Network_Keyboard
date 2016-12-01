#include "page_tvwall.h"
#include "ui_page_tvwall.h"

page_tvWall::page_tvWall(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_tvWall)
{
    ui->setupUi(this);
    init_form();
}

page_tvWall::~page_tvWall()
{
    delete ui;
}

void page_tvWall::init_form()
{
    ui->tableWidget_tvwall->setupStyle(tablewidget_tvwall::style_chn);

}
