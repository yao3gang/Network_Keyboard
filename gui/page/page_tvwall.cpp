#include "page_tvwall.h"
#include "ui_page_tvwall.h"

page_tvWall::page_tvWall(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_tvWall)
{
    ui->setupUi(this);
}

page_tvWall::~page_tvWall()
{
    delete ui;
}
