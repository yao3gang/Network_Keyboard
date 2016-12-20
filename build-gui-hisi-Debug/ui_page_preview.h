/********************************************************************************
** Form generated from reading UI file 'page_preview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE_PREVIEW_H
#define UI_PAGE_PREVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page_preview
{
public:
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_left;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_ptz;
    QPushButton *btn_ptz_top;
    QPushButton *btn_ptz_bottom;
    QPushButton *btn_ptz_right;
    QPushButton *btn_ptz_left;
    QPushButton *btn_ptz_top_right;
    QPushButton *btn_ptz_top_left;
    QPushButton *btn_ptz_bottom_left;
    QPushButton *btn_ptz_bottom_right;
    QPushButton *btn_ptz_centre;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_focus_inc;
    QLabel *lab_focus;
    QPushButton *btn_focus_dec;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_aperture_inc;
    QLabel *lab_aperture;
    QPushButton *btn_aperture_dec;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_zoom_inc;
    QLabel *lab_zoom;
    QPushButton *btn_zoom_dec;
    QFrame *frame;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QWidget *tab_3;
    QWidget *tab_4;

    void setupUi(QWidget *page_preview)
    {
        if (page_preview->objectName().isEmpty())
            page_preview->setObjectName(QString::fromUtf8("page_preview"));
        page_preview->resize(1024, 688);
        horizontalLayout_4 = new QHBoxLayout(page_preview);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        widget_left = new QWidget(page_preview);
        widget_left->setObjectName(QString::fromUtf8("widget_left"));
        verticalLayout = new QVBoxLayout(widget_left);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        treeWidget = new QTreeWidget(widget_left);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);
        treeWidget->setMinimumSize(QSize(200, 0));
        treeWidget->setMaximumSize(QSize(200, 16777215));

        verticalLayout->addWidget(treeWidget);

        groupBox = new QGroupBox(widget_left);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(200, 200));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget_ptz = new QWidget(groupBox);
        widget_ptz->setObjectName(QString::fromUtf8("widget_ptz"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_ptz->sizePolicy().hasHeightForWidth());
        widget_ptz->setSizePolicy(sizePolicy2);
        widget_ptz->setMinimumSize(QSize(0, 180));
        btn_ptz_top = new QPushButton(widget_ptz);
        btn_ptz_top->setObjectName(QString::fromUtf8("btn_ptz_top"));
        btn_ptz_top->setGeometry(QRect(86, 0, 30, 30));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btn_ptz_top->sizePolicy().hasHeightForWidth());
        btn_ptz_top->setSizePolicy(sizePolicy3);
        btn_ptz_top->setIconSize(QSize(30, 30));
        btn_ptz_bottom = new QPushButton(widget_ptz);
        btn_ptz_bottom->setObjectName(QString::fromUtf8("btn_ptz_bottom"));
        btn_ptz_bottom->setGeometry(QRect(86, 150, 30, 30));
        sizePolicy3.setHeightForWidth(btn_ptz_bottom->sizePolicy().hasHeightForWidth());
        btn_ptz_bottom->setSizePolicy(sizePolicy3);
        btn_ptz_bottom->setIconSize(QSize(30, 30));
        btn_ptz_right = new QPushButton(widget_ptz);
        btn_ptz_right->setObjectName(QString::fromUtf8("btn_ptz_right"));
        btn_ptz_right->setGeometry(QRect(161, 75, 30, 30));
        sizePolicy3.setHeightForWidth(btn_ptz_right->sizePolicy().hasHeightForWidth());
        btn_ptz_right->setSizePolicy(sizePolicy3);
        btn_ptz_right->setIconSize(QSize(30, 30));
        btn_ptz_left = new QPushButton(widget_ptz);
        btn_ptz_left->setObjectName(QString::fromUtf8("btn_ptz_left"));
        btn_ptz_left->setGeometry(QRect(11, 75, 30, 30));
        sizePolicy3.setHeightForWidth(btn_ptz_left->sizePolicy().hasHeightForWidth());
        btn_ptz_left->setSizePolicy(sizePolicy3);
        btn_ptz_left->setIconSize(QSize(30, 30));
        btn_ptz_top_right = new QPushButton(widget_ptz);
        btn_ptz_top_right->setObjectName(QString::fromUtf8("btn_ptz_top_right"));
        btn_ptz_top_right->setGeometry(QRect(141, 25, 30, 30));
        btn_ptz_top_right->setIconSize(QSize(30, 30));
        btn_ptz_top_left = new QPushButton(widget_ptz);
        btn_ptz_top_left->setObjectName(QString::fromUtf8("btn_ptz_top_left"));
        btn_ptz_top_left->setGeometry(QRect(31, 25, 30, 30));
        btn_ptz_top_left->setIconSize(QSize(30, 30));
        btn_ptz_bottom_left = new QPushButton(widget_ptz);
        btn_ptz_bottom_left->setObjectName(QString::fromUtf8("btn_ptz_bottom_left"));
        btn_ptz_bottom_left->setGeometry(QRect(31, 125, 30, 30));
        btn_ptz_bottom_left->setIconSize(QSize(30, 30));
        btn_ptz_bottom_right = new QPushButton(widget_ptz);
        btn_ptz_bottom_right->setObjectName(QString::fromUtf8("btn_ptz_bottom_right"));
        btn_ptz_bottom_right->setGeometry(QRect(141, 125, 30, 30));
        btn_ptz_bottom_right->setIconSize(QSize(30, 30));
        btn_ptz_centre = new QPushButton(widget_ptz);
        btn_ptz_centre->setObjectName(QString::fromUtf8("btn_ptz_centre"));
        btn_ptz_centre->setGeometry(QRect(86, 75, 30, 30));
        sizePolicy3.setHeightForWidth(btn_ptz_centre->sizePolicy().hasHeightForWidth());
        btn_ptz_centre->setSizePolicy(sizePolicy3);
        btn_ptz_centre->setIconSize(QSize(30, 30));
        btn_ptz_bottom_right->raise();
        btn_ptz_centre->raise();
        btn_ptz_top->raise();
        btn_ptz_bottom->raise();
        btn_ptz_right->raise();
        btn_ptz_left->raise();
        btn_ptz_top_right->raise();
        btn_ptz_top_left->raise();
        btn_ptz_bottom_left->raise();

        verticalLayout_3->addWidget(widget_ptz);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btn_focus_inc = new QPushButton(groupBox);
        btn_focus_inc->setObjectName(QString::fromUtf8("btn_focus_inc"));
        sizePolicy3.setHeightForWidth(btn_focus_inc->sizePolicy().hasHeightForWidth());
        btn_focus_inc->setSizePolicy(sizePolicy3);
        btn_focus_inc->setMinimumSize(QSize(45, 35));
        btn_focus_inc->setMaximumSize(QSize(45, 35));

        horizontalLayout_2->addWidget(btn_focus_inc);

        lab_focus = new QLabel(groupBox);
        lab_focus->setObjectName(QString::fromUtf8("lab_focus"));
        lab_focus->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lab_focus);

        btn_focus_dec = new QPushButton(groupBox);
        btn_focus_dec->setObjectName(QString::fromUtf8("btn_focus_dec"));
        sizePolicy3.setHeightForWidth(btn_focus_dec->sizePolicy().hasHeightForWidth());
        btn_focus_dec->setSizePolicy(sizePolicy3);
        btn_focus_dec->setMinimumSize(QSize(45, 35));
        btn_focus_dec->setMaximumSize(QSize(45, 35));

        horizontalLayout_2->addWidget(btn_focus_dec);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btn_aperture_inc = new QPushButton(groupBox);
        btn_aperture_inc->setObjectName(QString::fromUtf8("btn_aperture_inc"));
        sizePolicy3.setHeightForWidth(btn_aperture_inc->sizePolicy().hasHeightForWidth());
        btn_aperture_inc->setSizePolicy(sizePolicy3);
        btn_aperture_inc->setMinimumSize(QSize(45, 35));
        btn_aperture_inc->setMaximumSize(QSize(45, 35));

        horizontalLayout_3->addWidget(btn_aperture_inc);

        lab_aperture = new QLabel(groupBox);
        lab_aperture->setObjectName(QString::fromUtf8("lab_aperture"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lab_aperture->sizePolicy().hasHeightForWidth());
        lab_aperture->setSizePolicy(sizePolicy4);
        lab_aperture->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(lab_aperture);

        btn_aperture_dec = new QPushButton(groupBox);
        btn_aperture_dec->setObjectName(QString::fromUtf8("btn_aperture_dec"));
        sizePolicy3.setHeightForWidth(btn_aperture_dec->sizePolicy().hasHeightForWidth());
        btn_aperture_dec->setSizePolicy(sizePolicy3);
        btn_aperture_dec->setMinimumSize(QSize(45, 35));
        btn_aperture_dec->setMaximumSize(QSize(45, 35));

        horizontalLayout_3->addWidget(btn_aperture_dec);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_zoom_inc = new QPushButton(groupBox);
        btn_zoom_inc->setObjectName(QString::fromUtf8("btn_zoom_inc"));
        sizePolicy3.setHeightForWidth(btn_zoom_inc->sizePolicy().hasHeightForWidth());
        btn_zoom_inc->setSizePolicy(sizePolicy3);
        btn_zoom_inc->setMinimumSize(QSize(45, 35));
        btn_zoom_inc->setMaximumSize(QSize(45, 35));

        horizontalLayout->addWidget(btn_zoom_inc);

        lab_zoom = new QLabel(groupBox);
        lab_zoom->setObjectName(QString::fromUtf8("lab_zoom"));
        lab_zoom->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lab_zoom);

        btn_zoom_dec = new QPushButton(groupBox);
        btn_zoom_dec->setObjectName(QString::fromUtf8("btn_zoom_dec"));
        sizePolicy3.setHeightForWidth(btn_zoom_dec->sizePolicy().hasHeightForWidth());
        btn_zoom_dec->setSizePolicy(sizePolicy3);
        btn_zoom_dec->setMinimumSize(QSize(45, 35));
        btn_zoom_dec->setMaximumSize(QSize(45, 35));

        horizontalLayout->addWidget(btn_zoom_dec);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout->addWidget(groupBox);

        verticalLayout->setStretch(0, 1);

        horizontalLayout_4->addWidget(widget_left);

        frame = new QFrame(page_preview);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy5);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(1);
        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(60, 280, 271, 241));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tabWidget->addTab(tab_4, QString());

        horizontalLayout_4->addWidget(frame);


        retranslateUi(page_preview);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(page_preview);
    } // setupUi

    void retranslateUi(QWidget *page_preview)
    {
        page_preview->setWindowTitle(QApplication::translate("page_preview", "Form", 0, QApplication::UnicodeUTF8));
        btn_ptz_top->setText(QString());
        btn_ptz_bottom->setText(QString());
        btn_ptz_right->setText(QString());
        btn_ptz_left->setText(QString());
        btn_ptz_top_right->setText(QString());
        btn_ptz_top_left->setText(QString());
        btn_ptz_bottom_left->setText(QString());
        btn_ptz_bottom_right->setText(QString());
        btn_ptz_centre->setText(QString());
        btn_focus_inc->setText(QApplication::translate("page_preview", "focus+", 0, QApplication::UnicodeUTF8));
        lab_focus->setText(QApplication::translate("page_preview", "\350\201\232\347\204\246", 0, QApplication::UnicodeUTF8));
        btn_focus_dec->setText(QApplication::translate("page_preview", "focus-", 0, QApplication::UnicodeUTF8));
        btn_aperture_inc->setText(QApplication::translate("page_preview", "aperture+", 0, QApplication::UnicodeUTF8));
        lab_aperture->setText(QApplication::translate("page_preview", "\345\205\211\345\234\210", 0, QApplication::UnicodeUTF8));
        btn_aperture_dec->setText(QApplication::translate("page_preview", "aperture-", 0, QApplication::UnicodeUTF8));
        btn_zoom_inc->setText(QApplication::translate("page_preview", "zomm+", 0, QApplication::UnicodeUTF8));
        lab_zoom->setText(QApplication::translate("page_preview", "\345\217\230\345\200\215", 0, QApplication::UnicodeUTF8));
        btn_zoom_dec->setText(QApplication::translate("page_preview", "zomm-", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("page_preview", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("page_preview", "Tab 2", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("page_preview", "\351\241\265", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("page_preview", "\351\241\265", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class page_preview: public Ui_page_preview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_PREVIEW_H
