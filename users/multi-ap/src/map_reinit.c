#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>          // errno
#include <sys/ioctl.h>      // ioctl()
#include <sys/socket.h>     // socket()
#include <linux/wireless.h> // struct iwreq
#include <unistd.h>         // close()

#include "config_file_handler.h"
#include "apmib.h"
#include "ini.h"
#include "easymesh_datamodel.h"
#include "map_initialization.h"

uint8_t _is_interface_up(char *interface)
{
	int          skfd = 0;
	struct ifreq ifr;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);

	strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);

	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		close(skfd);
		return 0;
	}
	close(skfd);
	return (uint8_t) !!(ifr.ifr_flags & IFF_UP);
}

uint8_t _set_band_mib(int band, int channel)
{
	int w_idx  = wlan_idx;
	int vw_idx = vwlan_idx;
	if (!apmib_init()) {
		printf("[ERROR] Initialize AP MIB failed in set_configure_state!\n");
		return 1;
	}
	if (0 == band) {
		// 2g
		int channel_int = (int)channel;
		wlan_idx        = 1;
		vwlan_idx       = 0;
		apmib_set(MIB_WLAN_CHANNEL, (void *)&channel_int);
		// set band width for some specific channels
		if (14 == channel) {
			int tmp = 0;
			apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&tmp);
		}
		apmib_update(CURRENT_SETTING);
	} else if (1 == band) {
		// 5g
		int channel_int = (int)channel;
		wlan_idx        = 0;
		vwlan_idx       = 0;
		apmib_set(MIB_WLAN_CHANNEL, (void *)&channel_int);
		// set band width for some specific channels
		if (116 == channel || 136 == channel || 140 == channel || 165 == channel) {
			int tmp = 0;
			apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&tmp);
		}
		apmib_update(CURRENT_SETTING);
	} else {
		// error
		printf("Error Setting Band in agent_main in APMIB - Invalid band..\n");
		return 1;
	}
	wlan_idx  = w_idx;
	vwlan_idx = vw_idx;
	return 0;
}
uint8_t _set_configure_state(uint8_t configure_state)
{
	int configured_state_int = (int)configure_state;
	if (!apmib_init()) {
		printf("[ERROR] Initialize AP MIB failed in set_configure_state!\n");
		return 1;
	}
	if (!apmib_set(MIB_MAP_CONFIGURED_BAND, (void *)&configured_state_int)) {
		printf("[ERROR] Cannot set configured band.\n");
		return 0;
	}
	apmib_update(CURRENT_SETTING);
	return 0;
}

