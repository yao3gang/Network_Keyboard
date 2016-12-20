/********************************************************************************
** Form generated from reading UI file 'form_playback.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_PLAYBACK_H
#define UI_FORM_PLAYBACK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QTimeEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "treewidget_dev.h"

QT_BEGIN_NAMESPACE

class Ui_form_playback
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget_left3;
    QVBoxLayout *verticalLayout;
    treewidget_dev *treeWidget_nvr;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lab_date;
    QDateEdit *dateEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lab_start;
    QTimeEdit *timeEdit_start;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lab_end;
    QTimeEdit *timeEdit_end;
    QPushButton *btn_srh;
    QPushButton *btn_backup;
    QSpacerItem *verticalSpacer_2;
    QPushButton *btn_to_dec;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QWidget *widget_bottom;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_play_ctrl;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_play;
    QPushButton *btn_stop;
    QPushButton *btn_next_frame;
    QComboBox *comboBox_speed;
    QSlider *slider_play;
    QPushButton *btn_page_start;
    QPushButton *btn_page_pre;
    QPushButton *btn_page_next;
    QPushButton *btn_page_end;
    QLabel *lab_page;
    QPushButton *btn_extra;
    QWidget *widget_result;
    QHBoxLayout *horizontalLayout_6;
    QTableWidget *tableWidget_left;
    QTableWidget *tableWidget_right;

    void setupUi(QWidget *form_playback)
    {
        if (form_playback->objectName().isEmpty())
            form_playback->setObjectName(QString::fromUtf8("form_playback"));
        form_playback->resize(1024, 768);
        horizontalLayout = new QHBoxLayout(form_playback);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_left3 = new QWidget(form_playback);
        widget_left3->setObjectName(QString::fromUtf8("widget_left3"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_left3->sizePolicy().hasHeightForWidth());
        widget_left3->setSizePolicy(sizePolicy);
        widget_left3->setMinimumSize(QSize(200, 0));
        widget_left3->setMaximumSize(QSize(200, 16777215));
        verticalLayout = new QVBoxLayout(widget_left3);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        treeWidget_nvr = new treewidget_dev(widget_left3);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget_nvr->setHeaderItem(__qtreewidgetitem);
        treeWidget_nvr->setObjectName(QString::fromUtf8("treeWidget_nvr"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidget_nvr->sizePolicy().hasHeightForWidth());
        treeWidget_nvr->setSizePolicy(sizePolicy1);
        treeWidget_nvr->setMinimumSize(QSize(0, 0));
        treeWidget_nvr->setMaximumSize(QSize(200, 16777215));

        verticalLayout->addWidget(treeWidget_nvr);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(9, 0, 9, 9);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lab_date = new QLabel(widget_left3);
        lab_date->setObjectName(QString::fromUtf8("lab_date"));

        horizontalLayout_3->addWidget(lab_date);

        dateEdit = new QDateEdit(widget_left3);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dateEdit->sizePolicy().hasHeightForWidth());
        dateEdit->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(dateEdit);


        verticalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lab_start = new QLabel(widget_left3);
        lab_start->setObjectName(QString::fromUtf8("lab_start"));

        horizontalLayout_4->addWidget(lab_start);

        timeEdit_start = new QTimeEdit(widget_left3);
        timeEdit_start->setObjectName(QString::fromUtf8("timeEdit_start"));
        sizePolicy2.setHeightForWidth(timeEdit_start->sizePolicy().hasHeightForWidth());
        timeEdit_start->setSizePolicy(sizePolicy2);
        timeEdit_start->setMinimumDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0)));

        horizontalLayout_4->addWidget(timeEdit_start);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lab_end = new QLabel(widget_left3);
        lab_end->setObjectName(QString::fromUtf8("lab_end"));

        horizontalLayout_5->addWidget(lab_end);

        timeEdit_end = new QTimeEdit(widget_left3);
        timeEdit_end->setObjectName(QString::fromUtf8("timeEdit_end"));
        sizePolicy2.setHeightForWidth(timeEdit_end->sizePolicy().hasHeightForWidth());
        timeEdit_end->setSizePolicy(sizePolicy2);
        timeEdit_end->setMinimumDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0)));

        horizontalLayout_5->addWidget(timeEdit_end);


        verticalLayout_4->addLayout(horizontalLayout_5);

        btn_srh = new QPushButton(widget_left3);
        btn_srh->setObjectName(QString::fromUtf8("btn_srh"));
        btn_srh->setMinimumSize(QSize(0, 28));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_srh->setIcon(icon);
        btn_srh->setIconSize(QSize(20, 20));

        verticalLayout_4->addWidget(btn_srh);

        btn_backup = new QPushButton(widget_left3);
        btn_backup->setObjectName(QString::fromUtf8("btn_backup"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/image/btn_save.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_backup->setIcon(icon1);
        btn_backup->setIconSize(QSize(20, 20));

        verticalLayout_4->addWidget(btn_backup);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_2);

        btn_to_dec = new QPushButton(widget_left3);
        btn_to_dec->setObjectName(QString::fromUtf8("btn_to_dec"));
        btn_to_dec->setMinimumSize(QSize(0, 28));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/image/image_next.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_to_dec->setIcon(icon2);
        btn_to_dec->setIconSize(QSize(20, 20));

        verticalLayout_4->addWidget(btn_to_dec);


        verticalLayout->addLayout(verticalLayout_4);

        verticalLayout->setStretch(0, 1);

        horizontalLayout->addWidget(widget_left3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(form_playback);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy3);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(frame);

        widget_bottom = new QWidget(form_playback);
        widget_bottom->setObjectName(QString::fromUtf8("widget_bottom"));
        sizePolicy1.setHeightForWidth(widget_bottom->sizePolicy().hasHeightForWidth());
        widget_bottom->setSizePolicy(sizePolicy1);
        widget_bottom->setMinimumSize(QSize(0, 0));
        widget_bottom->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_3 = new QVBoxLayout(widget_bottom);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget_play_ctrl = new QWidget(widget_bottom);
        widget_play_ctrl->setObjectName(QString::fromUtf8("widget_play_ctrl"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widget_play_ctrl->sizePolicy().hasHeightForWidth());
        widget_play_ctrl->setSizePolicy(sizePolicy4);
        horizontalLayout_2 = new QHBoxLayout(widget_play_ctrl);
        horizontalLayout_2->setContentsMargins(8, 8, 8, 8);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btn_play = new QPushButton(widget_play_ctrl);
        btn_play->setObjectName(QString::fromUtf8("btn_play"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(btn_play->sizePolicy().hasHeightForWidth());
        btn_play->setSizePolicy(sizePolicy5);
        btn_play->setMinimumSize(QSize(24, 24));
        btn_play->setMaximumSize(QSize(24, 24));

        horizontalLayout_2->addWidget(btn_play);

        btn_stop = new QPushButton(widget_play_ctrl);
        btn_stop->setObjectName(QString::fromUtf8("btn_stop"));
        sizePolicy5.setHeightForWidth(btn_stop->sizePolicy().hasHeightForWidth());
        btn_stop->setSizePolicy(sizePolicy5);
        btn_stop->setMinimumSize(QSize(24, 24));
        btn_stop->setMaximumSize(QSize(24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/image/pctl_stop.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        btn_stop->setIcon(icon3);

        horizontalLayout_2->addWidget(btn_stop);

        btn_next_frame = new QPushButton(widget_play_ctrl);
        btn_next_frame->setObjectName(QString::fromUtf8("btn_next_frame"));
        sizePolicy5.setHeightForWidth(btn_next_frame->sizePolicy().hasHeightForWidth());
        btn_next_frame->setSizePolicy(sizePolicy5);
        btn_next_frame->setMinimumSize(QSize(24, 24));
        btn_next_frame->setMaximumSize(QSize(24, 24));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/image/pctl_frame.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        btn_next_frame->setIcon(icon4);

        horizontalLayout_2->addWidget(btn_next_frame);

        comboBox_speed = new QComboBox(widget_play_ctrl);
        comboBox_speed->setObjectName(QString::fromUtf8("comboBox_speed"));
        sizePolicy5.setHeightForWidth(comboBox_speed->sizePolicy().hasHeightForWidth());
        comboBox_speed->setSizePolicy(sizePolicy5);

        horizontalLayout_2->addWidget(comboBox_speed);

        slider_play = new QSlider(widget_play_ctrl);
        slider_play->setObjectName(QString::fromUtf8("slider_play"));
        slider_play->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(slider_play);

        btn_page_start = new QPushButton(widget_play_ctrl);
        btn_page_start->setObjectName(QString::fromUtf8("btn_page_start"));
        sizePolicy5.setHeightForWidth(btn_page_start->sizePolicy().hasHeightForWidth());
        btn_page_start->setSizePolicy(sizePolicy5);
        btn_page_start->setMinimumSize(QSize(24, 24));
        btn_page_start->setMaximumSize(QSize(24, 24));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/image/page_start.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_page_start->setIcon(icon5);

        horizontalLayout_2->addWidget(btn_page_start);

        btn_page_pre = new QPushButton(widget_play_ctrl);
        btn_page_pre->setObjectName(QString::fromUtf8("btn_page_pre"));
        sizePolicy5.setHeightForWidth(btn_page_pre->sizePolicy().hasHeightForWidth());
        btn_page_pre->setSizePolicy(sizePolicy5);
        btn_page_pre->setMinimumSize(QSize(24, 24));
        btn_page_pre->setMaximumSize(QSize(24, 24));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/image/page_pre.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_page_pre->setIcon(icon6);

        horizontalLayout_2->addWidget(btn_page_pre);

        btn_page_next = new QPushButton(widget_play_ctrl);
        btn_page_next->setObjectName(QString::fromUtf8("btn_page_next"));
        sizePolicy5.setHeightForWidth(btn_page_next->sizePolicy().hasHeightForWidth());
        btn_page_next->setSizePolicy(sizePolicy5);
        btn_page_next->setMinimumSize(QSize(24, 24));
        btn_page_next->setMaximumSize(QSize(24, 24));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/image/page_next.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_page_next->setIcon(icon7);

        horizontalLayout_2->addWidget(btn_page_next);

        btn_page_end = new QPushButton(widget_play_ctrl);
        btn_page_end->setObjectName(QString::fromUtf8("btn_page_end"));
        sizePolicy5.setHeightForWidth(btn_page_end->sizePolicy().hasHeightForWidth());
        btn_page_end->setSizePolicy(sizePolicy5);
        btn_page_end->setMinimumSize(QSize(24, 24));
        btn_page_end->setMaximumSize(QSize(24, 24));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/image/page_end.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_page_end->setIcon(icon8);

        horizontalLayout_2->addWidget(btn_page_end);

        lab_page = new QLabel(widget_play_ctrl);
        lab_page->setObjectName(QString::fromUtf8("lab_page"));

        horizontalLayout_2->addWidget(lab_page);

        btn_extra = new QPushButton(widget_play_ctrl);
        btn_extra->setObjectName(QString::fromUtf8("btn_extra"));
        sizePolicy5.setHeightForWidth(btn_extra->sizePolicy().hasHeightForWidth());
        btn_extra->setSizePolicy(sizePolicy5);
        btn_extra->setMinimumSize(QSize(24, 24));
        btn_extra->setMaximumSize(QSize(24, 24));
        btn_extra->setIconSize(QSize(24, 24));

        horizontalLayout_2->addWidget(btn_extra);


        verticalLayout_3->addWidget(widget_play_ctrl);

        widget_result = new QWidget(widget_bottom);
        widget_result->setObjectName(QString::fromUtf8("widget_result"));
        sizePolicy1.setHeightForWidth(widget_result->sizePolicy().hasHeightForWidth());
        widget_result->setSizePolicy(sizePolicy1);
        horizontalLayout_6 = new QHBoxLayout(widget_result);
        horizontalLayout_6->setSpacing(3);
        horizontalLayout_6->setContentsMargins(8, 8, 8, 8);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        tableWidget_left = new QTableWidget(widget_result);
        tableWidget_left->setObjectName(QString::fromUtf8("tableWidget_left"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(tableWidget_left->sizePolicy().hasHeightForWidth());
        tableWidget_left->setSizePolicy(sizePolicy6);
        tableWidget_left->setMaximumSize(QSize(16777215, 190));

        horizontalLayout_6->addWidget(tableWidget_left);

        tableWidget_right = new QTableWidget(widget_result);
        tableWidget_right->setObjectName(QString::fromUtf8("tableWidget_right"));
        sizePolicy6.setHeightForWidth(tableWidget_right->sizePolicy().hasHeightForWidth());
        tableWidget_right->setSizePolicy(sizePolicy6);
        tableWidget_right->setMaximumSize(QSize(16777215, 190));

        horizontalLayout_6->addWidget(tableWidget_right);


        verticalLayout_3->addWidget(widget_result);

        verticalLayout_3->setStretch(1, 1);

        verticalLayout_2->addWidget(widget_bottom);

        verticalLayout_2->setStretch(0, 1);

        horizontalLayout->addLayout(verticalLayout_2);


        retranslateUi(form_playback);

        QMetaObject::connectSlotsByName(form_playback);
    } // setupUi

    void retranslateUi(QWidget *form_playback)
    {
        form_playback->setWindowTitle(QApplication::translate("form_playback", "Form", 0, QApplication::UnicodeUTF8));
        lab_date->setText(QApplication::translate("form_playback", "\346\227\245\346\234\237", 0, QApplication::UnicodeUTF8));
        dateEdit->setDisplayFormat(QApplication::translate("form_playback", "yyyy-MM-dd", 0, QApplication::UnicodeUTF8));
        lab_start->setText(QApplication::translate("form_playback", "\350\265\267\345\247\213", 0, QApplication::UnicodeUTF8));
        timeEdit_start->setDisplayFormat(QApplication::translate("form_playback", "HH:mm:ss", 0, QApplication::UnicodeUTF8));
        lab_end->setText(QApplication::translate("form_playback", "\347\273\223\346\235\237", 0, QApplication::UnicodeUTF8));
        timeEdit_end->setDisplayFormat(QApplication::translate("form_playback", "HH:mm:ss", 0, QApplication::UnicodeUTF8));
        btn_srh->setText(QApplication::translate("form_playback", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        btn_backup->setText(QApplication::translate("form_playback", "\345\244\207\344\273\275", 0, QApplication::UnicodeUTF8));
        btn_to_dec->setText(QApplication::translate("form_playback", "\345\217\221\351\200\201\350\207\263\350\247\243\347\240\201\345\231\250", 0, QApplication::UnicodeUTF8));
        btn_stop->setText(QString());
        btn_next_frame->setText(QString());
        btn_page_next->setText(QString());
        btn_page_end->setText(QString());
        lab_page->setText(QApplication::translate("form_playback", "TextLabel", 0, QApplication::UnicodeUTF8));
        btn_extra->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class form_playback: public Ui_form_playback {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_PLAYBACK_H
