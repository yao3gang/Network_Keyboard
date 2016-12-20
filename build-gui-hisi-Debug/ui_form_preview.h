/********************************************************************************
** Form generated from reading UI file 'form_preview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_PREVIEW_H
#define UI_FORM_PREVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_form_preview
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
    QWidget *widget;

    void setupUi(QWidget *form_preview)
    {
        if (form_preview->objectName().isEmpty())
            form_preview->setObjectName(QString::fromUtf8("form_preview"));
        form_preview->resize(1024, 768);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(form_preview->sizePolicy().hasHeightForWidth());
        form_preview->setSizePolicy(sizePolicy);
        form_preview->setMinimumSize(QSize(1024, 768));
        form_preview->setMaximumSize(QSize(1024, 768));
        horizontalLayout_4 = new QHBoxLayout(form_preview);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        widget_left = new QWidget(form_preview);
        widget_left->setObjectName(QString::fromUtf8("widget_left"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_left->sizePolicy().hasHeightForWidth());
        widget_left->setSizePolicy(sizePolicy1);
        widget_left->setMinimumSize(QSize(200, 0));
        widget_left->setMaximumSize(QSize(200, 16777215));
        verticalLayout = new QVBoxLayout(widget_left);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 0, 6);
        treeWidget = new QTreeWidget(widget_left);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy2);
        treeWidget->setMinimumSize(QSize(0, 0));
        treeWidget->setMaximumSize(QSize(200, 16777215));

        verticalLayout->addWidget(treeWidget);

        groupBox = new QGroupBox(widget_left);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        groupBox->setMinimumSize(QSize(0, 300));
        groupBox->setMaximumSize(QSize(16777215, 300));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget_ptz = new QWidget(groupBox);
        widget_ptz->setObjectName(QString::fromUtf8("widget_ptz"));
        sizePolicy3.setHeightForWidth(widget_ptz->sizePolicy().hasHeightForWidth());
        widget_ptz->setSizePolicy(sizePolicy3);
        widget_ptz->setMinimumSize(QSize(0, 180));
        btn_ptz_top = new QPushButton(widget_ptz);
        btn_ptz_top->setObjectName(QString::fromUtf8("btn_ptz_top"));
        btn_ptz_top->setGeometry(QRect(80, 0, 30, 30));
        sizePolicy.setHeightForWidth(btn_ptz_top->sizePolicy().hasHeightForWidth());
        btn_ptz_top->setSizePolicy(sizePolicy);
        btn_ptz_top->setIconSize(QSize(30, 30));
        btn_ptz_bottom = new QPushButton(widget_ptz);
        btn_ptz_bottom->setObjectName(QString::fromUtf8("btn_ptz_bottom"));
        btn_ptz_bottom->setGeometry(QRect(80, 150, 30, 30));
        sizePolicy.setHeightForWidth(btn_ptz_bottom->sizePolicy().hasHeightForWidth());
        btn_ptz_bottom->setSizePolicy(sizePolicy);
        btn_ptz_bottom->setIconSize(QSize(30, 30));
        btn_ptz_right = new QPushButton(widget_ptz);
        btn_ptz_right->setObjectName(QString::fromUtf8("btn_ptz_right"));
        btn_ptz_right->setGeometry(QRect(155, 75, 30, 30));
        sizePolicy.setHeightForWidth(btn_ptz_right->sizePolicy().hasHeightForWidth());
        btn_ptz_right->setSizePolicy(sizePolicy);
        btn_ptz_right->setIconSize(QSize(30, 30));
        btn_ptz_left = new QPushButton(widget_ptz);
        btn_ptz_left->setObjectName(QString::fromUtf8("btn_ptz_left"));
        btn_ptz_left->setGeometry(QRect(5, 75, 30, 30));
        sizePolicy.setHeightForWidth(btn_ptz_left->sizePolicy().hasHeightForWidth());
        btn_ptz_left->setSizePolicy(sizePolicy);
        btn_ptz_left->setIconSize(QSize(30, 30));
        btn_ptz_top_right = new QPushButton(widget_ptz);
        btn_ptz_top_right->setObjectName(QString::fromUtf8("btn_ptz_top_right"));
        btn_ptz_top_right->setGeometry(QRect(135, 25, 30, 30));
        btn_ptz_top_right->setIconSize(QSize(30, 30));
        btn_ptz_top_left = new QPushButton(widget_ptz);
        btn_ptz_top_left->setObjectName(QString::fromUtf8("btn_ptz_top_left"));
        btn_ptz_top_left->setGeometry(QRect(25, 25, 30, 30));
        btn_ptz_top_left->setIconSize(QSize(30, 30));
        btn_ptz_bottom_left = new QPushButton(widget_ptz);
        btn_ptz_bottom_left->setObjectName(QString::fromUtf8("btn_ptz_bottom_left"));
        btn_ptz_bottom_left->setGeometry(QRect(25, 125, 30, 30));
        btn_ptz_bottom_left->setIconSize(QSize(30, 30));
        btn_ptz_bottom_right = new QPushButton(widget_ptz);
        btn_ptz_bottom_right->setObjectName(QString::fromUtf8("btn_ptz_bottom_right"));
        btn_ptz_bottom_right->setGeometry(QRect(135, 125, 30, 30));
        btn_ptz_bottom_right->setIconSize(QSize(30, 30));
        btn_ptz_centre = new QPushButton(widget_ptz);
        btn_ptz_centre->setObjectName(QString::fromUtf8("btn_ptz_centre"));
        btn_ptz_centre->setGeometry(QRect(80, 75, 30, 30));
        sizePolicy.setHeightForWidth(btn_ptz_centre->sizePolicy().hasHeightForWidth());
        btn_ptz_centre->setSizePolicy(sizePolicy);
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
        sizePolicy.setHeightForWidth(btn_focus_inc->sizePolicy().hasHeightForWidth());
        btn_focus_inc->setSizePolicy(sizePolicy);
        btn_focus_inc->setMinimumSize(QSize(45, 35));
        btn_focus_inc->setMaximumSize(QSize(45, 35));

        horizontalLayout_2->addWidget(btn_focus_inc);

        lab_focus = new QLabel(groupBox);
        lab_focus->setObjectName(QString::fromUtf8("lab_focus"));
        lab_focus->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lab_focus);

        btn_focus_dec = new QPushButton(groupBox);
        btn_focus_dec->setObjectName(QString::fromUtf8("btn_focus_dec"));
        sizePolicy.setHeightForWidth(btn_focus_dec->sizePolicy().hasHeightForWidth());
        btn_focus_dec->setSizePolicy(sizePolicy);
        btn_focus_dec->setMinimumSize(QSize(45, 35));
        btn_focus_dec->setMaximumSize(QSize(45, 35));

        horizontalLayout_2->addWidget(btn_focus_dec);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btn_aperture_inc = new QPushButton(groupBox);
        btn_aperture_inc->setObjectName(QString::fromUtf8("btn_aperture_inc"));
        sizePolicy.setHeightForWidth(btn_aperture_inc->sizePolicy().hasHeightForWidth());
        btn_aperture_inc->setSizePolicy(sizePolicy);
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
        sizePolicy.setHeightForWidth(btn_aperture_dec->sizePolicy().hasHeightForWidth());
        btn_aperture_dec->setSizePolicy(sizePolicy);
        btn_aperture_dec->setMinimumSize(QSize(45, 35));
        btn_aperture_dec->setMaximumSize(QSize(45, 35));

        horizontalLayout_3->addWidget(btn_aperture_dec);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_zoom_inc = new QPushButton(groupBox);
        btn_zoom_inc->setObjectName(QString::fromUtf8("btn_zoom_inc"));
        sizePolicy.setHeightForWidth(btn_zoom_inc->sizePolicy().hasHeightForWidth());
        btn_zoom_inc->setSizePolicy(sizePolicy);
        btn_zoom_inc->setMinimumSize(QSize(45, 35));
        btn_zoom_inc->setMaximumSize(QSize(45, 35));

        horizontalLayout->addWidget(btn_zoom_inc);

        lab_zoom = new QLabel(groupBox);
        lab_zoom->setObjectName(QString::fromUtf8("lab_zoom"));
        lab_zoom->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lab_zoom);

        btn_zoom_dec = new QPushButton(groupBox);
        btn_zoom_dec->setObjectName(QString::fromUtf8("btn_zoom_dec"));
        sizePolicy.setHeightForWidth(btn_zoom_dec->sizePolicy().hasHeightForWidth());
        btn_zoom_dec->setSizePolicy(sizePolicy);
        btn_zoom_dec->setMinimumSize(QSize(45, 35));
        btn_zoom_dec->setMaximumSize(QSize(45, 35));

        horizontalLayout->addWidget(btn_zoom_dec);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout->addWidget(groupBox);

        verticalLayout->setStretch(0, 1);

        horizontalLayout_4->addWidget(widget_left);

        widget = new QWidget(form_preview);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy5);

        horizontalLayout_4->addWidget(widget);


        retranslateUi(form_preview);

        QMetaObject::connectSlotsByName(form_preview);
    } // setupUi

    void retranslateUi(QWidget *form_preview)
    {
        form_preview->setWindowTitle(QApplication::translate("form_preview", "Form", 0, QApplication::UnicodeUTF8));
        btn_ptz_top->setText(QString());
        btn_ptz_bottom->setText(QString());
        btn_ptz_right->setText(QString());
        btn_ptz_left->setText(QString());
        btn_ptz_top_right->setText(QString());
        btn_ptz_top_left->setText(QString());
        btn_ptz_bottom_left->setText(QString());
        btn_ptz_bottom_right->setText(QString());
        btn_ptz_centre->setText(QString());
        btn_focus_inc->setText(QApplication::translate("form_preview", "focus+", 0, QApplication::UnicodeUTF8));
        lab_focus->setText(QApplication::translate("form_preview", "\350\201\232\347\204\246", 0, QApplication::UnicodeUTF8));
        btn_focus_dec->setText(QApplication::translate("form_preview", "focus-", 0, QApplication::UnicodeUTF8));
        btn_aperture_inc->setText(QApplication::translate("form_preview", "aperture+", 0, QApplication::UnicodeUTF8));
        lab_aperture->setText(QApplication::translate("form_preview", "\345\205\211\345\234\210", 0, QApplication::UnicodeUTF8));
        btn_aperture_dec->setText(QApplication::translate("form_preview", "aperture-", 0, QApplication::UnicodeUTF8));
        btn_zoom_inc->setText(QApplication::translate("form_preview", "zomm+", 0, QApplication::UnicodeUTF8));
        lab_zoom->setText(QApplication::translate("form_preview", "\345\217\230\345\200\215", 0, QApplication::UnicodeUTF8));
        btn_zoom_dec->setText(QApplication::translate("form_preview", "zomm-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class form_preview: public Ui_form_preview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_PREVIEW_H
