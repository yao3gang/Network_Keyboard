/********************************************************************************
** Form generated from reading UI file 'form_tvwall_config.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_TVWALL_CONFIG_H
#define UI_FORM_TVWALL_CONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "tablewidget_tvwall.h"
#include "treewidget_dev.h"

QT_BEGIN_NAMESPACE

class Ui_form_tvwall_config
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QWidget *gridWidget;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_tvwall_cols;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *lineEdit_tvwall_rows;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    tablewidget_tvwall *tableWidget_tvwall;
    treewidget_dev *tree_swich_dec;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_clr;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_cancel;
    QPushButton *btn_ok;

    void setupUi(QWidget *form_tvwall_config)
    {
        if (form_tvwall_config->objectName().isEmpty())
            form_tvwall_config->setObjectName(QString::fromUtf8("form_tvwall_config"));
        form_tvwall_config->resize(800, 600);
        verticalLayout = new QVBoxLayout(form_tvwall_config);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(form_tvwall_config);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridWidget = new QWidget(form_tvwall_config);
        gridWidget->setObjectName(QString::fromUtf8("gridWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gridWidget->sizePolicy().hasHeightForWidth());
        gridWidget->setSizePolicy(sizePolicy);
        gridWidget->setMinimumSize(QSize(150, 0));
        gridWidget->setMaximumSize(QSize(150, 16777215));
        gridLayout = new QGridLayout(gridWidget);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit_tvwall_cols = new QLineEdit(gridWidget);
        lineEdit_tvwall_cols->setObjectName(QString::fromUtf8("lineEdit_tvwall_cols"));

        gridLayout->addWidget(lineEdit_tvwall_cols, 2, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 2);

        label_3 = new QLabel(gridWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_2 = new QLabel(gridWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 0, 1, 2);

        lineEdit_tvwall_rows = new QLineEdit(gridWidget);
        lineEdit_tvwall_rows->setObjectName(QString::fromUtf8("lineEdit_tvwall_rows"));

        gridLayout->addWidget(lineEdit_tvwall_rows, 1, 1, 1, 1);

        label_4 = new QLabel(gridWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);


        horizontalLayout->addWidget(gridWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, -1, 0, -1);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tableWidget_tvwall = new tablewidget_tvwall(form_tvwall_config);
        tableWidget_tvwall->setObjectName(QString::fromUtf8("tableWidget_tvwall"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableWidget_tvwall->sizePolicy().hasHeightForWidth());
        tableWidget_tvwall->setSizePolicy(sizePolicy1);
        tableWidget_tvwall->setMinimumSize(QSize(300, 0));
        tableWidget_tvwall->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_tvwall->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_2->addWidget(tableWidget_tvwall);

        tree_swich_dec = new treewidget_dev(form_tvwall_config);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tree_swich_dec->setHeaderItem(__qtreewidgetitem);
        tree_swich_dec->setObjectName(QString::fromUtf8("tree_swich_dec"));
        sizePolicy.setHeightForWidth(tree_swich_dec->sizePolicy().hasHeightForWidth());
        tree_swich_dec->setSizePolicy(sizePolicy);
        tree_swich_dec->setMinimumSize(QSize(200, 0));
        tree_swich_dec->setMaximumSize(QSize(200, 16777215));
        tree_swich_dec->header()->setVisible(true);

        horizontalLayout_2->addWidget(tree_swich_dec);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, -1, -1, 0);
        btn_clr = new QPushButton(form_tvwall_config);
        btn_clr->setObjectName(QString::fromUtf8("btn_clr"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btn_clr->sizePolicy().hasHeightForWidth());
        btn_clr->setSizePolicy(sizePolicy2);
        btn_clr->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_3->addWidget(btn_clr);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        btn_cancel = new QPushButton(form_tvwall_config);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));

        horizontalLayout_3->addWidget(btn_cancel);

        btn_ok = new QPushButton(form_tvwall_config);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));

        horizontalLayout_3->addWidget(btn_ok);


        verticalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(1, 1);
        QWidget::setTabOrder(lineEdit_tvwall_rows, lineEdit_tvwall_cols);

        retranslateUi(form_tvwall_config);

        QMetaObject::connectSlotsByName(form_tvwall_config);
    } // setupUi

    void retranslateUi(QWidget *form_tvwall_config)
    {
        form_tvwall_config->setWindowTitle(QApplication::translate("form_tvwall_config", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("form_tvwall_config", "\350\257\267\345\205\210\350\256\276\347\275\256\345\270\203\345\261\200\357\274\214\347\204\266\345\220\216\344\273\216\345\217\263\346\241\206\346\213\226\346\213\275\344\270\200\344\270\252\350\247\243\347\240\201\345\231\250\346\224\276\345\205\245\345\267\246\346\241\206\350\247\243\347\240\201\345\231\250\345\210\227\347\233\270\345\272\224\345\215\225\345\205\203\346\240\274\344\270\255\357\274\214\344\273\245\347\241\256\345\256\232\347\241\256\345\256\232\345\222\214\345\261\217\345\271\225\347\232\204\347\273\221\345\256\232\345\205\263\347\263\273\343\200\202", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("form_tvwall_config", "\350\241\214\346\225\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("form_tvwall_config", "\350\256\276\347\275\256\345\270\203\345\261\200", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("form_tvwall_config", "\345\210\227\346\225\260\357\274\232", 0, QApplication::UnicodeUTF8));
        btn_clr->setText(QApplication::translate("form_tvwall_config", "\346\270\205\347\251\272", 0, QApplication::UnicodeUTF8));
        btn_cancel->setText(QApplication::translate("form_tvwall_config", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        btn_ok->setText(QApplication::translate("form_tvwall_config", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class form_tvwall_config: public Ui_form_tvwall_config {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_TVWALL_CONFIG_H
