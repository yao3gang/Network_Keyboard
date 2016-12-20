/********************************************************************************
** Form generated from reading UI file 'dlg_dev_tree.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_DEV_TREE_H
#define UI_DLG_DEV_TREE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dlg_dev_tree
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lab_tooltip;
    QTreeWidget *treeWidget_dev;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_cancel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_ok;

    void setupUi(QDialog *dlg_dev_tree)
    {
        if (dlg_dev_tree->objectName().isEmpty())
            dlg_dev_tree->setObjectName(QString::fromUtf8("dlg_dev_tree"));
        dlg_dev_tree->resize(420, 480);
        verticalLayout = new QVBoxLayout(dlg_dev_tree);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lab_tooltip = new QLabel(dlg_dev_tree);
        lab_tooltip->setObjectName(QString::fromUtf8("lab_tooltip"));

        verticalLayout->addWidget(lab_tooltip);

        treeWidget_dev = new QTreeWidget(dlg_dev_tree);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget_dev->setHeaderItem(__qtreewidgetitem);
        treeWidget_dev->setObjectName(QString::fromUtf8("treeWidget_dev"));
        treeWidget_dev->setColumnCount(1);

        verticalLayout->addWidget(treeWidget_dev);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_cancel = new QPushButton(dlg_dev_tree);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));
        btn_cancel->setMinimumSize(QSize(75, 0));

        horizontalLayout->addWidget(btn_cancel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btn_ok = new QPushButton(dlg_dev_tree);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));
        btn_ok->setMinimumSize(QSize(75, 0));

        horizontalLayout->addWidget(btn_ok);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(dlg_dev_tree);

        QMetaObject::connectSlotsByName(dlg_dev_tree);
    } // setupUi

    void retranslateUi(QDialog *dlg_dev_tree)
    {
        dlg_dev_tree->setWindowTitle(QApplication::translate("dlg_dev_tree", "Dialog", 0, QApplication::UnicodeUTF8));
        lab_tooltip->setText(QApplication::translate("dlg_dev_tree", "<html><head/><body><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        btn_cancel->setText(QApplication::translate("dlg_dev_tree", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        btn_ok->setText(QApplication::translate("dlg_dev_tree", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlg_dev_tree: public Ui_dlg_dev_tree {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_DEV_TREE_H
