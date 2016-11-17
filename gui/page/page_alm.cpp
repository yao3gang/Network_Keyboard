#include "page_alm.h"
#include "ui_page_alm.h"
#include <QtDebug>

page_alm::page_alm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_alm)
{
    ui->setupUi(this);
    this->init_form();
}

page_alm::~page_alm()
{
    delete ui;
}

void page_alm::init_form() //init stackwidget
{
    QList<QPushButton *> btns = ui->widget_left->findChildren<QPushButton *>();
    foreach (QPushButton * b, btns)
    {
        connect(b, SIGNAL(clicked()), this, SLOT(button_clicked()));
    }

    ui->btn_info->click();
}

void page_alm::button_clicked()
{
    QPushButton *btn = (QPushButton *)sender();
    QList<QPushButton *> btns = ui->widget_left->findChildren<QPushButton *>();

    foreach (QPushButton * b, btns)
    {
        b->setChecked(false);
    }

    btn->setChecked(true);

    QString name = btn->objectName();
    //qDebug() << name;

    if (ui->btn_info->objectName() == name)
    {
        ui->stackedWidget_config->setCurrentIndex(0);
    }
    else if (ui->btn_link->objectName() == name)
    {
        ui->stackedWidget_config->setCurrentIndex(1);
    }

}

void page_alm::on_btn_clr_clicked()
{
    qDebug() << "on_btn_clr_clicked";
}
