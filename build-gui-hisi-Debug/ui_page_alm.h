/********************************************************************************
** Form generated from reading UI file 'page_alm.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE_ALM_H
#define UI_PAGE_ALM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page_alm
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget_left;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_info;
    QPushButton *btn_link;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget_config;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_clr;
    QSpacerItem *horizontalSpacer_3;
    QTableWidget *tableWidget_alm_info;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lab_tooltip;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QTreeWidget *treeWidget_nvr;
    QTableWidget *tableWidget_link;

    void setupUi(QWidget *page_alm)
    {
        if (page_alm->objectName().isEmpty())
            page_alm->setObjectName(QString::fromUtf8("page_alm"));
        page_alm->resize(1024, 688);
        horizontalLayout = new QHBoxLayout(page_alm);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_left = new QWidget(page_alm);
        widget_left->setObjectName(QString::fromUtf8("widget_left"));
        widget_left->setMinimumSize(QSize(100, 0));
        widget_left->setMaximumSize(QSize(100, 16777215));
        verticalLayout = new QVBoxLayout(widget_left);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btn_info = new QPushButton(widget_left);
        btn_info->setObjectName(QString::fromUtf8("btn_info"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_info->sizePolicy().hasHeightForWidth());
        btn_info->setSizePolicy(sizePolicy);
        btn_info->setMinimumSize(QSize(100, 50));
        btn_info->setCheckable(true);

        verticalLayout->addWidget(btn_info);

        btn_link = new QPushButton(widget_left);
        btn_link->setObjectName(QString::fromUtf8("btn_link"));
        sizePolicy.setHeightForWidth(btn_link->sizePolicy().hasHeightForWidth());
        btn_link->setSizePolicy(sizePolicy);
        btn_link->setMinimumSize(QSize(100, 50));
        btn_link->setCheckable(true);

        verticalLayout->addWidget(btn_link);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget_left);

        stackedWidget_config = new QStackedWidget(page_alm);
        stackedWidget_config->setObjectName(QString::fromUtf8("stackedWidget_config"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(stackedWidget_config->sizePolicy().hasHeightForWidth());
        stackedWidget_config->setSizePolicy(sizePolicy1);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btn_clr = new QPushButton(page);
        btn_clr->setObjectName(QString::fromUtf8("btn_clr"));
        sizePolicy.setHeightForWidth(btn_clr->sizePolicy().hasHeightForWidth());
        btn_clr->setSizePolicy(sizePolicy);
        btn_clr->setMinimumSize(QSize(75, 23));

        horizontalLayout_2->addWidget(btn_clr);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_2);

        tableWidget_alm_info = new QTableWidget(page);
        if (tableWidget_alm_info->columnCount() < 4)
            tableWidget_alm_info->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_alm_info->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_alm_info->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_alm_info->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_alm_info->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget_alm_info->setObjectName(QString::fromUtf8("tableWidget_alm_info"));
        tableWidget_alm_info->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(tableWidget_alm_info);

        stackedWidget_config->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_3 = new QVBoxLayout(page_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        lab_tooltip = new QLabel(page_2);
        lab_tooltip->setObjectName(QString::fromUtf8("lab_tooltip"));

        horizontalLayout_3->addWidget(lab_tooltip);


        verticalLayout_3->addLayout(horizontalLayout_3);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        treeWidget_nvr = new QTreeWidget(page_2);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget_nvr->setHeaderItem(__qtreewidgetitem);
        treeWidget_nvr->setObjectName(QString::fromUtf8("treeWidget_nvr"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(treeWidget_nvr->sizePolicy().hasHeightForWidth());
        treeWidget_nvr->setSizePolicy(sizePolicy2);
        treeWidget_nvr->setMinimumSize(QSize(200, 0));
        treeWidget_nvr->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_4->addWidget(treeWidget_nvr);

        tableWidget_link = new QTableWidget(page_2);
        if (tableWidget_link->columnCount() < 3)
            tableWidget_link->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_link->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_link->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_link->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        tableWidget_link->setObjectName(QString::fromUtf8("tableWidget_link"));
        tableWidget_link->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_4->addWidget(tableWidget_link);


        verticalLayout_3->addLayout(horizontalLayout_4);

        stackedWidget_config->addWidget(page_2);

        horizontalLayout->addWidget(stackedWidget_config);


        retranslateUi(page_alm);

        stackedWidget_config->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(page_alm);
    } // setupUi

    void retranslateUi(QWidget *page_alm)
    {
        page_alm->setWindowTitle(QApplication::translate("page_alm", "Form", 0, QApplication::UnicodeUTF8));
        btn_info->setText(QApplication::translate("page_alm", "\346\212\245\350\255\246\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        btn_link->setText(QApplication::translate("page_alm", "\346\212\245\350\255\246\350\201\224\345\212\250", 0, QApplication::UnicodeUTF8));
        btn_clr->setText(QApplication::translate("page_alm", "\346\270\205\351\231\244", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_alm_info->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("page_alm", "\345\272\217\345\217\267", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_alm_info->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("page_alm", "\350\256\276\345\244\207IP", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_alm_info->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("page_alm", "\351\200\232\351\201\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_alm_info->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("page_alm", "\346\212\245\350\255\246\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        lab_tooltip->setText(QApplication::translate("page_alm", "<html><head/><body><p>    \350\257\267\345\205\210\345\234\250\345\267\246\350\276\271\346\241\206\344\270\255\351\200\211\344\270\255\345\276\205\350\256\276\347\275\256\347\232\204NVR\357\274\214\347\204\266\345\220\216\345\257\271\347\211\271\345\256\232\347\232\204\346\212\245\350\255\246\346\272\220\351\200\211\346\213\251\350\201\224\345\212\250\347\232\204\350\247\243\347\240\201\345\231\250\351\200\232\351\201\223\343\200\202</p></body></html>", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_link->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("page_alm", "NVR\346\212\245\350\255\246\346\272\220", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_link->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("page_alm", "\350\201\224\345\212\250\350\247\243\347\240\201\345\231\250\351\200\232\351\201\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_link->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("page_alm", "\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class page_alm: public Ui_page_alm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_ALM_H
