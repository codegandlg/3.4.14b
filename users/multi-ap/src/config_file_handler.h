#ifndef _CONFIG_FILE_HANDLER_H_
#define _CONFIG_FILE_HANDLER_H_
#include <stdint.h>
#include <stdio.h>

#include "easymesh_datamodel.h"

int read_mib_config_file(void *user, const char *section, const char *name, const char *value);
int read_config_file(void *user, const char *section, const char *name, const char *value);
int write_config_file(struct easymesh_datamodel *data_container);

#endif