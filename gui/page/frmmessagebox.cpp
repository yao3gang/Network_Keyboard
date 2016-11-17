#include "frmmessagebox.h"
#include "ui_frmmessagebox.h"
#include "iconhelper.h"
#include "appdef.h"
#include <QDesktopWidget>

frmMessageBox::frmMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMessageBox)
{
    ui->setupUi(this);
    this->InitStyle();
    //myHelper::FormInCenter(this);
    int frmX = width();
    int frmY = height();
    QDesktopWidget w;
    int deskWidth = w.availableGeometry().width();
    int deskHeight = w.availableGeometry().height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    move(movePoint);
}

frmMessageBox::~frmMessageBox()
{
    delete ui;
}

void frmMessageBox::InitStyle()
{
    this->setProperty("Form", true);
    this->setProperty("CanMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    IconHelper::Instance()->SetIcoMain(ui->lab_Ico, FontSize + 2);
    IconHelper::Instance()->SetIcoClose(ui->btnMenu_Close);
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));


    this->setWindowTitle(ui->lab_Title->text());
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void frmMessageBox::SetMessage(QString msg, int type)
{
    if (type == 0) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_info.png);");
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText("提示");
    } else if (type == 1) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_question.png);");
        ui->lab_Title->setText("询问");
    } else if (type == 2) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_error.png);");
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText("错误");
    }

    ui->labInfo->setText(msg);
    this->setWindowTitle(ui->lab_Title->text());
}

void frmMessageBox::on_btnOk_clicked()
{
    done(1);
    this->close();
}

//extern call function
//显示信息框,仅确定按钮
void ShowMessageBoxInfo(QString info) {
    frmMessageBox msg;
    msg.SetMessage(info, 0);
    msg.exec();
}

//显示错误框,仅确定按钮
void ShowMessageBoxError(QString info) {
    frmMessageBox msg;
    msg.SetMessage(info, 2);
    msg.exec();
}

//显示询问框,确定和取消按钮
int ShowMessageBoxQuesion(QString info) {
    frmMessageBox msg;
    msg.SetMessage(info, 1);
    return msg.exec();
}
