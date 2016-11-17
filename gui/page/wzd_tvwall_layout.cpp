#include "wzd_tvwall_layout.h"
#include "ui_wzd_tvwall_layout.h"

wzd_tvWall_layout *wzd_tvWall_layout::_instance = 0;

wzd_tvWall_layout::wzd_tvWall_layout(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::wzd_tvWall_layout)
{
    ui->setupUi(this);
}

wzd_tvWall_layout::~wzd_tvWall_layout()
{
    delete ui;
}


