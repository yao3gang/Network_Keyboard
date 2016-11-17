#include "dialogtest.h"
#include "ui_dialogtest.h"

Dialogtest::Dialogtest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogtest)
{
    ui->setupUi(this);
}

Dialogtest::~Dialogtest()
{
    delete ui;
}
