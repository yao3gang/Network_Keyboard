INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    page/page_main.h \
    page/page_config.h \
    page/wzd_tvwall_layout.h \
    page/page_dev_mgt.h \
    page/page_alm.h \
    page/page_tvwall.h \
    page/page_playback.h \
    page/dlg_dev_tree.h \
    page/page_preview.h \
    page/page_manager.h \
    page/page_main2.h \
    page/frmmessagebox.h \
    page/iconhelper.h \
    page/appdef.h \
    page/form_tvwall_config.h


SOURCES += \
    page/page_main.cpp \
    page/page_config.cpp \
    page/wzd_tvwall_layout.cpp \
    page/page_dev_mgt.cpp \
    page/page_alm.cpp \
    page/page_tvwall.cpp \
    page/page_playback.cpp \
    page/dlg_dev_tree.cpp \
    page/page_preview.cpp \
    page/page_manager.cpp \
    page/page_main2.cpp \
    page/frmmessagebox.cpp \
    page/iconhelper.cpp \
    page/form_tvwall_config.cpp

FORMS += \
    page/page_main.ui \
    page/page_config.ui \
    page/wzd_tvwall_layout.ui \
    page/page_dev_mgt.ui \
    page/page_alm.ui \
    page/page_tvwall.ui \
    page/page_playback.ui \
    page/dlg_dev_tree.ui \
    page/page_preview.ui \
    page/page_main2.ui \
    page/frmmessagebox.ui \
    page/form_tvwall_config.ui

RESOURCES += \
    other/rc.qrc
