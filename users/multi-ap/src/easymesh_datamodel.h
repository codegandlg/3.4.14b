#ifndef _EASYMESH_DATAMODEL_H_
#define _EASYMESH_DATAMODEL_H_
#include <stdint.h>
#include "map_initialization.h"
//
#define RELOAD                        (0)
#define FULL_RELOAD                   (1)    //wlan_app
#define BAND_MIB_SET                  (2)
#define CONFIG_STATE_SET              (3)

#define EASYMESH_RADIO_2G                 (0)
#define EASYMESH_RADIO_5G                 (1)

struct easymesh_interface_mib {
	char *  ssid;
	char *  network_key;
	uint8_t network_type;
	uint8_t is_enabled; //1: enabled, 0: disabled
	uint8_t encrypt_type;
	uint8_t need_configure;
	uint8_t interface_index;
};

struct easymesh_radio_mib {
	uint8_t                        radio_band;
	uint8_t                        radio_channel;
	uint8_t						   channel_bandwidth;
	uint8_t                        need_change_channel;
	uint8_t                        interface_nr;
	char *                         repeater_ssid;
	struct easymesh_interface_mib *interface_mib;
};

struct easymesh_datamodel {
	//global
	uint16_t alme_port_number;
	uint8_t  max_resend_time;
	char *   device_name;
	uint8_t  rssi_weightage;
	uint8_t  path_weightage;
	uint8_t  cu_weightage;
	uint8_t  roam_score_difference;
	uint8_t  min_evaluation_interval;
	uint8_t  min_roam_interval;
	uint8_t  max_num_device_allowed;
	//mib
	uint8_t configured_band;
	uint8_t hw_reg_domain;
	uint8_t ap_mib_channel;

	uint8_t                    radio_data_nr; //radio number
	struct easymesh_radio_mib *radio_data;

	uint8_t                   config_data_nr; //controller autoconfig radio data number
	struct radio_config_data *per_radio_config;
};

#endif
