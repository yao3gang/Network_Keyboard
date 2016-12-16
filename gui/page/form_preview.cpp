#include "form_preview.h"
#include "ui_form_preview.h"

form_preview::form_preview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_preview)
{
    ui->setupUi(this);
}

//初始化无边框窗体
void form_preview::init_style()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
}

form_preview::~form_preview()
{
    delete ui;
}
