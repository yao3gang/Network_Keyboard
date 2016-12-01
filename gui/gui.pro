#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T12:32:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app

DEFINES = GUI_BOND_CLASS_DECLARE
PROJECT_PATH = /mnt/hgfs/vmshare/Network_Keyboard
INCLUDEPATH = $${PROJECT_PATH}/common/inc \
	$${PROJECT_PATH}/mid_logic/inc \
	$${PROJECT_PATH}/low_module/config/inc \
	$${PROJECT_PATH}/low_module/net/inc	\
	$${PROJECT_PATH}/hisi_lib/inc

LIBS = -lpthread -lrt
LIBS += -L$${PROJECT_PATH}/ext_libs -lcommon -lconfig -lnet -lz -lbiz -ltl_hslib_nvr

TL_APP_LIB_PATH = /home/hdb1/Hi3535_SDK_V1.0.5.0/mpp/lib
LIBS += -L$${TL_APP_LIB_PATH} -lhdmi -lmpi -ltde -lvqe -lVoiceEngine -ljpeg -lresampler

include($$PWD/input/input.pri)
include($$PWD/page/page.pri)
include($$PWD/custom_widget/custom_widget.pri)
include($$PWD/bond/bond.pri)

SOURCES += main.cpp

HEADERS  +=

FORMS +=

RESOURCES += \
    other/rc.qrc

OTHER_FILES +=
