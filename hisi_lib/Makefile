include ../Makefile.param

TARGET=libtl_hslib_nvr.so

INC_PATH += -I./inc
INC_PATH += $(TL_APP_INC_PATH)

SRC_DIR := ./src
OBJ_DIR := ./obj

CROSS=$(CCARCH)

CC=$(CROSS)gcc
AR=$(CROSS)ar

CFLAGS += -O2 -Wall -fno-strict-aliasing $(INC_PATH) -D$(HISIARCH)
#CFLAGS += -Wall $(INC_PATH) -D$(HISIARCH)

all:$(TARGET)

#*************************all source files***********************
OBJS :=	$(OBJ_DIR)/hisi_sys			\
		$(OBJ_DIR)/hisi_vdec	\
		$(OBJ_DIR)/hisi_vo		\
		$(OBJ_DIR)/hisi_fb
#****************************************************************

OBJS := $(foreach obj,$(OBJS),$(obj).o)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@
	
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $(OBJS)
	chmod 777 $(TARGET)
	install -D -m 644 $@ $(INSTALL_LIB_PATH)/$@
	cp -f $@ $(NFS_PATH)


.PHONY : clean
clean:
	rm -f $(OBJ_DIR)/* $(TARGET)
	rm -f $(INSTALL_LIB_PATH)/$(TARGET)