uint8_t _set_ap_mib(uint8_t radio_idx, struct easymesh_interface_mib interface_mib)
{
	int  val;
	char buffer[100];

	if (radio_idx > 1) {
		// Only support wlan0 and wlan1
		printf("Unknown radio index %d\n", radio_idx);
		return 1;
	}

	if (!apmib_init()) {
		printf("[ERROR] Initialize AP MIB failed in apply_80211_configuration!\n");
		return 0;
	}

	wlan_idx  = radio_idx;
	vwlan_idx = interface_mib.interface_index;

	if (0 == interface_mib.is_enabled) {
		apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&val);
		if (val)
			return 0;
		else {
			val = 1;
			if (!apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&val)) {
				return 0;
			}
			apmib_update(CURRENT_SETTING);
			return 0;
		}
	}

	// interface enable
	apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&val);
	if (val) {
		val = 0;
		if (!apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&val)) {
			return 0;
		}
	}
	apmib_get(MIB_WLAN_SSID, (void *)buffer);
	if (0 == strcmp(interface_mib.ssid, buffer)) {
		apmib_get(MIB_WLAN_WPA_PSK, (void *)buffer);
		if (0 == strcmp(interface_mib.network_key, buffer)) {

			char interface_name[] = "wlan0-va0";
			if (interface_mib.interface_index) {
				interface_name[8] = '0' + interface_mib.interface_index - 1;
			} else {
				interface_name[5] = '\0';
			}
			printf("Setting is identical for interface %s, skip configuration\n", interface_name);

			return 0;
		}
	}
	// Set WLAN MODE
	val = AP_MODE;
	if (!apmib_set(MIB_WLAN_MODE, (void *)&val)) {
		return 0;
	}
	// Set SSID

	if (!apmib_set(MIB_WLAN_SSID, (void *)interface_mib.ssid)) {
		return 0;
	}
	// set bss_type
	int bss_type_int = (int)interface_mib.network_type;
	if (!apmib_set(MIB_WLAN_MAP_BSS_TYPE, (void *)&bss_type_int)) {
		return 0;
	}
	// HIDDEN SSID
	if (MULTI_AP_BACKHAUL_BSS_BIT == bss_type_int) {
		val = 1;
		if (!apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&val)) {
			return 0;
		}
	}
	//
	val = ENCRYPT_WPA2;
	if (!apmib_set(MIB_WLAN_ENCRYPT, (void *)&val)) {
		return 0;
	}
	//
	val = WPA_AUTH_PSK;
	if (!apmib_set(MIB_WLAN_WPA_AUTH, (void *)&val)) {
		return 0;
	}
	//
	val = WPA_CIPHER_AES;
	if (!apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&val)) {
		return 0;
	}
	//
	val = 0; // PSK_FORMAT_PASSPHRASE
	if (!apmib_set(MIB_WLAN_PSK_FORMAT, (void *)&val)) {
		return 0;
	}
	//
	if (!apmib_set(MIB_WLAN_WPA_PSK, (void *)interface_mib.network_key)) {
		return 0;
	}
	//
	val = WSC_AUTH_WPA2PSK;
	if (!apmib_set(MIB_WLAN_WSC_AUTH, (void *)&val)) {
		return 0;
	}
	//
	val = WSC_ENCRYPT_AES;
	if (!apmib_set(MIB_WLAN_WSC_ENC, (void *)&val)) {
		return 0;
	}
	//
	if (!apmib_set(MIB_WLAN_WSC_PSK, (void *)interface_mib.network_key)) {
		return 0;
	}
	//
	val = 1;
	if (!apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&val)) {
		return 0;
	}
	apmib_update(CURRENT_SETTING);
	return 1;
}

uint8_t _set_vxd_mib(uint8_t radio_idx, struct easymesh_interface_mib interface_mib)
{
	// set vxd mib
	int   val;
	char  buffer[100];

	if (radio_idx > 1) {
		// Only support wlan0 and wlan1
		printf("Unknown radio index %d\n", radio_idx);
		return 1;
	}

	if (!apmib_init()) {
		printf("[ERROR] Initialize AP MIB failed in apply_80211_configuration!\n");
		return 0;
	}

	wlan_idx = radio_idx;

	apmib_get(MIB_WLAN_SSID, (void *)buffer);
	if (0 == strcmp(interface_mib.ssid, buffer)) {

		if (0 == wlan_idx) {
			apmib_get(MIB_REPEATER_SSID1, (void *)buffer);
		} else if (1 == wlan_idx) {
			apmib_get(MIB_REPEATER_SSID2, (void *)buffer);
		}
		if (0 == strcmp(interface_mib.ssid, buffer)) {
			apmib_get(MIB_WLAN_WPA_PSK, (void *)buffer);
			if (0 == strcmp(interface_mib.network_key, buffer)) {
				printf("Setting is identical for interface wlan%d-vxd, skip configuration\n", wlan_idx);
				return 2;
			}
		}
	}
	// Set WLAN MODE
	val = CLIENT_MODE;
	if (!apmib_set(MIB_WLAN_MODE, (void *)&val)) {
		return 0;
	}
	// Set SSID
	if (!apmib_set(MIB_WLAN_SSID, (void *)interface_mib.ssid)) {
		return 0;
	}
	if (0 == wlan_idx) {
		if (!apmib_set(MIB_REPEATER_SSID1, (void *)interface_mib.ssid)) {
			return 0;
		}
	} else if (1 == wlan_idx) {
		if (!apmib_set(MIB_REPEATER_SSID2, (void *)interface_mib.ssid)) {
			return 0;
		}
	}
	if (!apmib_set(MIB_WLAN_WSC_SSID, (void *)interface_mib.ssid)) {
		return 0;
	}

	// Set BSS Type
	int bss_type_int = MULTI_AP_BACKHAUL_STA_BIT;
	if (!apmib_set(MIB_WLAN_MAP_BSS_TYPE, (void *)&bss_type_int)) {
		return 0;
	}

	val = ENCRYPT_WPA2;

	if (!apmib_set(MIB_WLAN_ENCRYPT, (void *)&val)) {
		return 0;
	}

	val = WPA_AUTH_PSK;

	if (!apmib_set(MIB_WLAN_WPA_AUTH, (void *)&val)) {
		return 0;
	}

	val = WPA_CIPHER_AES;

	if (!apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&val)) {
		return 0;
	}

	if (!apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&val)) {
		return 0;
	}

	val = 0; // PSK_FORMAT_PASSPHRASE
	if (!apmib_set(MIB_WLAN_PSK_FORMAT, (void *)&val)) {
		return 0;
	}

	if (!apmib_set(MIB_WLAN_WPA_PSK, (void *)interface_mib.network_key)) {
		return 0;
	}

	val = WSC_AUTH_WPA2PSK;
	if (!apmib_set(MIB_WLAN_WSC_AUTH, (void *)&val)) {
		return 0;
	}

	val = WSC_ENCRYPT_AES;
	if (!apmib_set(MIB_WLAN_WSC_ENC, (void *)&val)) {
		return 0;
	}
	// buffer =
	// strdup(data_container.radio_data[wlan_idx].bss_data[vwlan_idx].network_key);
	if (!apmib_set(MIB_WLAN_WSC_PSK, (void *)interface_mib.network_key)) {
		return 0;
	}

	val = 1;
	if (!apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&val)) {
		return 0;
	}
	apmib_update(CURRENT_SETTING);
	return 1;
}

