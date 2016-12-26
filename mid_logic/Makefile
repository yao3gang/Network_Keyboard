include ../Makefile.param

AR := $(CCARCH)ar
CC := $(CCARCH)gcc
CPP := $(CCARCH)g++

LIB_TARGET := libbiz.so

INC_DIR := ./inc ../gui/inc ../common/inc ../gui/bond ../low_module/config/inc ../low_module/net/inc ../hisi_lib/inc
SRC_DIR := ./src
OBJ_DIR := ./obj

CFLAGS += $(foreach inc,$(INC_DIR),-I$(inc))

#CFLAGS += -O2 -Wall -fno-strict-aliasing

ifdef MYDEFINE
	CFLAGS += -D$(MYDEFINE)
endif
ifdef MMUCHIP
	CFLAGS += -D$(MMUCHIP)
endif

OBJS :=	$(OBJ_DIR)/biz			\
		$(OBJ_DIR)/biz_config	\
		$(OBJ_DIR)/biz_net		\
		$(OBJ_DIR)/biz_device	\
		$(OBJ_DIR)/biz_system_complex	\
		$(OBJ_DIR)/biz_remote_stream	\
		$(OBJ_DIR)/biz_preview	\
		$(OBJ_DIR)/biz_playback

OBJS := $(foreach obj,$(OBJS),$(obj).o)

all:$(LIB_TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@
	
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CPP) $(CFLAGS) -fPIC -c $< -o $@

$(LIB_TARGET): $(OBJS)
	$(CPP) $(CFLAGS) -fPIC -shared -o $@ $(OBJS)
	install -D -m 644 $@ $(INSTALL_LIB_PATH)/$@
	cp -f $@ $(NFS_PATH)

LDFLAGS := -lpthread -lcommon -lbiz -lconfig -lz -lnet
$(OBJ_DIR)/test.o: test.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/test_cli.o: test_cli.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

test:$(OBJ_DIR)/test.o
	$(CPP) $(CFLAGS) $< -o $@ -L$(INSTALL_LIB_PATH) $(LDFLAGS)
	cp -f test $(NFS_PATH)

test_cli:$(OBJ_DIR)/test_cli.o
	$(CPP) $(CFLAGS) $< -o $@ -L$(INSTALL_LIB_PATH) $(LDFLAGS)
	cp -f test_cli $(NFS_PATH)

.PHONY : clean clean_test	
clean :
	rm -rf $(LIB_TARGET) $(OBJ_DIR)/*
	
clean_test :
	rm test test_cli










