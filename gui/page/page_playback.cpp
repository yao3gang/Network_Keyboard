#include "page_playback.h"
#include "ui_page_playback.h"

#include <QtDebug>

page_playback::page_playback(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_playback)
{
    ui->setupUi(this);
}

page_playback::~page_playback()
{
    delete ui;
}

void page_playback::showEvent( QShowEvent * event )
{
    qDebug() << "page_playback::showEvent" <<endl;
}
