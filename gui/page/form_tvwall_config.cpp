#include "form_tvwall_config.h"
#include "ui_form_tvwall_config.h"

form_tvwall_config::form_tvwall_config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_tvwall_config)
{
    ui->setupUi(this);
}

form_tvwall_config::~form_tvwall_config()
{
    delete ui;
}
