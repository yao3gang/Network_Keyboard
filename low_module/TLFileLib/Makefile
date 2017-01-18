include ../../Makefile.param

AR := $(CCARCH)ar
CC := $(CCARCH)gcc
CPP := $(CCARCH)g++

LIB_TARGET := libtlfile.so

INC_DIR := ./inc ../../common/inc
SRC_DIR := ./src
OBJ_DIR := ./obj

#CFLAGS += -O2 -Wall -fno-strict-aliasing

CFLAGS += $(foreach inc,$(INC_DIR),-I$(inc))

ifdef MYDEFINE
	CFLAGS += -D$(MYDEFINE)
endif
ifdef MMUCHIP
	CFLAGS += -D$(MMUCHIP)
endif


OBJS :=	$(OBJ_DIR)/util		\
		$(OBJ_DIR)/custommp4	\
		$(OBJ_DIR)/TLFileLib	\
		$(OBJ_DIR)/avilib

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

LDFLAGS := -lpthread -lconfig -lz -ltlfile
$(OBJ_DIR)/test.o: test.c
	$(CPP) $(CFLAGS) -c $< -o $@

test:$(OBJ_DIR)/test.o
	$(CPP) $(CFLAGS) $< -o $@ -L$(INSTALL_LIB_PATH) $(LDFLAGS)
	cp -f test $(NFS_PATH)

.PHONY : clean clean_test	
clean :
	rm -rf $(LIB_TARGET) $(OBJ_DIR)/*
	
clean_test :
	rm test










