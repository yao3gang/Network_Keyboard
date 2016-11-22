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
#include <QtGui/QTableWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page_tvWall
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lab_tooltip;
    QHBoxLayout *horizontalLayout_2;
    QTreeWidget *treeWidget_scn;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QGroupBox *gbox_preview;
    QVBoxLayout *verticalLayout_2;
    QComboBox *comboBox;
    QPushButton *btn_preview_apply;
    QGroupBox *gbox_poll;
    QVBoxLayout *verticalLayout;
    QCheckBox *cbox_poll_switch;
    QPushButton *btn_poll_sync;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QTableWidget *tableWidget;
    QSpacerItem *horizontalSpacer_2;
    QTreeWidget *treeWidget_nvr;

    void setupUi(QWidget *page_tvWall)
    {
        if (page_tvWall->objectName().isEmpty())
            page_tvWall->setObjectName(QString::fromUtf8("page_tvWall"));
        page_tvWall->resize(1024, 688);
        page_tvWall->setMinimumSize(QSize(0, 0));
        page_tvWall->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_4 = new QVBoxLayout(page_tvWall);
        verticalLayout_4->setSpacing(15);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        lab_tooltip = new QLabel(page_tvWall);
        lab_tooltip->setObjectName(QString::fromUtf8("lab_tooltip"));

        horizontalLayout_4->addWidget(lab_tooltip);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        treeWidget_scn = new QTreeWidget(page_tvWall);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget_scn->setHeaderItem(__qtreewidgetitem);
        treeWidget_scn->setObjectName(QString::fromUtf8("treeWidget_scn"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeWidget_scn->sizePolicy().hasHeightForWidth());
        treeWidget_scn->setSizePolicy(sizePolicy);
        treeWidget_scn->setMinimumSize(QSize(200, 0));
        treeWidget_scn->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_2->addWidget(treeWidget_scn);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gbox_preview = new QGroupBox(page_tvWall);
        gbox_preview->setObjectName(QString::fromUtf8("gbox_preview"));
        verticalLayout_2 = new QVBoxLayout(gbox_preview);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 15, -1, -1);
        comboBox = new QComboBox(gbox_preview);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);
        comboBox->setMinimumSize(QSize(100, 0));

        verticalLayout_2->addWidget(comboBox);

        btn_preview_apply = new QPushButton(gbox_preview);
        btn_preview_apply->setObjectName(QString::fromUtf8("btn_preview_apply"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btn_preview_apply->sizePolicy().hasHeightForWidth());
        btn_preview_apply->setSizePolicy(sizePolicy2);
        btn_preview_apply->setMinimumSize(QSize(75, 0));
        btn_preview_apply->setMaximumSize(QSize(75, 16777215));

        verticalLayout_2->addWidget(btn_preview_apply);


        horizontalLayout->addWidget(gbox_preview);

        gbox_poll = new QGroupBox(page_tvWall);
        gbox_poll->setObjectName(QString::fromUtf8("gbox_poll"));
        verticalLayout = new QVBoxLayout(gbox_poll);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 15, -1, -1);
        cbox_poll_switch = new QCheckBox(gbox_poll);
        cbox_poll_switch->setObjectName(QString::fromUtf8("cbox_poll_switch"));

        verticalLayout->addWidget(cbox_poll_switch);

        btn_poll_sync = new QPushButton(gbox_poll);
        btn_poll_sync->setObjectName(QString::fromUtf8("btn_poll_sync"));
        sizePolicy2.setHeightForWidth(btn_poll_sync->sizePolicy().hasHeightForWidth());
        btn_poll_sync->setSizePolicy(sizePolicy2);
        btn_poll_sync->setMinimumSize(QSize(75, 0));
        btn_poll_sync->setMaximumSize(QSize(75, 16777215));

        verticalLayout->addWidget(btn_poll_sync);


        horizontalLayout->addWidget(gbox_poll);

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
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tableWidget = new QTableWidget(groupBox);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_3->addWidget(tableWidget);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        treeWidget_nvr = new QTreeWidget(groupBox);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        treeWidget_nvr->setHeaderItem(__qtreewidgetitem1);
        treeWidget_nvr->setObjectName(QString::fromUtf8("treeWidget_nvr"));
        sizePolicy.setHeightForWidth(treeWidget_nvr->sizePolicy().hasHeightForWidth());
        treeWidget_nvr->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(treeWidget_nvr);


        verticalLayout_3->addWidget(groupBox);


        horizontalLayout_2->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout_2);


        retranslateUi(page_tvWall);

        QMetaObject::connectSlotsByName(page_tvWall);
    } // setupUi

    void retranslateUi(QWidget *page_tvWall)
    {
        page_tvWall->setWindowTitle(QApplication::translate("page_tvWall", "Form", 0, QApplication::UnicodeUTF8));
        lab_tooltip->setText(QApplication::translate("page_tvWall", "<html><head/><body><p><span style=\" font-size:11pt;\">\350\257\267\344\273\216\345\267\246\350\276\271\351\200\211\345\256\232\344\270\200\344\270\252\345\261\217\345\271\225\350\277\233\350\241\214\350\256\276\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        gbox_preview->setTitle(QApplication::translate("page_tvWall", "\351\242\204\350\247\210\347\224\273\351\235\242", 0, QApplication::UnicodeUTF8));
        btn_preview_apply->setText(QApplication::translate("page_tvWall", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        gbox_poll->setTitle(QApplication::translate("page_tvWall", "\350\275\256\345\267\241", 0, QApplication::UnicodeUTF8));
        cbox_poll_switch->setText(QApplication::translate("page_tvWall", "\345\274\200\345\220\257", 0, QApplication::UnicodeUTF8));
        btn_poll_sync->setText(QApplication::translate("page_tvWall", "\345\220\214\346\255\245", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("page_tvWall", "\345\261\217\347\224\273\351\235\242", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("page_tvWall", "NVR\351\200\232\351\201\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("page_tvWall", "\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class page_tvWall: public Ui_page_tvWall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_TVWALL_H
