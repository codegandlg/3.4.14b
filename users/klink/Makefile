ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../.config
else
include $(DIR_USERS)/.config
endif
KLINK_DIR=$(shell pwd)
COMMON_CFLAGS =

ifeq ($(CONFIG_APP_MULTI_AP),y)
COMMON_CFLAGS += -DRTK_MULTI_AP
COMMON_CFLAGS += -DUNIVERSAL_REPEATER
COMMON_CFLAGS += -DWIFI_SIMPLE_CONFIG
endif
COMMON_CFLAGS += -DCONFIG_RTL_8197F

CC = msdk-linux-gcc

APMIB_DIR = ../boa/apmib

CFLAGS := -s -Wall -Os
LDFLAGS := -s -Wall
IFLAGS := -I. -I$(APMIB_DIR)

TARGET = klinkM klinkS

all: $(TARGET)


klinkM: klinkMaster.o
	$(CC) klinkMaster.o $(LDFLAGS) $(COMMON_CFLAGS) -L$(APMIB_DIR) -lapmib  -o klinkM

klinkS: klinkSlave.o 
	$(CC) klinkSlave.o $(LDFLAGS) $(COMMON_CFLAGS) -L$(APMIB_DIR) -lapmib  -o klinkS

klinkSlave.o:klinkSlave.c
	$(CC) -c klinkSlave.c -o klinkSlave.o

klinkMaster.o:klinkMaster.c
	$(CC) -c klinkMaster.c -o klinkMaster.o
	
romfs:
	$(ROMFSINST) $(KLINK_DIR)/klinkM /bin/
	$(ROMFSINST) $(KLINK_DIR)/klinkS /bin/
	
.PHONY: clean

clean:
	rm -f *o
	rm -f $(TARGET)