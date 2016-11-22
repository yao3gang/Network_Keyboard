/********************************************************************************
** Form generated from reading UI file 'page_dev_mgt.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE_DEV_MGT_H
#define UI_PAGE_DEV_MGT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page_dev_mgt
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_left1;
    QVBoxLayout *verticalLayout_4;
    QLabel *lab_srh_type;
    QComboBox *cmb_srh_type;
    QPushButton *btn_srh;
    QPushButton *btn_add;
    QSpacerItem *verticalSpacer;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget_srh;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_left2;
    QVBoxLayout *verticalLayout_5;
    QLabel *lab_dev_all;
    QLabel *lab_added_type;
    QComboBox *cmb_added_type;
    QPushButton *btn_info;
    QPushButton *btn_del;
    QSpacerItem *verticalSpacer_2;
    QPushButton *btn_del_all;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget_dev;

    void setupUi(QWidget *page_dev_mgt)
    {
        if (page_dev_mgt->objectName().isEmpty())
            page_dev_mgt->setObjectName(QString::fromUtf8("page_dev_mgt"));
        page_dev_mgt->resize(1024, 688);
        verticalLayout = new QVBoxLayout(page_dev_mgt);
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        verticalLayout->setContentsMargins(9, 9, 9, 9);
#endif
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(page_dev_mgt);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
#ifndef Q_OS_MAC
        horizontalLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        horizontalLayout->setContentsMargins(9, 9, 9, 9);
#endif
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_left1 = new QWidget(groupBox);
        widget_left1->setObjectName(QString::fromUtf8("widget_left1"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_left1->sizePolicy().hasHeightForWidth());
        widget_left1->setSizePolicy(sizePolicy);
        widget_left1->setMinimumSize(QSize(150, 0));
        widget_left1->setMaximumSize(QSize(150, 16777215));
        verticalLayout_4 = new QVBoxLayout(widget_left1);
#ifndef Q_OS_MAC
        verticalLayout_4->setSpacing(6);
#endif
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        lab_srh_type = new QLabel(widget_left1);
        lab_srh_type->setObjectName(QString::fromUtf8("lab_srh_type"));

        verticalLayout_4->addWidget(lab_srh_type);

        cmb_srh_type = new QComboBox(widget_left1);
        cmb_srh_type->setObjectName(QString::fromUtf8("cmb_srh_type"));

        verticalLayout_4->addWidget(cmb_srh_type);

        btn_srh = new QPushButton(widget_left1);
        btn_srh->setObjectName(QString::fromUtf8("btn_srh"));

        verticalLayout_4->addWidget(btn_srh);

        btn_add = new QPushButton(widget_left1);
        btn_add->setObjectName(QString::fromUtf8("btn_add"));

        verticalLayout_4->addWidget(btn_add);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget_left1);

        widget_3 = new QWidget(groupBox);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        verticalLayout_2 = new QVBoxLayout(widget_3);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget_srh = new QTableWidget(widget_3);
        if (tableWidget_srh->columnCount() < 4)
            tableWidget_srh->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_srh->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_srh->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_srh->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_srh->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget_srh->setObjectName(QString::fromUtf8("tableWidget_srh"));
        tableWidget_srh->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_srh->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(tableWidget_srh);


        horizontalLayout->addWidget(widget_3);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(page_dev_mgt);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
#ifndef Q_OS_MAC
        horizontalLayout_2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        horizontalLayout_2->setContentsMargins(9, 9, 9, 9);
#endif
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        widget_left2 = new QWidget(groupBox_2);
        widget_left2->setObjectName(QString::fromUtf8("widget_left2"));
        sizePolicy.setHeightForWidth(widget_left2->sizePolicy().hasHeightForWidth());
        widget_left2->setSizePolicy(sizePolicy);
        widget_left2->setMinimumSize(QSize(150, 0));
        widget_left2->setMaximumSize(QSize(150, 16777215));
        verticalLayout_5 = new QVBoxLayout(widget_left2);
#ifndef Q_OS_MAC
        verticalLayout_5->setSpacing(6);
#endif
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        lab_dev_all = new QLabel(widget_left2);
        lab_dev_all->setObjectName(QString::fromUtf8("lab_dev_all"));
        lab_dev_all->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lab_dev_all);

        lab_added_type = new QLabel(widget_left2);
        lab_added_type->setObjectName(QString::fromUtf8("lab_added_type"));

        verticalLayout_5->addWidget(lab_added_type);

        cmb_added_type = new QComboBox(widget_left2);
        cmb_added_type->setObjectName(QString::fromUtf8("cmb_added_type"));

        verticalLayout_5->addWidget(cmb_added_type);

        btn_info = new QPushButton(widget_left2);
        btn_info->setObjectName(QString::fromUtf8("btn_info"));

        verticalLayout_5->addWidget(btn_info);

        btn_del = new QPushButton(widget_left2);
        btn_del->setObjectName(QString::fromUtf8("btn_del"));

        verticalLayout_5->addWidget(btn_del);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        btn_del_all = new QPushButton(widget_left2);
        btn_del_all->setObjectName(QString::fromUtf8("btn_del_all"));

        verticalLayout_5->addWidget(btn_del_all);

        lab_dev_all->raise();
        btn_info->raise();
        btn_del->raise();
        btn_del_all->raise();
        lab_added_type->raise();
        cmb_added_type->raise();

        horizontalLayout_2->addWidget(widget_left2);

        widget_4 = new QWidget(groupBox_2);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        verticalLayout_3 = new QVBoxLayout(widget_4);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tableWidget_dev = new QTableWidget(widget_4);
        if (tableWidget_dev->columnCount() < 5)
            tableWidget_dev->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_dev->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_dev->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_dev->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_dev->setHorizontalHeaderItem(3, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_dev->setHorizontalHeaderItem(4, __qtablewidgetitem8);
        tableWidget_dev->setObjectName(QString::fromUtf8("tableWidget_dev"));
        tableWidget_dev->horizontalHeader()->setStretchLastSection(true);
        tableWidget_dev->verticalHeader()->setVisible(true);

        verticalLayout_3->addWidget(tableWidget_dev);


        horizontalLayout_2->addWidget(widget_4);


        verticalLayout->addWidget(groupBox_2);


        retranslateUi(page_dev_mgt);

        QMetaObject::connectSlotsByName(page_dev_mgt);
    } // setupUi

    void retranslateUi(QWidget *page_dev_mgt)
    {
        page_dev_mgt->setWindowTitle(QApplication::translate("page_dev_mgt", "Form", 0, QApplication::UnicodeUTF8));
        lab_srh_type->setText(QApplication::translate("page_dev_mgt", "\350\256\276\345\244\207\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        btn_srh->setText(QApplication::translate("page_dev_mgt", "\346\220\234\347\264\242", 0, QApplication::UnicodeUTF8));
        btn_add->setText(QApplication::translate("page_dev_mgt", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_srh->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("page_dev_mgt", "\351\200\211\344\270\255", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_srh->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("page_dev_mgt", "\350\256\276\345\244\207\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_srh->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("page_dev_mgt", "IP", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_srh->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("page_dev_mgt", "\351\200\232\351\201\223\346\225\260", 0, QApplication::UnicodeUTF8));
        lab_dev_all->setText(QApplication::translate("page_dev_mgt", "<html><head/><body><p><span style=\" font-size:11pt;\">\345\267\262\346\267\273\345\212\240\350\256\276\345\244\207</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lab_added_type->setText(QApplication::translate("page_dev_mgt", "\350\256\276\345\244\207\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        btn_info->setText(QApplication::translate("page_dev_mgt", "\350\257\246\347\273\206\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        btn_del->setText(QApplication::translate("page_dev_mgt", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        btn_del_all->setText(QApplication::translate("page_dev_mgt", "\345\205\250\351\203\250\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_dev->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("page_dev_mgt", "\351\200\211\344\270\255", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_dev->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("page_dev_mgt", "\350\256\276\345\244\207\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_dev->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("page_dev_mgt", "IP", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_dev->horizontalHeaderItem(3);
        ___qtablewidgetitem7->setText(QApplication::translate("page_dev_mgt", "\351\200\232\351\201\223\346\225\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_dev->horizontalHeaderItem(4);
        ___qtablewidgetitem8->setText(QApplication::translate("page_dev_mgt", "\350\277\236\346\216\245\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class page_dev_mgt: public Ui_page_dev_mgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_DEV_MGT_H
