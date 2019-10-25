BUILD_FOLDER               := $(shell pwd)/build

CONTROLLER_EXE             := map_controller
CONTROLLER_TEST_EXE        := map_controller_test
AGENT_EXE                  := map_agent
AGENT_TEST_EXE             := map_agent_test
HLE_ENTITY_EXE             := hle_entity

MAP_REINIT_EXE             := map_reinit
MAP_REINIT_SRC_MAIN        := src/map_reinit.c src/ini.c src/config_file_handler.c
MAP_REINIT_OBJ_MAIN        := $(patsubst src/%.c,$(BUILD_FOLDER)/%.o,$(MAP_REINIT_SRC_MAIN))

APMIB_INC     := $(shell pwd)/../boa/apmib/

CCFLAGS       := -Wall -Wextra -Werror -O2 -DRTK_MULTI_AP -I$(APMIB_INC) -I$(shell pwd)/include
CCFLAGS       += $(COMMON_CFLAGS) $(EXTRA_CFLAGS)

################################################################################
# Targets
################################################################################
.PHONY: all
all: clean $(MAP_REINIT_EXE)

$(MAP_REINIT_EXE): $(MAP_REINIT_OBJ_MAIN)
	$(CC) $(MAP_REINIT_OBJ_MAIN) -L$(APMIB_INC) -lapmib -o $@

$(BUILD_FOLDER)/%.o : src/%.c
	mkdir -p $(BUILD_FOLDER)
	$(CC) $(CCFLAGS) -c $(addprefix -I,$(EXTERNAL_INC)) $< -o $@

.PHONY: clean
clean:
	rm -f $(BUILD_FOLDER)/*.o
	rm -f $(MAP_REINIT_EXE)

.PHONY: romfs
romfs:
	$(ROMFSINST) $(CONTROLLER_EXE) /bin/map_controller
	$(ROMFSINST) $(CONTROLLER_TEST_EXE) /bin/map_controller_test
	$(ROMFSINST) $(AGENT_EXE) /bin/map_agent
	$(ROMFSINST) $(AGENT_TEST_EXE) /bin/map_agent_test
	$(ROMFSINST) $(HLE_ENTITY_EXE) /bin/hle_entity
	$(ROMFSINST) $(MAP_REINIT_EXE) /bin/map_reinit
	$(ROMFSINST) multiap.conf /etc/multiap.conf
	$(ROMFSINST) lib/libmultiap.so /lib
