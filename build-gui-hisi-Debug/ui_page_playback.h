/********************************************************************************
** Form generated from reading UI file 'page_playback.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE_PLAYBACK_H
#define UI_PAGE_PLAYBACK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page_playback
{
public:
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QLabel *lab_srh_info;
    QTreeWidget *treeWidget_nvr;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lab_srh_start;
    QDateTimeEdit *dateTimeEdit_start;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lab_srh_end;
    QDateTimeEdit *dateTimeEdit_start_2;
    QPushButton *btn_srh;
    QPushButton *btn_save;
    QSpacerItem *verticalSpacer;
    QPushButton *btn_to_dec;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_srh_ret;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *tableWidget;
    QLabel *lab_play_tip;
    QWidget *page_play;
    QVBoxLayout *verticalLayout_5;
    QFrame *frame_play_window;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_play;
    QPushButton *btn_next;
    QPushButton *btn_stop;
    QPushButton *btn_pre;
    QComboBox *comboBox_speed;
    QSpacerItem *horizontalSpacer;
    QSlider *horizontalSlider;

    void setupUi(QWidget *page_playback)
    {
        if (page_playback->objectName().isEmpty())
            page_playback->setObjectName(QString::fromUtf8("page_playback"));
        page_playback->resize(1024, 688);
        horizontalLayout_4 = new QHBoxLayout(page_playback);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        groupBox = new QGroupBox(page_playback);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lab_srh_info = new QLabel(groupBox);
        lab_srh_info->setObjectName(QString::fromUtf8("lab_srh_info"));

        verticalLayout_3->addWidget(lab_srh_info);

        treeWidget_nvr = new QTreeWidget(groupBox);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget_nvr->setHeaderItem(__qtreewidgetitem);
        treeWidget_nvr->setObjectName(QString::fromUtf8("treeWidget_nvr"));
        sizePolicy.setHeightForWidth(treeWidget_nvr->sizePolicy().hasHeightForWidth());
        treeWidget_nvr->setSizePolicy(sizePolicy);
        treeWidget_nvr->setMinimumSize(QSize(200, 0));
        treeWidget_nvr->setMaximumSize(QSize(200, 16777215));

        verticalLayout_3->addWidget(treeWidget_nvr);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lab_srh_start = new QLabel(groupBox);
        lab_srh_start->setObjectName(QString::fromUtf8("lab_srh_start"));

        horizontalLayout_2->addWidget(lab_srh_start);

        dateTimeEdit_start = new QDateTimeEdit(groupBox);
        dateTimeEdit_start->setObjectName(QString::fromUtf8("dateTimeEdit_start"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dateTimeEdit_start->sizePolicy().hasHeightForWidth());
        dateTimeEdit_start->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(dateTimeEdit_start);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lab_srh_end = new QLabel(groupBox);
        lab_srh_end->setObjectName(QString::fromUtf8("lab_srh_end"));

        horizontalLayout_3->addWidget(lab_srh_end);

        dateTimeEdit_start_2 = new QDateTimeEdit(groupBox);
        dateTimeEdit_start_2->setObjectName(QString::fromUtf8("dateTimeEdit_start_2"));
        sizePolicy1.setHeightForWidth(dateTimeEdit_start_2->sizePolicy().hasHeightForWidth());
        dateTimeEdit_start_2->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(dateTimeEdit_start_2);


        verticalLayout_3->addLayout(horizontalLayout_3);

        btn_srh = new QPushButton(groupBox);
        btn_srh->setObjectName(QString::fromUtf8("btn_srh"));
        btn_srh->setMinimumSize(QSize(0, 28));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_srh->setIcon(icon);
        btn_srh->setIconSize(QSize(20, 20));

        verticalLayout_3->addWidget(btn_srh);

        btn_save = new QPushButton(groupBox);
        btn_save->setObjectName(QString::fromUtf8("btn_save"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/image/btn_save.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_save->setIcon(icon1);
        btn_save->setIconSize(QSize(20, 20));

        verticalLayout_3->addWidget(btn_save);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer);

        btn_to_dec = new QPushButton(groupBox);
        btn_to_dec->setObjectName(QString::fromUtf8("btn_to_dec"));
        btn_to_dec->setMinimumSize(QSize(0, 28));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/image/image_next.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_to_dec->setIcon(icon2);
        btn_to_dec->setIconSize(QSize(20, 20));

        verticalLayout_3->addWidget(btn_to_dec);


        horizontalLayout_4->addWidget(groupBox);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(page_playback);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy2);
        page_srh_ret = new QWidget();
        page_srh_ret->setObjectName(QString::fromUtf8("page_srh_ret"));
        verticalLayout_4 = new QVBoxLayout(page_srh_ret);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, 0, -1, -1);
        tableWidget = new QTableWidget(page_srh_ret);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout_4->addWidget(tableWidget);

        lab_play_tip = new QLabel(page_srh_ret);
        lab_play_tip->setObjectName(QString::fromUtf8("lab_play_tip"));

        verticalLayout_4->addWidget(lab_play_tip);

        stackedWidget->addWidget(page_srh_ret);
        page_play = new QWidget();
        page_play->setObjectName(QString::fromUtf8("page_play"));
        verticalLayout_5 = new QVBoxLayout(page_play);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        frame_play_window = new QFrame(page_play);
        frame_play_window->setObjectName(QString::fromUtf8("frame_play_window"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_play_window->sizePolicy().hasHeightForWidth());
        frame_play_window->setSizePolicy(sizePolicy3);
        frame_play_window->setFrameShape(QFrame::StyledPanel);
        frame_play_window->setFrameShadow(QFrame::Raised);

        verticalLayout_5->addWidget(frame_play_window);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 9, 0, 9);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_play = new QPushButton(page_play);
        btn_play->setObjectName(QString::fromUtf8("btn_play"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(btn_play->sizePolicy().hasHeightForWidth());
        btn_play->setSizePolicy(sizePolicy4);
        btn_play->setMinimumSize(QSize(60, 50));

        horizontalLayout->addWidget(btn_play);

        btn_next = new QPushButton(page_play);
        btn_next->setObjectName(QString::fromUtf8("btn_next"));
        sizePolicy4.setHeightForWidth(btn_next->sizePolicy().hasHeightForWidth());
        btn_next->setSizePolicy(sizePolicy4);
        btn_next->setMinimumSize(QSize(60, 50));

        horizontalLayout->addWidget(btn_next);

        btn_stop = new QPushButton(page_play);
        btn_stop->setObjectName(QString::fromUtf8("btn_stop"));
        sizePolicy4.setHeightForWidth(btn_stop->sizePolicy().hasHeightForWidth());
        btn_stop->setSizePolicy(sizePolicy4);
        btn_stop->setMinimumSize(QSize(60, 50));

        horizontalLayout->addWidget(btn_stop);

        btn_pre = new QPushButton(page_play);
        btn_pre->setObjectName(QString::fromUtf8("btn_pre"));
        sizePolicy4.setHeightForWidth(btn_pre->sizePolicy().hasHeightForWidth());
        btn_pre->setSizePolicy(sizePolicy4);
        btn_pre->setMinimumSize(QSize(60, 50));

        horizontalLayout->addWidget(btn_pre);

        comboBox_speed = new QComboBox(page_play);
        comboBox_speed->setObjectName(QString::fromUtf8("comboBox_speed"));

        horizontalLayout->addWidget(comboBox_speed);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalSlider = new QSlider(page_play);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalSlider);


        verticalLayout_5->addLayout(verticalLayout_2);

        stackedWidget->addWidget(page_play);

        verticalLayout->addWidget(stackedWidget);


        horizontalLayout_4->addLayout(verticalLayout);


        retranslateUi(page_playback);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(page_playback);
    } // setupUi

    void retranslateUi(QWidget *page_playback)
    {
        page_playback->setWindowTitle(QApplication::translate("page_playback", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        lab_srh_info->setText(QApplication::translate("page_playback", "<html><head/><body><p><span style=\" font-size:11pt;\">\350\257\267\345\205\210\351\200\211\345\256\232\344\270\200\344\270\252\351\200\232\351\201\223</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lab_srh_start->setText(QApplication::translate("page_playback", "\344\273\216 ", 0, QApplication::UnicodeUTF8));
        dateTimeEdit_start->setDisplayFormat(QApplication::translate("page_playback", "yyyy-M-d HH:mm:ss", 0, QApplication::UnicodeUTF8));
        lab_srh_end->setText(QApplication::translate("page_playback", "\345\210\260 ", 0, QApplication::UnicodeUTF8));
        dateTimeEdit_start_2->setDisplayFormat(QApplication::translate("page_playback", "yyyy-M-d HH:mm:ss", 0, QApplication::UnicodeUTF8));
        btn_srh->setText(QApplication::translate("page_playback", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        btn_save->setText(QApplication::translate("page_playback", "\345\244\207\344\273\275", 0, QApplication::UnicodeUTF8));
        btn_to_dec->setText(QApplication::translate("page_playback", "\345\217\221\351\200\201\350\207\263\350\247\243\347\240\201\345\231\250", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("page_playback", "\351\200\232\351\201\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("page_playback", "\345\274\200\345\247\213\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("page_playback", "\347\273\223\346\235\237\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        lab_play_tip->setText(QApplication::translate("page_playback", "\345\217\214\345\207\273\346\226\207\344\273\266\346\222\255\346\224\276", 0, QApplication::UnicodeUTF8));
        btn_play->setText(QApplication::translate("page_playback", "play", 0, QApplication::UnicodeUTF8));
        btn_next->setText(QApplication::translate("page_playback", "next", 0, QApplication::UnicodeUTF8));
        btn_stop->setText(QApplication::translate("page_playback", "stop", 0, QApplication::UnicodeUTF8));
        btn_pre->setText(QApplication::translate("page_playback", "pre", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class page_playback: public Ui_page_playback {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE_PLAYBACK_H
