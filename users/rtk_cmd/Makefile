#dzh begin
include $(DIR_LINUX)/.config
#dzh end

all: check_modify	rtk_cmd
#CC	= mips-uclibc-gcc
#STRIP	= mips-uclibc-strip
include $(DIR_LINUX)/.config

ifeq ($(KERNEL_VER),linux-2.6.30)
CFLAGS	= -Wall -Os -I$(DIR_LINUX)/include/
LDFLAGS = -Wall -Os -I$(DIR_LINUX)/include/
CFLAGS += -DKERNEL_2_6_30
endif
ifeq ($(KERNEL_VER),linux-3.10)
CFLAGS	= -Wall -Os -I$(DIR_LINUX)/include/uapi
LDFLAGS = -Wall -Os -I$(DIR_LINUX)/include/uapi
CFLAGS += -DKERNEL_3_10
endif
IFLAGS	=
#LDFLAGS = -Wall -Os -I$(DIR_LINUX)/include/
SOURCES = rtk_cmd.c rtk_filter.c rtk_igmp_delete.c rtk_fastbridge.c rtk_hw_qos.c rtk_83xx_hw_qos.c

ifdef CONFIG_RTL_FAST_FILTER
	CFLAGS += -D CONFIG_RTL_FAST_FILTER
endif

ifeq ($(CONFIG_RTL_HW_QOS_SUPPORT),y)
CFLAGS += -DCONFIG_RTL_HW_QOS_SUPPORT
endif

ifeq ($(CONFIG_RTL_83XX_QOS_SUPPORT),y)
CFLAGS += -DCONFIG_RTL_83XX_QOS_SUPPORT
endif

ifeq ($(SLINK),1)
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS += --static -Wl,--gc-sections
endif


DEBUG	= -Wall -Os

check_modify:
				@echo "" > ./.diff_config; 
				@if [ -e .kernel_config ]; then \
								diff .kernel_config $(DIR_LINUX)/.config > .diff_config; \
								if [ -s .diff_config ]; then \
												cp $(DIR_LINUX)/.config .kernel_config; \
												make clean; \
								fi; \
				else \
								cp $(DIR_LINUX)/.config ./.kernel_config; \
				fi; 
rtk_cmd:       $(SOURCES:.c=.o)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	$(STRIP) $@
romfs:
	$(ROMFSINST) /bin/rtk_cmd

clean:
	rm -f rtk_cmd *.o 

