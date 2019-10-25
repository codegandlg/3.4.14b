ifeq ($(wildcard $(DIR_LINUX)/.config),)
DIR_LINUX_KERNEL = $(DIR_ROOT)/linux-2.6.30
else
DIR_LINUX_KERNEL = $(DIR_LINUX)
endif

CRYPTLIB = portingssl
WSCSRC = src

all:
	make -C $(CRYPTLIB)

	@echo "" > ./.diff_config; 

	@if [ -e .kernel_config ]; then \
		diff .kernel_config $(DIR_LINUX_KERNEL)/.config > .diff_config; \
		if [ -s .diff_config ]; then \
		        cp $(DIR_LINUX_KERNEL)/.config .kernel_config; \
			make -C $(WSCSRC) clean; \
                fi; \
        else \
                cp $(DIR_LINUX_KERNEL)/.config ./.kernel_config; \
	fi;

	make -C $(WSCSRC)
	
clean:
	make -C $(CRYPTLIB) clean
	make -C $(WSCSRC) clean

romfs:
	$(ROMFSINST) $(WSCSRC)/wscd /bin/wscd
	$(ROMFSINST) $(WSCSRC)/wscd.conf /etc/wscd.conf
