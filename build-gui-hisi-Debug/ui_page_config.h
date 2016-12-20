/********************************************************************************
** Form generated from reading UI file 'page_config.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE_CONFIG_H
#define UI_PAGE_CONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTimeEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page_config
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget_left;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_date;
    QPushButton *btn_network;
    QPushButton *btn_tv_wall;
    QPushButton *btn_upgrade;
    QPushButton *btn_sysinfo;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget_config;
    QWidget *page_date;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_date;
    QGridLayout *gridLayout_2;
    QLabel *lab_server;
    QPushButton *btn_sync;
    QLabel *lab_time_format;
    QComboBox *comboBox_time;
    QLineEdit *lineEdit_server;
    QTimeEdit *timeEdit;
    QLabel *lab_date;
    QSpacerItem *verticalSpacer_10;
    QDateEdit *dateEdit;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_date_cancel;
    QPushButton *btn_date_apply;
    QPushButton *btn_date_def;
    QLabel *lab_date_format;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *checkBox_auto_sync;
    QLabel *lab_time;
    QComboBox *comboBox_date;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_net;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *groupBox_net;
    QGridLayout *gridLayout_3;
    QLabel *lab_gateway;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btn_net_cancel;
    QPushButton *btn_net_apply;
    QPushButton *btn_net_def;
    QLabel *lab_dns1;
    QLabel *lab_dns2;
    QLabel *lab_submask;
    QLineEdit *lineEdit_mac;
    QLineEdit *lineEdit_ipaddr;
    QLineEdit *lineEdit_submask;
    QLabel *lab_time_format_2;
    QLabel *lab_ipaddr;
    QLineEdit *lineEdit_gateway;
    QLineEdit *lineEdit_dns1;
    QLineEdit *lineEdit_dns2;
    QSpacerItem *verticalSpacer_11;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_10;
    QCheckBox *checkBox_dhcp;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_mac_warning;
    QWidget *page_tvWall;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *btn_tvWall_add;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *btn_tvWall_modify;
    QSpacerItem *horizontalSpacer_15;
    QPushButton *btn_tvWall_del;
    QSpacerItem *horizontalSpacer_13;
    QTableWidget *tableWidget_tvwall;
    QWidget *page_upgrade;
    QGridLayout *gridLayout_6;
    QGroupBox *groupBox_upgrade;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *btn_upgrade_cancel;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btn_openfile;
    QLabel *label_file_name;
    QLabel *label_info;
    QProgressBar *pbar_upgrade;
    QLabel *label_file;
    QPushButton *btn_upgrade_apply;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_13;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_4;
    QWidget *page_sysInfo;
    QGridLayout *gridLayout_9;
    QGroupBox *groupBox_sysInfo;
    QGridLayout *gridLayout_8;
    QLabel *lab_sys_vision_val;
    QLabel *lab_dev_type_val;
    QLabel *lab_dev_type;
    QLabel *lab_sys_vision;
    QSpacerItem *verticalSpacer_9;
    QSpacerItem *horizontalSpacer_14;
    QSpacerItem *verticalSpacer_12;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *verticalSpacer_8;

    void setupUi(QWidget *page_config)
    {
        if (page_config->objectName().isEmpty())
            page_config->setObjectName(QString::fromUtf8("page_config"));
        page_config->resize(1024, 688);
        horizontalLayout = new QHBoxLayout(page_config);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_left = new QWidget(page_config);
        widget_left->setObjectName(QString::fromUtf8("widget_left"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_left->sizePolicy().hasHeightForWidth());
        widget_left->setSizePolicy(sizePolicy);
        widget_left->setMinimumSize(QSize(100, 0));
        widget_left->setMaximumSize(QSize(100, 16777215));
        verticalLayout = new QVBoxLayout(widget_left);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btn_date = new QPushButton(widget_left);
        btn_date->setObjectName(QString::fromUtf8("btn_date"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btn_date->sizePolicy().hasHeightForWidth());
        btn_date->setSizePolicy(sizePolicy1);
        btn_date->setMinimumSize(QSize(100, 50));
        btn_date->setCheckable(true);

        verticalLayout->addWidget(btn_date);

        btn_network = new QPushButton(widget_left);
        btn_network->setObjectName(QString::fromUtf8("btn_network"));
        sizePolicy1.setHeightForWidth(btn_network->sizePolicy().hasHeightForWidth());
        btn_network->setSizePolicy(sizePolicy1);
        btn_network->setMinimumSize(QSize(100, 50));
        btn_network->setCheckable(true);

        verticalLayout->addWidget(btn_network);

        btn_tv_wall = new QPushButton(widget_left);
        btn_tv_wall->setObjectName(QString::fromUtf8("btn_tv_wall"));
        sizePolicy1.setHeightForWidth(btn_tv_wall->sizePolicy().hasHeightForWidth());
        btn_tv_wall->setSizePolicy(sizePolicy1);
        btn_tv_wall->setMinimumSize(QSize(100, 50));
        btn_tv_wall->setCheckable(true);

        verticalLayout->addWidget(btn_tv_wall);

        btn_upgrade = new QPushButton(widget_left);
        btn_upgrade->setObjectName(QString::fromUtf8("btn_upgrade"));
        sizePolicy1.setHeightForWidth(btn_upgrade->sizePolicy().hasHeightForWidth());
        btn_upgrade->setSizePolicy(sizePolicy1);
        btn_upgrade->setMinimumSize(QSize(100, 50));
        btn_upgrade->setCheckable(true);

        verticalLayout->addWidget(btn_upgrade);

        btn_sysinfo = new QPushButton(widget_left);
        btn_sysinfo->setObjectName(QString::fromUtf8("btn_sysinfo"));
        sizePolicy1.setHeightForWidth(btn_sysinfo->sizePolicy().hasHeightForWidth());
        btn_sysinfo->setSizePolicy(sizePolicy1);
        btn_sysinfo->setMinimumSize(QSize(100, 50));
        btn_sysinfo->setCheckable(true);

        verticalLayout->addWidget(btn_sysinfo);

        verticalSpacer = new QSpacerItem(20, 435, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget_left);

        stackedWidget_config = new QStackedWidget(page_config);
        stackedWidget_config->setObjectName(QString::fromUtf8("stackedWidget_config"));
        stackedWidget_config->setMinimumSize(QSize(924, 0));
        stackedWidget_config->setMaximumSize(QSize(924, 16777215));
        page_date = new QWidget();
        page_date->setObjectName(QString::fromUtf8("page_date"));
        gridLayout = new QGridLayout(page_date);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_date = new QGroupBox(page_date);
        groupBox_date->setObjectName(QString::fromUtf8("groupBox_date"));
        sizePolicy1.setHeightForWidth(groupBox_date->sizePolicy().hasHeightForWidth());
        groupBox_date->setSizePolicy(sizePolicy1);
        groupBox_date->setMinimumSize(QSize(400, 300));
        gridLayout_2 = new QGridLayout(groupBox_date);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        lab_server = new QLabel(groupBox_date);
        lab_server->setObjectName(QString::fromUtf8("lab_server"));

        gridLayout_2->addWidget(lab_server, 3, 0, 1, 1);

        btn_sync = new QPushButton(groupBox_date);
        btn_sync->setObjectName(QString::fromUtf8("btn_sync"));
        sizePolicy1.setHeightForWidth(btn_sync->sizePolicy().hasHeightForWidth());
        btn_sync->setSizePolicy(sizePolicy1);
        btn_sync->setMinimumSize(QSize(75, 0));

        gridLayout_2->addWidget(btn_sync, 5, 2, 1, 1);

        lab_time_format = new QLabel(groupBox_date);
        lab_time_format->setObjectName(QString::fromUtf8("lab_time_format"));

        gridLayout_2->addWidget(lab_time_format, 2, 0, 1, 1);

        comboBox_time = new QComboBox(groupBox_date);
        comboBox_time->setObjectName(QString::fromUtf8("comboBox_time"));

        gridLayout_2->addWidget(comboBox_time, 2, 1, 1, 1);

        lineEdit_server = new QLineEdit(groupBox_date);
        lineEdit_server->setObjectName(QString::fromUtf8("lineEdit_server"));

        gridLayout_2->addWidget(lineEdit_server, 3, 1, 1, 2);

        timeEdit = new QTimeEdit(groupBox_date);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setMinimumDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0)));

        gridLayout_2->addWidget(timeEdit, 7, 1, 1, 1);

        lab_date = new QLabel(groupBox_date);
        lab_date->setObjectName(QString::fromUtf8("lab_date"));

        gridLayout_2->addWidget(lab_date, 6, 0, 1, 1);

        verticalSpacer_10 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_10, 0, 0, 1, 1);

        dateEdit = new QDateEdit(groupBox_date);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));

        gridLayout_2->addWidget(dateEdit, 6, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btn_date_cancel = new QPushButton(groupBox_date);
        btn_date_cancel->setObjectName(QString::fromUtf8("btn_date_cancel"));
        sizePolicy1.setHeightForWidth(btn_date_cancel->sizePolicy().hasHeightForWidth());
        btn_date_cancel->setSizePolicy(sizePolicy1);
        btn_date_cancel->setMinimumSize(QSize(75, 0));

        horizontalLayout_2->addWidget(btn_date_cancel);

        btn_date_apply = new QPushButton(groupBox_date);
        btn_date_apply->setObjectName(QString::fromUtf8("btn_date_apply"));
        sizePolicy1.setHeightForWidth(btn_date_apply->sizePolicy().hasHeightForWidth());
        btn_date_apply->setSizePolicy(sizePolicy1);
        btn_date_apply->setMinimumSize(QSize(75, 0));

        horizontalLayout_2->addWidget(btn_date_apply);


        gridLayout_2->addLayout(horizontalLayout_2, 8, 1, 1, 2);

        btn_date_def = new QPushButton(groupBox_date);
        btn_date_def->setObjectName(QString::fromUtf8("btn_date_def"));
        sizePolicy1.setHeightForWidth(btn_date_def->sizePolicy().hasHeightForWidth());
        btn_date_def->setSizePolicy(sizePolicy1);
        btn_date_def->setMinimumSize(QSize(75, 0));

        gridLayout_2->addWidget(btn_date_def, 8, 0, 1, 1);

        lab_date_format = new QLabel(groupBox_date);
        lab_date_format->setObjectName(QString::fromUtf8("lab_date_format"));

        gridLayout_2->addWidget(lab_date_format, 1, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 2, 2, 1, 1);

        checkBox_auto_sync = new QCheckBox(groupBox_date);
        checkBox_auto_sync->setObjectName(QString::fromUtf8("checkBox_auto_sync"));
        checkBox_auto_sync->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(checkBox_auto_sync->sizePolicy().hasHeightForWidth());
        checkBox_auto_sync->setSizePolicy(sizePolicy2);
        checkBox_auto_sync->setChecked(false);

        gridLayout_2->addWidget(checkBox_auto_sync, 5, 0, 1, 1);

        lab_time = new QLabel(groupBox_date);
        lab_time->setObjectName(QString::fromUtf8("lab_time"));

        gridLayout_2->addWidget(lab_time, 7, 0, 1, 1);

        comboBox_date = new QComboBox(groupBox_date);
        comboBox_date->setObjectName(QString::fromUtf8("comboBox_date"));

        gridLayout_2->addWidget(comboBox_date, 1, 1, 1, 1);

        gridLayout_2->setColumnStretch(0, 1);
        gridLayout_2->setColumnStretch(1, 1);
        gridLayout_2->setColumnStretch(2, 1);

        gridLayout->addWidget(groupBox_date, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 1, 0, 1, 1);

        stackedWidget_config->addWidget(page_date);
        page_net = new QWidget();
        page_net->setObjectName(QString::fromUtf8("page_net"));
        gridLayout_4 = new QGridLayout(page_net);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_6 = new QSpacerItem(497, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_6, 0, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 361, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 1, 0, 1, 1);

        groupBox_net = new QGroupBox(page_net);
        groupBox_net->setObjectName(QString::fromUtf8("groupBox_net"));
        sizePolicy2.setHeightForWidth(groupBox_net->sizePolicy().hasHeightForWidth());
        groupBox_net->setSizePolicy(sizePolicy2);
        groupBox_net->setMinimumSize(QSize(400, 300));
        gridLayout_3 = new QGridLayout(groupBox_net);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        lab_gateway = new QLabel(groupBox_net);
        lab_gateway->setObjectName(QString::fromUtf8("lab_gateway"));

        gridLayout_3->addWidget(lab_gateway, 4, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        btn_net_cancel = new QPushButton(groupBox_net);
        btn_net_cancel->setObjectName(QString::fromUtf8("btn_net_cancel"));
        sizePolicy1.setHeightForWidth(btn_net_cancel->sizePolicy().hasHeightForWidth());
        btn_net_cancel->setSizePolicy(sizePolicy1);
        btn_net_cancel->setMinimumSize(QSize(75, 0));

        horizontalLayout_3->addWidget(btn_net_cancel);

        btn_net_apply = new QPushButton(groupBox_net);
        btn_net_apply->setObjectName(QString::fromUtf8("btn_net_apply"));
        sizePolicy1.setHeightForWidth(btn_net_apply->sizePolicy().hasHeightForWidth());
        btn_net_apply->setSizePolicy(sizePolicy1);
        btn_net_apply->setMinimumSize(QSize(75, 0));

        horizontalLayout_3->addWidget(btn_net_apply);


        gridLayout_3->addLayout(horizontalLayout_3, 9, 1, 1, 2);

        btn_net_def = new QPushButton(groupBox_net);
        btn_net_def->setObjectName(QString::fromUtf8("btn_net_def"));
        sizePolicy1.setHeightForWidth(btn_net_def->sizePolicy().hasHeightForWidth());
        btn_net_def->setSizePolicy(sizePolicy1);
        btn_net_def->setMinimumSize(QSize(75, 0));

        gridLayout_3->addWidget(btn_net_def, 9, 0, 1, 1);

        lab_dns1 = new QLabel(groupBox_net);
        lab_dns1->setObjectName(QString::fromUtf8("lab_dns1"));

        gridLayout_3->addWidget(lab_dns1, 7, 0, 1, 1);

        lab_dns2 = new QLabel(groupBox_net);
        lab_dns2->setObjectName(QString::fromUtf8("lab_dns2"));
        lab_dns2->setLineWidth(1);

        gridLayout_3->addWidget(lab_dns2, 8, 0, 1, 1);

        lab_submask = new QLabel(groupBox_net);
        lab_submask->setObjectName(QString::fromUtf8("lab_submask"));

        gridLayout_3->addWidget(lab_submask, 3, 0, 1, 1);

        lineEdit_mac = new QLineEdit(groupBox_net);
        lineEdit_mac->setObjectName(QString::fromUtf8("lineEdit_mac"));

        gridLayout_3->addWidget(lineEdit_mac, 1, 1, 1, 1);

        lineEdit_ipaddr = new QLineEdit(groupBox_net);
        lineEdit_ipaddr->setObjectName(QString::fromUtf8("lineEdit_ipaddr"));

        gridLayout_3->addWidget(lineEdit_ipaddr, 2, 1, 1, 1);

        lineEdit_submask = new QLineEdit(groupBox_net);
        lineEdit_submask->setObjectName(QString::fromUtf8("lineEdit_submask"));

        gridLayout_3->addWidget(lineEdit_submask, 3, 1, 1, 1);

        lab_time_format_2 = new QLabel(groupBox_net);
        lab_time_format_2->setObjectName(QString::fromUtf8("lab_time_format_2"));

        gridLayout_3->addWidget(lab_time_format_2, 1, 0, 1, 1);

        lab_ipaddr = new QLabel(groupBox_net);
        lab_ipaddr->setObjectName(QString::fromUtf8("lab_ipaddr"));

        gridLayout_3->addWidget(lab_ipaddr, 2, 0, 1, 1);

        lineEdit_gateway = new QLineEdit(groupBox_net);
        lineEdit_gateway->setObjectName(QString::fromUtf8("lineEdit_gateway"));

        gridLayout_3->addWidget(lineEdit_gateway, 4, 1, 1, 1);

        lineEdit_dns1 = new QLineEdit(groupBox_net);
        lineEdit_dns1->setObjectName(QString::fromUtf8("lineEdit_dns1"));

        gridLayout_3->addWidget(lineEdit_dns1, 7, 1, 1, 1);

        lineEdit_dns2 = new QLineEdit(groupBox_net);
        lineEdit_dns2->setObjectName(QString::fromUtf8("lineEdit_dns2"));

        gridLayout_3->addWidget(lineEdit_dns2, 8, 1, 1, 1);

        verticalSpacer_11 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_11, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_10 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        checkBox_dhcp = new QCheckBox(groupBox_net);
        checkBox_dhcp->setObjectName(QString::fromUtf8("checkBox_dhcp"));
        sizePolicy2.setHeightForWidth(checkBox_dhcp->sizePolicy().hasHeightForWidth());
        checkBox_dhcp->setSizePolicy(sizePolicy2);
        checkBox_dhcp->setChecked(false);

        horizontalLayout_4->addWidget(checkBox_dhcp);


        gridLayout_3->addLayout(horizontalLayout_4, 2, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_4, 3, 2, 1, 1);

        label_mac_warning = new QLabel(groupBox_net);
        label_mac_warning->setObjectName(QString::fromUtf8("label_mac_warning"));

        gridLayout_3->addWidget(label_mac_warning, 1, 2, 1, 1);

        gridLayout_3->setColumnStretch(0, 1);
        gridLayout_3->setColumnStretch(1, 2);
        gridLayout_3->setColumnStretch(2, 1);

        gridLayout_4->addWidget(groupBox_net, 0, 0, 1, 1);

        stackedWidget_config->addWidget(page_net);
        page_tvWall = new QWidget();
        page_tvWall->setObjectName(QString::fromUtf8("page_tvWall"));
        verticalLayout_2 = new QVBoxLayout(page_tvWall);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_11 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_11);

        btn_tvWall_add = new QPushButton(page_tvWall);
        btn_tvWall_add->setObjectName(QString::fromUtf8("btn_tvWall_add"));
        btn_tvWall_add->setEnabled(true);
        sizePolicy1.setHeightForWidth(btn_tvWall_add->sizePolicy().hasHeightForWidth());
        btn_tvWall_add->setSizePolicy(sizePolicy1);
        btn_tvWall_add->setMinimumSize(QSize(100, 0));

        horizontalLayout_8->addWidget(btn_tvWall_add);

        horizontalSpacer_12 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_12);

        btn_tvWall_modify = new QPushButton(page_tvWall);
        btn_tvWall_modify->setObjectName(QString::fromUtf8("btn_tvWall_modify"));
        sizePolicy1.setHeightForWidth(btn_tvWall_modify->sizePolicy().hasHeightForWidth());
        btn_tvWall_modify->setSizePolicy(sizePolicy1);
        btn_tvWall_modify->setMinimumSize(QSize(100, 0));

        horizontalLayout_8->addWidget(btn_tvWall_modify);

        horizontalSpacer_15 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_15);

        btn_tvWall_del = new QPushButton(page_tvWall);
        btn_tvWall_del->setObjectName(QString::fromUtf8("btn_tvWall_del"));
        sizePolicy1.setHeightForWidth(btn_tvWall_del->sizePolicy().hasHeightForWidth());
        btn_tvWall_del->setSizePolicy(sizePolicy1);
        btn_tvWall_del->setMinimumSize(QSize(100, 0));

        horizontalLayout_8->addWidget(btn_tvWall_del);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_13);


        verticalLayout_2->addLayout(horizontalLayout_8);

        tableWidget_tvwall = new QTableWidget(page_tvWall);
        tableWidget_tvwall->setObjectName(QString::fromUtf8("tableWidget_tvwall"));
        tableWidget_tvwall->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget_tvwall->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_tvwall->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(tableWidget_tvwall);

        stackedWidget_config->addWidget(page_tvWall);
        page_upgrade = new QWidget();
        page_upgrade->setObjectName(QString::fromUtf8("page_upgrade"));
        gridLayout_6 = new QGridLayout(page_upgrade);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        groupBox_upgrade = new QGroupBox(page_upgrade);
        groupBox_upgrade->setObjectName(QString::fromUtf8("groupBox_upgrade"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox_upgrade->sizePolicy().hasHeightForWidth());
        groupBox_upgrade->setSizePolicy(sizePolicy3);
        groupBox_upgrade->setMinimumSize(QSize(600, 200));
        gridLayout_5 = new QGridLayout(groupBox_upgrade);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        btn_upgrade_cancel = new QPushButton(groupBox_upgrade);
        btn_upgrade_cancel->setObjectName(QString::fromUtf8("btn_upgrade_cancel"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(btn_upgrade_cancel->sizePolicy().hasHeightForWidth());
        btn_upgrade_cancel->setSizePolicy(sizePolicy4);
        btn_upgrade_cancel->setMinimumSize(QSize(75, 0));
        btn_upgrade_cancel->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_5->addWidget(btn_upgrade_cancel);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);


        gridLayout_5->addLayout(horizontalLayout_5, 6, 1, 1, 1);

        btn_openfile = new QPushButton(groupBox_upgrade);
        btn_openfile->setObjectName(QString::fromUtf8("btn_openfile"));
        sizePolicy4.setHeightForWidth(btn_openfile->sizePolicy().hasHeightForWidth());
        btn_openfile->setSizePolicy(sizePolicy4);
        btn_openfile->setMinimumSize(QSize(75, 0));

        gridLayout_5->addWidget(btn_openfile, 3, 2, 1, 1);

        label_file_name = new QLabel(groupBox_upgrade);
        label_file_name->setObjectName(QString::fromUtf8("label_file_name"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_file_name->sizePolicy().hasHeightForWidth());
        label_file_name->setSizePolicy(sizePolicy5);
        label_file_name->setFrameShape(QFrame::Panel);
        label_file_name->setFrameShadow(QFrame::Sunken);

        gridLayout_5->addWidget(label_file_name, 3, 1, 1, 1);

        label_info = new QLabel(groupBox_upgrade);
        label_info->setObjectName(QString::fromUtf8("label_info"));
        label_info->setWordWrap(true);

        gridLayout_5->addWidget(label_info, 1, 0, 1, 3);

        pbar_upgrade = new QProgressBar(groupBox_upgrade);
        pbar_upgrade->setObjectName(QString::fromUtf8("pbar_upgrade"));
        pbar_upgrade->setMinimumSize(QSize(0, 20));
        pbar_upgrade->setMaximumSize(QSize(16777215, 20));
        pbar_upgrade->setMaximum(100);
        pbar_upgrade->setValue(0);
        pbar_upgrade->setOrientation(Qt::Horizontal);
        pbar_upgrade->setInvertedAppearance(false);

        gridLayout_5->addWidget(pbar_upgrade, 4, 0, 1, 3);

        label_file = new QLabel(groupBox_upgrade);
        label_file->setObjectName(QString::fromUtf8("label_file"));
        sizePolicy.setHeightForWidth(label_file->sizePolicy().hasHeightForWidth());
        label_file->setSizePolicy(sizePolicy);
        label_file->setMinimumSize(QSize(60, 0));

        gridLayout_5->addWidget(label_file, 3, 0, 1, 1);

        btn_upgrade_apply = new QPushButton(groupBox_upgrade);
        btn_upgrade_apply->setObjectName(QString::fromUtf8("btn_upgrade_apply"));
        sizePolicy4.setHeightForWidth(btn_upgrade_apply->sizePolicy().hasHeightForWidth());
        btn_upgrade_apply->setSizePolicy(sizePolicy4);
        btn_upgrade_apply->setMinimumSize(QSize(75, 0));
        btn_upgrade_apply->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_5->addWidget(btn_upgrade_apply, 6, 2, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_6, 2, 0, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout_5->addItem(verticalSpacer_7, 5, 0, 1, 1);

        verticalSpacer_13 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_5->addItem(verticalSpacer_13, 0, 0, 1, 1);


        gridLayout_6->addWidget(groupBox_upgrade, 0, 0, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(297, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_9, 0, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 361, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_4, 1, 0, 1, 1);

        stackedWidget_config->addWidget(page_upgrade);
        page_sysInfo = new QWidget();
        page_sysInfo->setObjectName(QString::fromUtf8("page_sysInfo"));
        gridLayout_9 = new QGridLayout(page_sysInfo);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        groupBox_sysInfo = new QGroupBox(page_sysInfo);
        groupBox_sysInfo->setObjectName(QString::fromUtf8("groupBox_sysInfo"));
        sizePolicy1.setHeightForWidth(groupBox_sysInfo->sizePolicy().hasHeightForWidth());
        groupBox_sysInfo->setSizePolicy(sizePolicy1);
        groupBox_sysInfo->setMinimumSize(QSize(400, 300));
        gridLayout_8 = new QGridLayout(groupBox_sysInfo);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        lab_sys_vision_val = new QLabel(groupBox_sysInfo);
        lab_sys_vision_val->setObjectName(QString::fromUtf8("lab_sys_vision_val"));

        gridLayout_8->addWidget(lab_sys_vision_val, 3, 2, 1, 1);

        lab_dev_type_val = new QLabel(groupBox_sysInfo);
        lab_dev_type_val->setObjectName(QString::fromUtf8("lab_dev_type_val"));
        sizePolicy5.setHeightForWidth(lab_dev_type_val->sizePolicy().hasHeightForWidth());
        lab_dev_type_val->setSizePolicy(sizePolicy5);

        gridLayout_8->addWidget(lab_dev_type_val, 1, 2, 1, 1);

        lab_dev_type = new QLabel(groupBox_sysInfo);
        lab_dev_type->setObjectName(QString::fromUtf8("lab_dev_type"));
        lab_dev_type->setEnabled(true);
        sizePolicy.setHeightForWidth(lab_dev_type->sizePolicy().hasHeightForWidth());
        lab_dev_type->setSizePolicy(sizePolicy);

        gridLayout_8->addWidget(lab_dev_type, 1, 0, 1, 1);

        lab_sys_vision = new QLabel(groupBox_sysInfo);
        lab_sys_vision->setObjectName(QString::fromUtf8("lab_sys_vision"));

        gridLayout_8->addWidget(lab_sys_vision, 3, 0, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_9, 4, 0, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_14, 1, 1, 1, 1);

        verticalSpacer_12 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_12, 0, 0, 1, 1);

        gridLayout_8->setColumnStretch(0, 1);
        gridLayout_8->setColumnStretch(1, 1);

        gridLayout_9->addWidget(groupBox_sysInfo, 0, 0, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(497, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_16, 0, 1, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 361, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_8, 1, 0, 1, 1);

        stackedWidget_config->addWidget(page_sysInfo);

        horizontalLayout->addWidget(stackedWidget_config);

        QWidget::setTabOrder(btn_date, btn_network);
        QWidget::setTabOrder(btn_network, btn_tv_wall);
        QWidget::setTabOrder(btn_tv_wall, btn_upgrade);
        QWidget::setTabOrder(btn_upgrade, btn_sysinfo);
        QWidget::setTabOrder(btn_sysinfo, comboBox_date);
        QWidget::setTabOrder(comboBox_date, comboBox_time);
        QWidget::setTabOrder(comboBox_time, lineEdit_server);
        QWidget::setTabOrder(lineEdit_server, checkBox_auto_sync);
        QWidget::setTabOrder(checkBox_auto_sync, btn_sync);
        QWidget::setTabOrder(btn_sync, dateEdit);
        QWidget::setTabOrder(dateEdit, timeEdit);
        QWidget::setTabOrder(timeEdit, btn_date_def);
        QWidget::setTabOrder(btn_date_def, btn_date_cancel);
        QWidget::setTabOrder(btn_date_cancel, btn_date_apply);
        QWidget::setTabOrder(btn_date_apply, lineEdit_mac);
        QWidget::setTabOrder(lineEdit_mac, lineEdit_ipaddr);
        QWidget::setTabOrder(lineEdit_ipaddr, checkBox_dhcp);
        QWidget::setTabOrder(checkBox_dhcp, lineEdit_submask);
        QWidget::setTabOrder(lineEdit_submask, lineEdit_gateway);
        QWidget::setTabOrder(lineEdit_gateway, lineEdit_dns1);
        QWidget::setTabOrder(lineEdit_dns1, lineEdit_dns2);
        QWidget::setTabOrder(lineEdit_dns2, btn_net_def);
        QWidget::setTabOrder(btn_net_def, btn_net_cancel);
        QWidget::setTabOrder(btn_net_cancel, btn_net_apply);
        QWidget::setTabOrder(btn_net_apply, btn_tvWall_add);
        QWidget::setTabOrder(btn_tvWall_add, btn_tvWall_modify);
        QWidget::setTabOrder(btn_tvWall_modify, tableWidget_tvwall);
        QWidget::setTabOrder(tableWidget_tvwall, btn_openfile);
        QWidget::setTabOrder(btn_openfile, btn_upgrade_cancel);
        QWidget::setTabOrder(btn_upgrade_cancel, btn_upgrade_apply);

        retranslateUi(page_config);

        stackedWidget_config->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(page_config);
    } // setupUi

    void retranslateUi(QWidget *page_config)
    {
        page_config->setWindowTitle(QApplication::translate("page_config", "Form", 0, QApplication::UnicodeUTF8));
        btn_date->setText(QApplication::translate("page_config", "\346\227\245\346\234\237\345\222\214\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        btn_network->setText(QApplication::translate("page_config", "\347\275\221\347\273\234", 0, QApplication::UnicodeUTF8));
        btn_tv_wall->setText(QApplication::translate("page_config", "\347\224\265\350\247\206\345\242\231\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        btn_upgrade->setText(QApplication::translate("page_config", "\345\215\207\347\272\247", 0, QApplication::UnicodeUTF8));
        btn_sysinfo->setText(QApplication::translate("page_config", "\347\263\273\347\273\237\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        groupBox_date->setTitle(QApplication::translate("page_config", "\346\227\245\346\234\237\345\222\214\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        lab_server->setText(QApplication::translate("page_config", "\347\275\221\347\273\234\346\227\266\351\227\264\346\234\215\345\212\241\345\231\250", 0, QApplication::UnicodeUTF8));
        btn_sync->setText(QApplication::translate("page_config", "\347\253\213\345\215\263\345\220\214\346\255\245", 0, QApplication::UnicodeUTF8));
        lab_time_format->setText(QApplication::translate("page_config", "\346\227\266\351\227\264\346\240\274\345\274\217", 0, QApplication::UnicodeUTF8));
        timeEdit->setDisplayFormat(QApplication::translate("page_config", "HH:mm:ss", 0, QApplication::UnicodeUTF8));
        lab_date->setText(QApplication::translate("page_config", "\347\263\273\347\273\237\346\227\245\346\234\237", 0, QApplication::UnicodeUTF8));
        dateEdit->setDisplayFormat(QApplication::translate("page_config", "yyyy-MM-dd", 0, QApplication::UnicodeUTF8));
        btn_date_cancel->setText(QApplication::translate("page_config", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        btn_date_apply->setText(QApplication::translate("page_config", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        btn_date_def->setText(QApplication::translate("page_config", "\351\273\230\350\256\244\345\200\274", 0, QApplication::UnicodeUTF8));
        lab_date_format->setText(QApplication::translate("page_config", "\346\227\245\346\234\237\346\240\274\345\274\217", 0, QApplication::UnicodeUTF8));
        checkBox_auto_sync->setText(QApplication::translate("page_config", "\350\207\252\345\212\250\345\220\214\346\255\245", 0, QApplication::UnicodeUTF8));
        lab_time->setText(QApplication::translate("page_config", "\347\263\273\347\273\237\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        groupBox_net->setTitle(QApplication::translate("page_config", "\347\275\221\347\273\234\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        lab_gateway->setText(QApplication::translate("page_config", "\351\273\230\350\256\244\347\275\221\345\205\263", 0, QApplication::UnicodeUTF8));
        btn_net_cancel->setText(QApplication::translate("page_config", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        btn_net_apply->setText(QApplication::translate("page_config", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        btn_net_def->setText(QApplication::translate("page_config", "\351\273\230\350\256\244\345\200\274", 0, QApplication::UnicodeUTF8));
        lab_dns1->setText(QApplication::translate("page_config", "\351\246\226\351\200\211DNS\346\234\215\345\212\241\345\231\250", 0, QApplication::UnicodeUTF8));
        lab_dns2->setText(QApplication::translate("page_config", "\345\244\207\351\200\211DNS\346\234\215\345\212\241\345\231\250", 0, QApplication::UnicodeUTF8));
        lab_submask->setText(QApplication::translate("page_config", "\345\255\220\347\275\221\346\216\251\347\240\201", 0, QApplication::UnicodeUTF8));
        lineEdit_mac->setInputMask(QApplication::translate("page_config", "HH:HH:HH:HH:HH:HH;_", 0, QApplication::UnicodeUTF8));
        lab_time_format_2->setText(QApplication::translate("page_config", "MAC\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        lab_ipaddr->setText(QApplication::translate("page_config", "IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        checkBox_dhcp->setText(QApplication::translate("page_config", "DHCP", 0, QApplication::UnicodeUTF8));
        label_mac_warning->setText(QApplication::translate("page_config", "label_mac_warning", 0, QApplication::UnicodeUTF8));
        btn_tvWall_add->setText(QApplication::translate("page_config", "\346\267\273\345\212\240\347\224\265\350\247\206\345\242\231", 0, QApplication::UnicodeUTF8));
        btn_tvWall_modify->setText(QApplication::translate("page_config", "\344\277\256\346\224\271\347\224\265\350\247\206\345\242\231", 0, QApplication::UnicodeUTF8));
        btn_tvWall_del->setText(QApplication::translate("page_config", "\345\210\240\351\231\244\347\224\265\350\247\206\345\242\231", 0, QApplication::UnicodeUTF8));
        groupBox_upgrade->setTitle(QApplication::translate("page_config", "\345\215\207\347\272\247", 0, QApplication::UnicodeUTF8));
        btn_upgrade_cancel->setText(QApplication::translate("page_config", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        btn_openfile->setText(QApplication::translate("page_config", "\346\265\217\350\247\210", 0, QApplication::UnicodeUTF8));
        label_file_name->setText(QString());
        label_info->setText(QApplication::translate("page_config", "<html><head/><body><p>\350\257\267\346\217\222\345\205\245U\347\233\230\357\274\214\351\200\211\346\213\251\345\205\266\344\270\255\345\257\271\345\272\224\347\232\204\345\215\207\347\272\247\346\226\207\344\273\266\357\274\233\345\215\207\347\272\247\350\277\207\347\250\213\344\270\255\350\257\267\344\270\215\350\246\201\345\205\263\351\227\255\347\224\265\346\272\220\357\274\201</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_file->setText(QApplication::translate("page_config", "\345\215\207\347\272\247\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        btn_upgrade_apply->setText(QApplication::translate("page_config", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        groupBox_sysInfo->setTitle(QApplication::translate("page_config", "\347\263\273\347\273\237\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        lab_dev_type->setText(QApplication::translate("page_config", "\350\256\276\345\244\207\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        lab_sys_vision->setText(QApplication::translate("page_config", "\347\263\273\347\273\237\347\211\210\346\234\254", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class page_config: public Ui_page_config {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_CONFIG_H