uint8_t _reload_setting(char *interfaces_name, uint8_t is_enabled)
{
	char cmd[128];

	if (NULL == strstr(interfaces_name, "wlan")) {
		return 1;
	}

	if (0 == _is_interface_up(interfaces_name) && !is_enabled) {
		return 2;
	}

	if(!is_enabled) {
		sprintf(cmd, "brctl delif br0 %s; ifconfig %s down", interfaces_name, interfaces_name);
		system(cmd);
		return 0;
	}

	sprintf(cmd, "brctl delif br0 %s; flash set_mib %s", interfaces_name,
	        interfaces_name);
	printf("%s\n", cmd);
	system(cmd);

	if (strstr(interfaces_name, "va")) {
		char root_interface[] = "wlan0";
		root_interface[4]     = interfaces_name[4];
		sprintf(cmd, "ifconfig %s up && brctl addif br0 %s", root_interface, root_interface);
		printf("%s\n", cmd);
		system(cmd);
	}
	sprintf(cmd, "ifconfig %s up && brctl addif br0 %s", interfaces_name, interfaces_name);
	printf("%s\n", cmd);
	system(cmd);


	return 0;
}

uint8_t _set_radio_band_width(uint8_t radio_band, uint8_t channel)
{
	int channel_int;
	// 20MHz
	int bandwidth = 0;

	int old_wlan_idx  = wlan_idx;
	int old_vwlan_idx = vwlan_idx;

	if (MAP_CONFIG_5G == radio_band) {
		// 5g
		wlan_idx  = 0;
		vwlan_idx = 0;

		channel_int = (int)channel;
		apmib_set(MIB_WLAN_CHANNEL, (void *)&channel_int);
		// set band width for some specific channels
		if (116 == channel_int || 136 == channel_int || 140 == channel_int || 165 == channel_int) {
			apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&bandwidth);
		}

	} else if (MAP_CONFIG_2G == radio_band) {
		// 2g
		wlan_idx  = 1;
		vwlan_idx = 0;

		channel_int = (int)channel;
		apmib_set(MIB_WLAN_CHANNEL, (void *)&channel_int);
		// set band width for some specific channels
		if (14 == channel_int) {
			apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&bandwidth);
		}
	} else {
		return 1;
	}

	wlan_idx  = old_wlan_idx;
	vwlan_idx = old_vwlan_idx;

	return 0;
}

