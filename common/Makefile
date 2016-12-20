include ../Makefile.param

AR := $(CCARCH)ar
CC := $(CCARCH)gcc
CPP := $(CCARCH)g++

LIB_TARGET := libcommon.so

INC_DIR := ./inc
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


OBJS := 	$(OBJ_DIR)/api	\
			$(OBJ_DIR)/cmsgque \
			$(OBJ_DIR)/cpool_allocator \
			$(OBJ_DIR)/thread \
			$(OBJ_DIR)/cthread \
			$(OBJ_DIR)/cmutex \
			$(OBJ_DIR)/cspin_lock \
			$(OBJ_DIR)/ctimer \
			$(OBJ_DIR)/semaphore \
			$(OBJ_DIR)/crwlock	\
			$(OBJ_DIR)/ccircular_buffer	\
			$(OBJ_DIR)/ccircular_buffer_lockfree	\
			$(OBJ_DIR)/ccircular_buffer_lock

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

LDFLAGS := -lpthread -lcommon
$(OBJ_DIR)/test.o: test.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

test:$(OBJ_DIR)/test.o
	$(CPP) $(CFLAGS) $< -o $@ -L$(INSTALL_LIB_PATH) $(LDFLAGS)
	cp -f test $(NFS_PATH)

.PHONY : clean clean_test	
clean :
	rm -rf $(LIB_TARGET) $(OBJ_DIR)/*
	
clean_test :
	rm test










