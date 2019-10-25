#ifndef _MAP_INITIALIZATION_H_
#define _MAP_INITIALIZATION_H_

#include <stdint.h>

#define INIT_ERROR_OUT_OF_MEMORY      (1)
#define INIT_ERROR_INVALID_ARGUMENTS  (2)
#define INIT_ERROR_NO_INTERFACES      (3)
#define INIT_ERROR_INTERFACE_ERROR    (4)
#define INIT_ERROR_OS                 (5)
#define INIT_ERROR_PROTOCOL_EXTENSION (6)
#define INIT_ERROR_CONFIG_FILE        (7)

#define MULTI_AP_TEARDOWN_BIT      (0x10)
#define MULTI_AP_FRONTHAUL_BSS_BIT (0x20)
#define MULTI_AP_BACKHAUL_BSS_BIT  (0x40)
#define MULTI_AP_BACKHAUL_STA_BIT  (0x80)

#define MAP_CONFIG_2G                 (0)
#define MAP_CONFIG_5G                 (1)
//

struct bss_config_data {
	char *  ssid;
	char *  network_key;
	uint8_t network_type;
	//uint8_t al_id[6];
};

struct vendor_specific_data {
	uint8_t  vendor_oui[3];
	uint16_t payload_len;
	uint8_t  *payload;
};

struct radio_config_data {
	uint8_t                      radio_band;
	uint8_t                      bss_data_nr;
	struct bss_config_data *     bss_data;
	uint8_t                      vendor_data_nr;
	struct vendor_specific_data *vendor_datas;
};

struct device_info {
	uint8_t al_mac[6];
};

uint8_t map_init(const char *mq_name, uint8_t device_role,
                 uint8_t configure_state, uint8_t resend_time,
                 char *  device_name,
                 uint8_t verbose_level, uint8_t overwrite_pushbutton,
                 uint8_t config_data_nr, struct radio_config_data *config_data,
                 uint8_t self_listening, char *al_interfaces);

uint8_t map_set_controller(int as_controller);

struct device_info *map_get_device_information();

void map_exit();

#endif

