/********************************************************************************
** Form generated from reading UI file 'page_tvwall.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE_TVWALL_H
#define UI_PAGE_TVWALL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "tablewidget_tvwall.h"
#include "treewidget_dev.h"

QT_BEGIN_NAMESPACE

class Ui_page_tvWall
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    treewidget_dev *treeWidget_screen;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QGroupBox *gbox_patrol;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QComboBox *cmb_patrol_interval;
    QLabel *label_2;
    QComboBox *cmb_patrol_windows;
    QCheckBox *ckb_patrol_enable;
    QPushButton *btn_patrol_apply;
    QPushButton *btn_patrol_sync;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    tablewidget_tvwall *tableWidget_tvwall;
    treewidget_dev *treeWidget_nvr;

    void setupUi(QWidget *page_tvWall)
    {
        if (page_tvWall->objectName().isEmpty())
            page_tvWall->setObjectName(QString::fromUtf8("page_tvWall"));
        page_tvWall->resize(1024, 688);
        page_tvWall->setMinimumSize(QSize(0, 0));
        page_tvWall->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_4 = new QVBoxLayout(page_tvWall);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        treeWidget_screen = new treewidget_dev(page_tvWall);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget_screen->setHeaderItem(__qtreewidgetitem);
        treeWidget_screen->setObjectName(QString::fromUtf8("treeWidget_screen"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeWidget_screen->sizePolicy().hasHeightForWidth());
        treeWidget_screen->setSizePolicy(sizePolicy);
        treeWidget_screen->setMinimumSize(QSize(220, 0));
        treeWidget_screen->setMaximumSize(QSize(220, 16777215));

        horizontalLayout_2->addWidget(treeWidget_screen);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gbox_patrol = new QGroupBox(page_tvWall);
        gbox_patrol->setObjectName(QString::fromUtf8("gbox_patrol"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gbox_patrol->sizePolicy().hasHeightForWidth());
        gbox_patrol->setSizePolicy(sizePolicy1);
        gbox_patrol->setMinimumSize(QSize(0, 80));
        gbox_patrol->setMaximumSize(QSize(16777215, 80));
        horizontalLayout_4 = new QHBoxLayout(gbox_patrol);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 15, -1, -1);
        label = new QLabel(gbox_patrol);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        cmb_patrol_interval = new QComboBox(gbox_patrol);
        cmb_patrol_interval->setObjectName(QString::fromUtf8("cmb_patrol_interval"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cmb_patrol_interval->sizePolicy().hasHeightForWidth());
        cmb_patrol_interval->setSizePolicy(sizePolicy2);
        cmb_patrol_interval->setMinimumSize(QSize(60, 0));
        cmb_patrol_interval->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_4->addWidget(cmb_patrol_interval);

        label_2 = new QLabel(gbox_patrol);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        cmb_patrol_windows = new QComboBox(gbox_patrol);
        cmb_patrol_windows->setObjectName(QString::fromUtf8("cmb_patrol_windows"));
        sizePolicy2.setHeightForWidth(cmb_patrol_windows->sizePolicy().hasHeightForWidth());
        cmb_patrol_windows->setSizePolicy(sizePolicy2);
        cmb_patrol_windows->setMinimumSize(QSize(100, 0));
        cmb_patrol_windows->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_4->addWidget(cmb_patrol_windows);

        ckb_patrol_enable = new QCheckBox(gbox_patrol);
        ckb_patrol_enable->setObjectName(QString::fromUtf8("ckb_patrol_enable"));

        horizontalLayout_4->addWidget(ckb_patrol_enable);

        btn_patrol_apply = new QPushButton(gbox_patrol);
        btn_patrol_apply->setObjectName(QString::fromUtf8("btn_patrol_apply"));
        btn_patrol_apply->setEnabled(true);
        sizePolicy2.setHeightForWidth(btn_patrol_apply->sizePolicy().hasHeightForWidth());
        btn_patrol_apply->setSizePolicy(sizePolicy2);
        btn_patrol_apply->setMinimumSize(QSize(75, 0));
        btn_patrol_apply->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_4->addWidget(btn_patrol_apply);

        btn_patrol_sync = new QPushButton(gbox_patrol);
        btn_patrol_sync->setObjectName(QString::fromUtf8("btn_patrol_sync"));

        horizontalLayout_4->addWidget(btn_patrol_sync);


        horizontalLayout->addWidget(gbox_patrol);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout);

        groupBox = new QGroupBox(page_tvWall);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tableWidget_tvwall = new tablewidget_tvwall(groupBox);
        tableWidget_tvwall->setObjectName(QString::fromUtf8("tableWidget_tvwall"));
        tableWidget_tvwall->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_3->addWidget(tableWidget_tvwall);

        treeWidget_nvr = new treewidget_dev(groupBox);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        treeWidget_nvr->setHeaderItem(__qtreewidgetitem1);
        treeWidget_nvr->setObjectName(QString::fromUtf8("treeWidget_nvr"));
        sizePolicy.setHeightForWidth(treeWidget_nvr->sizePolicy().hasHeightForWidth());
        treeWidget_nvr->setSizePolicy(sizePolicy);
        treeWidget_nvr->setMinimumSize(QSize(250, 0));
        treeWidget_nvr->setMaximumSize(QSize(250, 16777215));

        horizontalLayout_3->addWidget(treeWidget_nvr);


        verticalLayout_3->addWidget(groupBox);


        horizontalLayout_2->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout_2);

        QWidget::setTabOrder(treeWidget_screen, tableWidget_tvwall);
        QWidget::setTabOrder(tableWidget_tvwall, treeWidget_nvr);
        QWidget::setTabOrder(treeWidget_nvr, ckb_patrol_enable);
        QWidget::setTabOrder(ckb_patrol_enable, btn_patrol_apply);

        retranslateUi(page_tvWall);

        QMetaObject::connectSlotsByName(page_tvWall);
    } // setupUi

    void retranslateUi(QWidget *page_tvWall)
    {
        page_tvWall->setWindowTitle(QApplication::translate("page_tvWall", "Form", 0, QApplication::UnicodeUTF8));
        gbox_patrol->setTitle(QApplication::translate("page_tvWall", "\350\275\256\345\267\241", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("page_tvWall", "\351\227\264\351\232\224", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("page_tvWall", "\347\247\222   \351\200\232\351\201\223\346\225\260", 0, QApplication::UnicodeUTF8));
        ckb_patrol_enable->setText(QApplication::translate("page_tvWall", "\345\274\200\345\220\257", 0, QApplication::UnicodeUTF8));
        btn_patrol_apply->setText(QApplication::translate("page_tvWall", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        btn_patrol_sync->setText(QApplication::translate("page_tvWall", "\350\275\256\345\267\241\345\220\214\346\255\245", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class page_tvWall: public Ui_page_tvWall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_TVWALL_H
