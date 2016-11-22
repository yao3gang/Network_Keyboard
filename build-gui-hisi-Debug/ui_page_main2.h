/********************************************************************************
** Form generated from reading UI file 'page_main2.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE_MAIN2_H
#define UI_PAGE_MAIN2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page_main2
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_title;
    QHBoxLayout *horizontalLayout;
    QLabel *lab_ico;
    QSpacerItem *horizontalSpacer;
    QLabel *lab_title;
    QWidget *widget_top;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *btn_preview;
    QToolButton *btn_playback;
    QToolButton *btn_tvWall;
    QToolButton *btn_alarm;
    QToolButton *btn_device;
    QToolButton *btn_config;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_datetime;
    QVBoxLayout *verticalLayout_3;
    QLabel *lab_date;
    QLabel *lab_time;
    QWidget *widget_main;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;

    void setupUi(QWidget *page_main2)
    {
        if (page_main2->objectName().isEmpty())
            page_main2->setObjectName(QString::fromUtf8("page_main2"));
        page_main2->resize(1024, 768);
        verticalLayout = new QVBoxLayout(page_main2);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_title = new QWidget(page_main2);
        widget_title->setObjectName(QString::fromUtf8("widget_title"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_title->sizePolicy().hasHeightForWidth());
        widget_title->setSizePolicy(sizePolicy);
        widget_title->setMinimumSize(QSize(0, 80));
        widget_title->setMaximumSize(QSize(16777215, 80));
        horizontalLayout = new QHBoxLayout(widget_title);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lab_ico = new QLabel(widget_title);
        lab_ico->setObjectName(QString::fromUtf8("lab_ico"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lab_ico->sizePolicy().hasHeightForWidth());
        lab_ico->setSizePolicy(sizePolicy1);
        lab_ico->setMinimumSize(QSize(0, 0));
        lab_ico->setMaximumSize(QSize(16777215, 16777215));
        lab_ico->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lab_ico);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lab_title = new QLabel(widget_title);
        lab_title->setObjectName(QString::fromUtf8("lab_title"));
        lab_title->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(lab_title);

        widget_top = new QWidget(widget_title);
        widget_top->setObjectName(QString::fromUtf8("widget_top"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_top->sizePolicy().hasHeightForWidth());
        widget_top->setSizePolicy(sizePolicy2);
        horizontalLayout_2 = new QHBoxLayout(widget_top);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btn_preview = new QToolButton(widget_top);
        btn_preview->setObjectName(QString::fromUtf8("btn_preview"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btn_preview->sizePolicy().hasHeightForWidth());
        btn_preview->setSizePolicy(sizePolicy3);
        btn_preview->setMinimumSize(QSize(95, 0));
        btn_preview->setMaximumSize(QSize(95, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/banner/main_about.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_preview->setIcon(icon);
        btn_preview->setIconSize(QSize(35, 35));
        btn_preview->setCheckable(true);
        btn_preview->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_2->addWidget(btn_preview);

        btn_playback = new QToolButton(widget_top);
        btn_playback->setObjectName(QString::fromUtf8("btn_playback"));
        sizePolicy3.setHeightForWidth(btn_playback->sizePolicy().hasHeightForWidth());
        btn_playback->setSizePolicy(sizePolicy3);
        btn_playback->setMinimumSize(QSize(95, 0));
        btn_playback->setMaximumSize(QSize(95, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/banner/main_data.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_playback->setIcon(icon1);
        btn_playback->setIconSize(QSize(35, 35));
        btn_playback->setCheckable(true);
        btn_playback->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_2->addWidget(btn_playback);

        btn_tvWall = new QToolButton(widget_top);
        btn_tvWall->setObjectName(QString::fromUtf8("btn_tvWall"));
        sizePolicy3.setHeightForWidth(btn_tvWall->sizePolicy().hasHeightForWidth());
        btn_tvWall->setSizePolicy(sizePolicy3);
        btn_tvWall->setMinimumSize(QSize(95, 0));
        btn_tvWall->setMaximumSize(QSize(95, 16777215));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/banner/main_exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_tvWall->setIcon(icon2);
        btn_tvWall->setIconSize(QSize(35, 35));
        btn_tvWall->setCheckable(true);
        btn_tvWall->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_2->addWidget(btn_tvWall);

        btn_alarm = new QToolButton(widget_top);
        btn_alarm->setObjectName(QString::fromUtf8("btn_alarm"));
        sizePolicy3.setHeightForWidth(btn_alarm->sizePolicy().hasHeightForWidth());
        btn_alarm->setSizePolicy(sizePolicy3);
        btn_alarm->setMinimumSize(QSize(95, 0));
        btn_alarm->setMaximumSize(QSize(95, 16777215));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/banner/main_company.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_alarm->setIcon(icon3);
        btn_alarm->setIconSize(QSize(35, 35));
        btn_alarm->setCheckable(true);
        btn_alarm->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_2->addWidget(btn_alarm);

        btn_device = new QToolButton(widget_top);
        btn_device->setObjectName(QString::fromUtf8("btn_device"));
        sizePolicy3.setHeightForWidth(btn_device->sizePolicy().hasHeightForWidth());
        btn_device->setSizePolicy(sizePolicy3);
        btn_device->setMinimumSize(QSize(95, 0));
        btn_device->setMaximumSize(QSize(95, 16777215));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/banner/main_help.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_device->setIcon(icon4);
        btn_device->setIconSize(QSize(35, 35));
        btn_device->setCheckable(true);
        btn_device->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_2->addWidget(btn_device);

        btn_config = new QToolButton(widget_top);
        btn_config->setObjectName(QString::fromUtf8("btn_config"));
        sizePolicy3.setHeightForWidth(btn_config->sizePolicy().hasHeightForWidth());
        btn_config->setSizePolicy(sizePolicy3);
        btn_config->setMinimumSize(QSize(95, 0));
        btn_config->setMaximumSize(QSize(95, 16777215));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/banner/main_config.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_config->setIcon(icon5);
        btn_config->setIconSize(QSize(35, 35));
        btn_config->setCheckable(true);
        btn_config->setPopupMode(QToolButton::DelayedPopup);
        btn_config->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_2->addWidget(btn_config);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        horizontalLayout->addWidget(widget_top);

        widget_datetime = new QWidget(widget_title);
        widget_datetime->setObjectName(QString::fromUtf8("widget_datetime"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widget_datetime->sizePolicy().hasHeightForWidth());
        widget_datetime->setSizePolicy(sizePolicy4);
        widget_datetime->setMinimumSize(QSize(190, 0));
        widget_datetime->setMaximumSize(QSize(190, 16777215));
        verticalLayout_3 = new QVBoxLayout(widget_datetime);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lab_date = new QLabel(widget_datetime);
        lab_date->setObjectName(QString::fromUtf8("lab_date"));
        sizePolicy1.setHeightForWidth(lab_date->sizePolicy().hasHeightForWidth());
        lab_date->setSizePolicy(sizePolicy1);
        lab_date->setMinimumSize(QSize(0, 0));
        lab_date->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_3->addWidget(lab_date);

        lab_time = new QLabel(widget_datetime);
        lab_time->setObjectName(QString::fromUtf8("lab_time"));
        sizePolicy1.setHeightForWidth(lab_time->sizePolicy().hasHeightForWidth());
        lab_time->setSizePolicy(sizePolicy1);
        lab_time->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_3->addWidget(lab_time);


        horizontalLayout->addWidget(widget_datetime);


        verticalLayout->addWidget(widget_title);

        widget_main = new QWidget(page_main2);
        widget_main->setObjectName(QString::fromUtf8("widget_main"));
        sizePolicy1.setHeightForWidth(widget_main->sizePolicy().hasHeightForWidth());
        widget_main->setSizePolicy(sizePolicy1);
        widget_main->setMinimumSize(QSize(0, 0));
        widget_main->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_2 = new QVBoxLayout(widget_main);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        stackedWidget = new QStackedWidget(widget_main);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));

        verticalLayout_2->addWidget(stackedWidget);


        verticalLayout->addWidget(widget_main);


        retranslateUi(page_main2);

        QMetaObject::connectSlotsByName(page_main2);
    } // setupUi

    void retranslateUi(QWidget *page_main2)
    {
        page_main2->setWindowTitle(QApplication::translate("page_main2", "Form", 0, QApplication::UnicodeUTF8));
        lab_ico->setText(QString());
        lab_title->setText(QString());
        btn_preview->setText(QApplication::translate("page_main2", "\351\242\204\350\247\210", 0, QApplication::UnicodeUTF8));
        btn_playback->setText(QApplication::translate("page_main2", "\345\233\236\346\224\276", 0, QApplication::UnicodeUTF8));
        btn_tvWall->setText(QApplication::translate("page_main2", "\347\224\265\350\247\206\345\242\231", 0, QApplication::UnicodeUTF8));
        btn_alarm->setText(QApplication::translate("page_main2", "\346\212\245\350\255\246\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        btn_device->setText(QApplication::translate("page_main2", "\350\256\276\345\244\207\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        btn_config->setText(QApplication::translate("page_main2", "\347\263\273\347\273\237\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        lab_date->setText(QString());
        lab_time->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class page_main2: public Ui_page_main2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_MAIN2_H