uint8_t _easymesh_set_mib()
{
	int                      i, j;
	struct easymesh_datamodel easymesh_db;
	easymesh_db.configured_band = 0;
	easymesh_db.radio_data_nr   = 0;
	easymesh_db.radio_data  = NULL;
	easymesh_db.per_radio_config = NULL;

	if (ini_parse("/var/multiap_mib.conf", read_mib_config_file, &easymesh_db) < 0) {
		printf("[RTK] Can't load configuration file!! \n");
		return INIT_ERROR_CONFIG_FILE;
	}
	if (!apmib_init()) {
		printf("[ERROR] Initialize AP MIB failed in apply_80211_configuration!\n");
		return 0;
	}

	// set configure_state
	int configured_state_int = (int)easymesh_db.configured_band;
	if (!apmib_set(MIB_MAP_CONFIGURED_BAND, (void *)&configured_state_int)) {
		printf("[ERROR] Cannot set configured band.\n");
		return 0;
	}

	// set channel & bonding

	// set mib
	int  radio_band, interface_index;
	char interface_name[10] = "wlan0-va0\0";

	for (i = 0; i < easymesh_db.radio_data_nr; i++) {
		radio_band = easymesh_db.radio_data[i].radio_band;
		if (MAP_CONFIG_2G == radio_band) {
			wlan_idx          = 1;
			interface_name[4] = '1';

		} else if (MAP_CONFIG_5G == radio_band) {
			wlan_idx          = 0;
			interface_name[4] = '0';
		} else {
			printf("unsupported band!\n");
			continue;
		}

		if (easymesh_db.radio_data[i].need_change_channel) {
			_set_radio_band_width(radio_band, easymesh_db.radio_data[i].radio_channel);
		}

		for (j = 0; j < easymesh_db.radio_data[i].interface_nr; j++) {
			interface_index = easymesh_db.radio_data[i].interface_mib[j].interface_index;

			if (NUM_VWLAN_INTERFACE == interface_index) {
				vwlan_idx = 5;

				if (1 == easymesh_db.radio_data[i].interface_mib[j].need_configure) {
					_set_vxd_mib(wlan_idx, easymesh_db.radio_data[i].interface_mib[j]);
					//
					interface_name[5] = '-';
					interface_name[7] = 'x';
					interface_name[8] = 'd';
					_reload_setting(interface_name, easymesh_db.radio_data[i].interface_mib[j].is_enabled);
				}
			} else {
				vwlan_idx = j;
				if (1 == easymesh_db.radio_data[i].interface_mib[j].need_configure) {
					_set_ap_mib(wlan_idx, easymesh_db.radio_data[i].interface_mib[j]);
					//
					if (0 == j) {
						interface_name[5] = '\0';
					} else {
						interface_name[5] = '-';
						interface_name[8] = j - 1 + '0';
					}
					_reload_setting(interface_name,  easymesh_db.radio_data[i].interface_mib[j].is_enabled);
				}
			}
		}
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int        c;
	int        reinit = 0;
	static int band, cur_channel;

	while ((c = getopt(argc, argv, "flb:c")) != -1) {
		switch (c) {
		case 'f': {
			reinit = FULL_RELOAD;
			break;
		}
		case 'l': {
			reinit = RELOAD;
			break;
		}
		case 'b': {
			char *token, *saveptr;
			int * band_str = NULL;
			int   str_num = 0, i = 0;
			for (token = (char *)optarg;; token = NULL) {
				token = strtok_r(token, "_", &saveptr);
				if (NULL == token)
					break;
				i++;
				band_str          = (int *)realloc(band_str, i * sizeof(int));
				band_str[str_num] = atoi(token);
				str_num += 1;
			}
			if (2 == str_num) {
				band        = band_str[0];
				cur_channel = band_str[1];
			} else {
				printf("wrong cmd for map_reinit daemon!!!!\n");
				return 0;
			}
			reinit = BAND_MIB_SET;
			free(band_str);
			break;
		}
		case 'c': {
			reinit = CONFIG_STATE_SET;
			break;
		}
		}
	}

	if (FULL_RELOAD == reinit) {
		_easymesh_set_mib();
		printf("\nNeed full reload!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		system("sysconf init gw all");
	} else if (RELOAD == reinit) {
		_easymesh_set_mib();
		printf("\nOnly need reload!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		system("sysconf init ap wlan_app; killall map_agent; map_agent -vvd > /dev/null");
	} else if (BAND_MIB_SET == reinit) {
		_set_band_mib(band, cur_channel);
	} else if (CONFIG_STATE_SET == reinit) {
		_set_configure_state(0);
	}
	return 0;
}
