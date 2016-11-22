/********************************************************************************
** Form generated from reading UI file 'wzd_tvwall_layout.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WZD_TVWALL_LAYOUT_H
#define UI_WZD_TVWALL_LAYOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QWizard>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_wzd_tvWall_layout
{
public:
    QWizardPage *wizardPage1;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_left_tv;
    QVBoxLayout *verticalLayout;
    QLabel *lab_title;
    QLabel *lab_tvWall_name;
    QLineEdit *lineEdit_tvWall_name;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lab_h;
    QLineEdit *lineEdit_tvWall_h;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lab_v;
    QLineEdit *lineEdit_tvWall_v;
    QLabel *lab_description;
    QTextEdit *textEdit;
    QSpacerItem *verticalSpacer;
    QWidget *widget_disp;
    QWizardPage *wizardPage2;
    QVBoxLayout *verticalLayout_2;
    QLabel *lab_bind;
    QLabel *lab_help;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QTableWidget *tableWidget_screen;
    QSpacerItem *horizontalSpacer;
    QTreeWidget *treeWidget_dec;

    void setupUi(QWizard *wzd_tvWall_layout)
    {
        if (wzd_tvWall_layout->objectName().isEmpty())
            wzd_tvWall_layout->setObjectName(QString::fromUtf8("wzd_tvWall_layout"));
        wzd_tvWall_layout->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(wzd_tvWall_layout->sizePolicy().hasHeightForWidth());
        wzd_tvWall_layout->setSizePolicy(sizePolicy);
        wzd_tvWall_layout->setMinimumSize(QSize(800, 600));
        wzd_tvWall_layout->setMaximumSize(QSize(800, 600));
        wzd_tvWall_layout->setSizeGripEnabled(false);
        wzd_tvWall_layout->setModal(false);
        wzd_tvWall_layout->setOptions(QWizard::CancelButtonOnLeft|QWizard::HelpButtonOnRight);
        wizardPage1 = new QWizardPage();
        wizardPage1->setObjectName(QString::fromUtf8("wizardPage1"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(wizardPage1->sizePolicy().hasHeightForWidth());
        wizardPage1->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(wizardPage1);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_left_tv = new QWidget(wizardPage1);
        widget_left_tv->setObjectName(QString::fromUtf8("widget_left_tv"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_left_tv->sizePolicy().hasHeightForWidth());
        widget_left_tv->setSizePolicy(sizePolicy2);
        widget_left_tv->setMinimumSize(QSize(150, 0));
        widget_left_tv->setMaximumSize(QSize(150, 16777215));
        verticalLayout = new QVBoxLayout(widget_left_tv);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lab_title = new QLabel(widget_left_tv);
        lab_title->setObjectName(QString::fromUtf8("lab_title"));
        lab_title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lab_title);

        lab_tvWall_name = new QLabel(widget_left_tv);
        lab_tvWall_name->setObjectName(QString::fromUtf8("lab_tvWall_name"));

        verticalLayout->addWidget(lab_tvWall_name);

        lineEdit_tvWall_name = new QLineEdit(widget_left_tv);
        lineEdit_tvWall_name->setObjectName(QString::fromUtf8("lineEdit_tvWall_name"));

        verticalLayout->addWidget(lineEdit_tvWall_name);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lab_h = new QLabel(widget_left_tv);
        lab_h->setObjectName(QString::fromUtf8("lab_h"));

        horizontalLayout_2->addWidget(lab_h);

        lineEdit_tvWall_h = new QLineEdit(widget_left_tv);
        lineEdit_tvWall_h->setObjectName(QString::fromUtf8("lineEdit_tvWall_h"));

        horizontalLayout_2->addWidget(lineEdit_tvWall_h);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lab_v = new QLabel(widget_left_tv);
        lab_v->setObjectName(QString::fromUtf8("lab_v"));

        horizontalLayout_3->addWidget(lab_v);

        lineEdit_tvWall_v = new QLineEdit(widget_left_tv);
        lineEdit_tvWall_v->setObjectName(QString::fromUtf8("lineEdit_tvWall_v"));

        horizontalLayout_3->addWidget(lineEdit_tvWall_v);


        verticalLayout->addLayout(horizontalLayout_3);

        lab_description = new QLabel(widget_left_tv);
        lab_description->setObjectName(QString::fromUtf8("lab_description"));

        verticalLayout->addWidget(lab_description);

        textEdit = new QTextEdit(widget_left_tv);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget_left_tv);

        widget_disp = new QWidget(wizardPage1);
        widget_disp->setObjectName(QString::fromUtf8("widget_disp"));
        sizePolicy1.setHeightForWidth(widget_disp->sizePolicy().hasHeightForWidth());
        widget_disp->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(widget_disp);

        wzd_tvWall_layout->addPage(wizardPage1);
        wizardPage2 = new QWizardPage();
        wizardPage2->setObjectName(QString::fromUtf8("wizardPage2"));
        sizePolicy1.setHeightForWidth(wizardPage2->sizePolicy().hasHeightForWidth());
        wizardPage2->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(wizardPage2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lab_bind = new QLabel(wizardPage2);
        lab_bind->setObjectName(QString::fromUtf8("lab_bind"));

        verticalLayout_2->addWidget(lab_bind);

        lab_help = new QLabel(wizardPage2);
        lab_help->setObjectName(QString::fromUtf8("lab_help"));

        verticalLayout_2->addWidget(lab_help);

        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        tableWidget_screen = new QTableWidget(wizardPage2);
        if (tableWidget_screen->columnCount() < 3)
            tableWidget_screen->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_screen->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_screen->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_screen->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget_screen->setObjectName(QString::fromUtf8("tableWidget_screen"));

        horizontalLayout_4->addWidget(tableWidget_screen);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        treeWidget_dec = new QTreeWidget(wizardPage2);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget_dec->setHeaderItem(__qtreewidgetitem);
        treeWidget_dec->setObjectName(QString::fromUtf8("treeWidget_dec"));
        treeWidget_dec->setMinimumSize(QSize(200, 0));
        treeWidget_dec->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_4->addWidget(treeWidget_dec);


        verticalLayout_2->addLayout(horizontalLayout_4);

        wzd_tvWall_layout->addPage(wizardPage2);

        retranslateUi(wzd_tvWall_layout);

        QMetaObject::connectSlotsByName(wzd_tvWall_layout);
    } // setupUi

    void retranslateUi(QWizard *wzd_tvWall_layout)
    {
        wzd_tvWall_layout->setWindowTitle(QApplication::translate("wzd_tvWall_layout", "Wizard", 0, QApplication::UnicodeUTF8));
        lab_title->setText(QApplication::translate("wzd_tvWall_layout", "<html><head/><body><p><span style=\" font-size:11pt;\">\345\270\203\345\261\200\350\256\276\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lab_tvWall_name->setText(QApplication::translate("wzd_tvWall_layout", "<html><head/><body><p>\347\224\265\350\247\206\345\242\231\345\220\215\347\247\260:</p></body></html>", 0, QApplication::UnicodeUTF8));
        lab_h->setText(QApplication::translate("wzd_tvWall_layout", "\350\241\214\345\270\203\345\261\200\357\274\232", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_tvWall_h->setToolTip(QApplication::translate("wzd_tvWall_layout", "<html><head/><body><p>\344\270\200\350\241\214\346\234\211\345\244\232\345\260\221\344\270\252\345\261\217\345\271\225</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lab_v->setText(QApplication::translate("wzd_tvWall_layout", "\345\210\227\345\270\203\345\261\200\357\274\232", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_tvWall_v->setToolTip(QApplication::translate("wzd_tvWall_layout", "<html><head/><body><p>\344\270\200\345\210\227\346\234\211\345\244\232\345\260\221\344\270\252\345\261\217\345\271\225</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lab_description->setText(QApplication::translate("wzd_tvWall_layout", "\346\217\217\350\277\260\357\274\232", 0, QApplication::UnicodeUTF8));
        lab_bind->setText(QApplication::translate("wzd_tvWall_layout", "<html><head/><body><p><span style=\" font-size:11pt;\">\347\273\221\345\256\232\350\256\276\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lab_help->setText(QApplication::translate("wzd_tvWall_layout", "<html><head/><body><p>\344\273\216\345\217\263\346\241\206\344\270\255\346\213\226\346\213\275\344\270\200\344\270\252\350\247\243\347\240\201\345\231\250\346\224\276\345\205\245\345\267\246\346\241\206\345\261\217\345\271\225\345\220\216\344\270\200\344\270\252\345\215\225\345\205\203\346\240\274\344\270\255\357\274\214\344\273\245\347\241\256\345\256\232\347\241\256\345\256\232\345\261\217\345\222\214\350\247\243\347\240\201\345\231\250\347\232\204\347\273\221\345\256\232\345\205\263\347\263\273\343\200\202</p></body></html>", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_screen->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("wzd_tvWall_layout", "\345\261\217\345\271\225", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_screen->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("wzd_tvWall_layout", "\350\247\243\347\240\201\345\231\250", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_screen->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("wzd_tvWall_layout", "\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class wzd_tvWall_layout: public Ui_wzd_tvWall_layout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WZD_TVWALL_LAYOUT_H
