#include "page_preview.h"
#include "ui_page_preview.h"

#include <QtDebug>

page_preview::page_preview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_preview)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    init_form();
}

page_preview::~page_preview()
{
    delete ui;
}

void page_preview::init_form()
{
    QPixmap btn_icon = QPixmap(":/image/ptz_direct.png");
    QMatrix m;
    //right
    //m.rotate(0);
    ui->btn_ptz_right->setIcon(QIcon(btn_icon.transformed(m)));
    ui->btn_ptz_right->setProperty("btn_ptz",true);
    //bottom right
    m.rotate(45);
    ui->btn_ptz_bottom_right->setIcon(QIcon(btn_icon.transformed(m)));
    ui->btn_ptz_bottom_right->setProperty("btn_ptz",true);
    //bottom
    m.rotate(45);
    ui->btn_ptz_bottom->setIcon(QIcon(btn_icon.transformed(m)));
    ui->btn_ptz_bottom->setProperty("btn_ptz",true);
    //bottom left
    m.rotate(45);
    ui->btn_ptz_bottom_left->setIcon(QIcon(btn_icon.transformed(m)));
    ui->btn_ptz_bottom_left->setProperty("btn_ptz",true);
    //left
    m.rotate(45);
    ui->btn_ptz_left->setIcon(QIcon(btn_icon.transformed(m)));
    ui->btn_ptz_left->setProperty("btn_ptz",true);
    //top left
    m.rotate(45);
    ui->btn_ptz_top_left->setIcon(QIcon(btn_icon.transformed(m)));
    ui->btn_ptz_top_left->setProperty("btn_ptz",true);
    //top
    m.rotate(45);
    ui->btn_ptz_top->setIcon(QIcon(btn_icon.transformed(m)));
    ui->btn_ptz_top->setProperty("btn_ptz",true);
    //top right
    m.rotate(45);
    ui->btn_ptz_top_right->setIcon(QIcon(btn_icon.transformed(m)));
    ui->btn_ptz_top_right->setProperty("btn_ptz",true);
}

void page_preview::hideEvent( QHideEvent * event )
{
    qDebug() << "page_preview::hideEvent" <<endl;
}
