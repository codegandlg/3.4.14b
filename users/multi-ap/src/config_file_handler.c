#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "config_file_handler.h"
#include "easymesh_datamodel.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t _atoi(char *s, int base)
{
	uint8_t k    = 0;
	int     sign = 1;
	if (NULL == s) {
		return 0;
	}

	k = 0;
	if (base == 10) {
		if (*s == '-') {
			sign = -1;
			s++;
		}
		while (*s != '\0' && *s >= '0' && *s <= '9') {
			k = 10 * k + (*s - '0');
			s++;
		}
		k *= sign;
	} else {
		while (*s != '\0') {
			uint8_t v;
			if (*s >= '0' && *s <= '9')
				v = *s - '0';
			else if (*s >= 'a' && *s <= 'f')
				v = *s - 'a' + 10;
			else if (*s >= 'A' && *s <= 'F')
				v = *s - 'A' + 10;
			else {
				printf("error hex format!\n");
#if 1
				return k;
#else
				return 0;
#endif
			}
			k = 16 * k + v;
			s++;
		}
	}
	return k;
}
void _writeToConfig(FILE *fp, unsigned char config_nr, char **config_array)
{
	int i;
	for (i = 0; i < config_nr; i++) {
		fprintf(fp, "%s", config_array[i]);
		//free(config_array[i]);
		if (i < (config_nr - 1)) {
			fprintf(fp, ",");
		} else {
			fprintf(fp, "\n");
		}
	}
}

void _writeOuiToConfig(FILE *fp, unsigned char config_nr, unsigned char **config_array)
{
	int i;
	for (i = 0; i < config_nr; i++) {
		fprintf(fp, "%02x%02x%02x", config_array[i][0], config_array[i][1], config_array[i][2]);
		//free(config_array[i]);
		if (i < (config_nr - 1)) {
			fprintf(fp, ",");
		} else {
			fprintf(fp, "\n");
		}
	}
}

void _writeToConfig_dec(FILE *fp, unsigned char config_nr, unsigned char *config_array)
{
	int i;
	for (i = 0; i < config_nr; i++) {
		fprintf(fp, "%d", config_array[i]);
		if (i < (config_nr - 1)) {
			fprintf(fp, ",");
		} else {
			fprintf(fp, "\n");
		}
	}
}

int read_config_file(void *user, const char *section, const char *name,
                     const char *value)
{
	struct easymesh_datamodel *      pconfig    = (struct easymesh_datamodel *)user;
	static struct radio_config_data *_5g_config = NULL;
	static struct radio_config_data *_2g_config = NULL;
	int                              i          = 0;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("global", "alme_port_number")) {
		pconfig->alme_port_number = atoi(value);
		printf("[CONFIG] Multi-AP alme port: %d\n", pconfig->alme_port_number);
	} else if (MATCH("global", "max_resend_time")) {
		pconfig->max_resend_time = atoi(value);
		printf("[CONFIG] Max resend time for the message that doesn't acknowledged in time is %d\n", pconfig->max_resend_time);
	} else if (MATCH("global", "rssi_weightage")) {
		pconfig->rssi_weightage = atoi(value);
		printf("[CONFIG] Rssi weightage is %d\n", pconfig->rssi_weightage);
	} else if (MATCH("global", "path_weightage")) {
		pconfig->path_weightage = atoi(value);
		printf("[CONFIG] Path weightage is %d\n", pconfig->path_weightage);
	} else if (MATCH("global", "cu_weightage")) {
		pconfig->cu_weightage = atoi(value);
		printf("[CONFIG] Cu weightage is %d\n", pconfig->cu_weightage);
	} else if (MATCH("global", "roam_score_difference")) {
		pconfig->roam_score_difference = atoi(value);
		printf("[CONFIG] Roam score difference is %d\n", pconfig->roam_score_difference);
	} else if (MATCH("global", "min_evaluation_interval")) {
		pconfig->min_evaluation_interval = atoi(value);
		printf("[CONFIG] Min evaluation interval is %d\n", pconfig->min_evaluation_interval);
	} else if (MATCH("global", "min_roam_interval")) {
		pconfig->min_roam_interval = atoi(value);
		printf("[CONFIG] Min roam interval is %d\n", pconfig->min_roam_interval);
	} else if (MATCH("global", "device_name")) {
		pconfig->device_name = strdup(value);
		printf("[CONFIG] Device name set by user is %s\n", pconfig->device_name);
	} else if (MATCH("global", "max_num_device_allowed")) {
		pconfig->max_num_device_allowed = atoi(value);
		printf("[CONFIG] Max number of device allowed is %d\n", pconfig->max_num_device_allowed);

	} else if (MATCH("5g_config_data", "number")) {
		pconfig->config_data_nr += 1;
		pconfig->per_radio_config = (struct radio_config_data *)realloc(pconfig->per_radio_config, pconfig->config_data_nr * sizeof(struct radio_config_data));
		printf("[CONFIG] 5g_config_data number : %d\n", atoi(value));
		_5g_config                 = &(pconfig->per_radio_config[pconfig->config_data_nr - 1]);
		_5g_config->bss_data_nr    = atoi(value);
		_5g_config->radio_band     = EASYMESH_RADIO_5G;
		_5g_config->vendor_data_nr = 0;
		_5g_config->vendor_datas   = NULL;
		_5g_config->bss_data       = (struct bss_config_data *)malloc(_5g_config->bss_data_nr * sizeof(struct bss_config_data));
	} else if (MATCH("5g_config_data", "ssid")) {

		// // parse the interfaces and its role
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->bss_data[str_num].ssid = strdup(token);
			printf("[CONFIG] 5G ssid %d: %s\n", str_num, _5g_config->bss_data[str_num].ssid);
			str_num += 1;
		}
	} else if (MATCH("5g_config_data", "network_key")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->bss_data[str_num].network_key = strdup(token);
			printf("[CONFIG] 5G Network key %d: %s\n", str_num, _5g_config->bss_data[str_num].network_key);
			str_num += 1;
		}
	} else if (MATCH("5g_config_data", "network_type")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			printf("[CONFIG] 5G Network type %d: %s\n", str_num, token);
			_5g_config->bss_data[str_num].network_type = atoi(token);

			str_num += 1;
		}
	} else if (MATCH("2.4g_config_data", "number")) {
		pconfig->config_data_nr += 1;
		pconfig->per_radio_config = (struct radio_config_data *)realloc(pconfig->per_radio_config, (pconfig->config_data_nr) * sizeof(struct radio_config_data));
		_2g_config                = &(pconfig->per_radio_config[pconfig->config_data_nr - 1]);
		_2g_config->bss_data_nr   = atoi(value);
		_2g_config->radio_band    = EASYMESH_RADIO_2G;
		printf("[CONFIG] 24g_config_data number : %d\n", atoi(value));
		_2g_config->vendor_data_nr = 0;
		_2g_config->vendor_datas   = NULL;
		_2g_config->bss_data       = (struct bss_config_data *)malloc(_2g_config->bss_data_nr * sizeof(struct bss_config_data));
	} else if (MATCH("2.4g_config_data", "ssid")) {

		// // parse the interfaces and its role
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->bss_data[str_num].ssid = strdup(token);
			printf("[CONFIG] 24G ssid %d: %s\n", str_num, _2g_config->bss_data[str_num].ssid);
			str_num += 1;
		}
	} else if (MATCH("2.4g_config_data", "network_key")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->bss_data[str_num].network_key = strdup(token);
			printf("[CONFIG] 24G Network key %d: %s\n", str_num, _2g_config->bss_data[str_num].network_key);
			str_num += 1;
		}
	} else if (MATCH("2.4g_config_data", "network_type")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			printf("[CONFIG] 24G Network type %d: %s\n", str_num, token);
			_2g_config->bss_data[str_num].network_type = atoi(token);

			str_num += 1;
		}
	} else if (MATCH("vendor_data_2.4g", "vendor_data_nr")) {
		_2g_config->vendor_data_nr = atoi(value);
		_2g_config->vendor_datas   = (struct vendor_specific_data *)malloc(_2g_config->vendor_data_nr * sizeof(struct vendor_specific_data));
		printf("[CONFIG] 2G vendor data number is: %d\n", _2g_config->vendor_data_nr);
	} else if (MATCH("vendor_data_2.4g", "vendor_payload")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			printf("[CONFIG] 2G vendor config data payload %d: %s\n", str_num, token);
			_2g_config->vendor_datas[str_num].payload                                                = (uint8_t *)strdup(token);
			_2g_config->vendor_datas[str_num].payload_len                                            = (uint16_t)strlen(token);
			_2g_config->vendor_datas[str_num].payload[_2g_config->vendor_datas[str_num].payload_len] = '\0';
			_2g_config->vendor_datas[str_num].payload_len += 1;
			str_num += 1;
		}
	} else if (MATCH("vendor_data_2.4g", "vendor_oui")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;

			for (i = 0; i < 3; i++) {
				char oui_B[3] = { '\0' };
				memcpy(oui_B, token + i * 2, 2);
				_2g_config->vendor_datas[str_num].vendor_oui[i] = _atoi(oui_B, 16);
			}
			printf("[CONFIG] 2G vendor config data oui %d: %02x%02x%02x\n", str_num, _2g_config->vendor_datas[str_num].vendor_oui[0], _2g_config->vendor_datas[str_num].vendor_oui[1], _2g_config->vendor_datas[str_num].vendor_oui[2]);
			str_num += 1;
		}
	} else if (MATCH("vendor_data_5g", "vendor_data_nr")) {
		_5g_config->vendor_data_nr = atoi(value);
		_5g_config->vendor_datas   = (struct vendor_specific_data *)malloc(_5g_config->vendor_data_nr * sizeof(struct vendor_specific_data));
		printf("[CONFIG] 5G vendor data number is: %d\n", _5g_config->vendor_data_nr);
	} else if (MATCH("vendor_data_5g", "vendor_payload")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			printf("[CONFIG] 5G vendor config data payload %d: %s\n", str_num, token);
			_5g_config->vendor_datas[str_num].payload                                                = (uint8_t *)strdup(token);
			_5g_config->vendor_datas[str_num].payload_len                                            = (uint16_t)strlen(token);
			_5g_config->vendor_datas[str_num].payload[_5g_config->vendor_datas[str_num].payload_len] = '\0';
			_5g_config->vendor_datas[str_num].payload_len += 1;
			str_num += 1;
		}
	} else if (MATCH("vendor_data_5g", "vendor_oui")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			for (i = 0; i < 3; i++) {
				char oui_B[3] = { '\0' };
				memcpy(oui_B, token + i * 2, 2);
				_5g_config->vendor_datas[str_num].vendor_oui[i] = _atoi(oui_B, 16);
			}
			printf("[CONFIG] 5G vendor config data oui %d: %02x%02x%02x\n", str_num, _5g_config->vendor_datas[str_num].vendor_oui[0], _5g_config->vendor_datas[str_num].vendor_oui[1], _5g_config->vendor_datas[str_num].vendor_oui[2]);
			str_num += 1;
		}
	} else {
		return 0; /* unknown section/name, error */
	}
	return 1;
}

int read_mib_config_file(void *user, const char *section, const char *name, const char *value)
{
	struct easymesh_datamodel *       pconfig    = (struct easymesh_datamodel *)user;
	static struct easymesh_radio_mib *_5g_config = NULL;
	static struct easymesh_radio_mib *_2g_config = NULL;
	static struct radio_config_data *_5g_vendor_data = NULL;
	static struct radio_config_data *_2g_vendor_data = NULL;
	int                              i               = 0;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("global", "configured_band")) {
		pconfig->configured_band = atoi(value);
		printf("[CONFIG] Configured band is %d\n", pconfig->configured_band);
	} else if (MATCH("global", "hw_reg_domain")) {
		pconfig->hw_reg_domain = atoi(value);
		printf("[CONFIG] HW redion domain is %d\n", pconfig->hw_reg_domain);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	else if (MATCH("mib_info_5g", "channel")) {
		pconfig->radio_data_nr += 1;
		pconfig->radio_data             = (struct easymesh_radio_mib *)realloc(pconfig->radio_data, pconfig->radio_data_nr * sizeof(struct easymesh_radio_mib));
		_5g_config                      = &(pconfig->radio_data[pconfig->radio_data_nr - 1]);
		_5g_config->radio_channel       = atoi(value);
		_5g_config->need_change_channel = 0;
		_5g_config->radio_band          = EASYMESH_RADIO_5G;
		printf("[CONFIG] agent 5g channel is %d\n", _5g_config->radio_channel);

	} else if (MATCH("mib_info_5g", "channel_bandwidth")) {
		_5g_config->channel_bandwidth = atoi(value);
		printf("[CONFIG] agent 5g channel bandwidth is %d\n", _5g_config->channel_bandwidth);

	} else if (MATCH("mib_info_5g", "repeater_ssid")) {
		_5g_config->repeater_ssid = strdup(value);
		printf("[CONFIG] agent 5g repeater ssid is %s\n", _5g_config->repeater_ssid);

	} else if (MATCH("mib_info_5g", "bss_number")) {
		_5g_config->interface_nr = atoi(value);
		printf("[CONFIG] 5g_bss_data number : %d\n", atoi(value));
		_5g_config->interface_mib = (struct easymesh_interface_mib *)malloc(_5g_config->interface_nr * sizeof(struct easymesh_interface_mib));

	} else if (MATCH("mib_info_5g", "ssid")) {
		// // parse the interfaces and its role
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].ssid = strdup(token);
			printf("[CONFIG] 5G ssid %d: %s\n", str_num, _5g_config->interface_mib[str_num].ssid);
			str_num += 1;
		}
	} else if (MATCH("mib_info_5g", "network_key")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].network_key = strdup(token);
			printf("[CONFIG] 5G Network key %d: %s\n", str_num, _5g_config->interface_mib[str_num].network_key);
			str_num += 1;
		}
	} else if (MATCH("mib_info_5g", "is_enabled")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].is_enabled = atoi(token);
			printf("[CONFIG] agent 5G status %d: %d\n", str_num, _5g_config->interface_mib[str_num].is_enabled);
			str_num += 1;
		}
	} else if (MATCH("mib_info_5g", "encrypt_type")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].encrypt_type = atoi(token);
			printf("[CONFIG] agent 5G encrypt_type %d: %d\n", str_num, _5g_config->interface_mib[str_num].encrypt_type);
			str_num += 1;
		}
	} else if (MATCH("mib_info_2.4g", "channel")) {
		pconfig->radio_data_nr += 1;
		pconfig->radio_data             = (struct easymesh_radio_mib *)realloc(pconfig->radio_data, pconfig->radio_data_nr * sizeof(struct easymesh_radio_mib));
		_2g_config                      = &(pconfig->radio_data[pconfig->radio_data_nr - 1]);
		_2g_config->radio_channel       = atoi(value);
		_2g_config->need_change_channel = 0;
		_2g_config->radio_band          = EASYMESH_RADIO_2G;
		printf("[CONFIG] agent 2.4g channel is %d\n", _2g_config->radio_channel);

	} else if (MATCH("mib_info_2.4g", "channel_bandwidth")) {
		_2g_config->channel_bandwidth = atoi(value);
		printf("[CONFIG] agent 2.4g channel bandwidth is %d\n", _2g_config->channel_bandwidth);

	} else if (MATCH("mib_info_2.4g", "repeater_ssid")) {
		_2g_config->repeater_ssid = strdup(value);
		printf("[CONFIG] agent 2.4g repeater ssid is %s\n", _2g_config->repeater_ssid);

	} else if (MATCH("mib_info_2.4g", "bss_number")) {
		_2g_config->interface_nr = atoi(value);
		printf("[CONFIG] 24g_bss_data number : %d\n", atoi(value));
		_2g_config->interface_mib = (struct easymesh_interface_mib *)malloc(_2g_config->interface_nr * sizeof(struct easymesh_interface_mib));

	} else if (MATCH("mib_info_2.4g", "ssid")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].ssid = strdup(token);
			printf("[CONFIG] 24G ssid %d: %s\n", str_num, _2g_config->interface_mib[str_num].ssid);
			str_num += 1;
		}
	} else if (MATCH("mib_info_2.4g", "network_key")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].network_key = strdup(token);
			printf("[CONFIG] 24G Network key %d: %s\n", str_num, _2g_config->interface_mib[str_num].network_key);
			str_num += 1;
		}
	} else if (MATCH("mib_info_2.4g", "is_enabled")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].is_enabled = atoi(token);
			printf("[CONFIG] agent 2G status %d: %d\n", str_num, _2g_config->interface_mib[str_num].is_enabled);
			str_num += 1;
		}
	} else if (MATCH("mib_info_2.4g", "encrypt_type")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].encrypt_type = atoi(token);
			printf("[CONFIG] agent 2G encrypt_type %d: %d\n", str_num, _2g_config->interface_mib[str_num].encrypt_type);
			str_num += 1;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (MATCH("setmib_global_data", "configured_band")) {
		pconfig->configured_band = atoi(value);
		printf("[CONFIG] Configured band is %d\n", pconfig->configured_band);
	} else if (MATCH("setmib_global_data", "hw_reg_domain")) {
		pconfig->hw_reg_domain = atoi(value);
		printf("[CONFIG] HW redion domain is %d\n", pconfig->hw_reg_domain);
	} else if (MATCH("5g_setmib_config_data", "radio_channel")) {
		pconfig->radio_data_nr += 1;
		pconfig->config_data_nr += 1;
		pconfig->radio_data             = (struct easymesh_radio_mib *)realloc(pconfig->radio_data, pconfig->radio_data_nr * sizeof(struct easymesh_radio_mib));
		pconfig->per_radio_config       = (struct radio_config_data *)realloc(pconfig->per_radio_config, pconfig->config_data_nr * sizeof(struct radio_config_data));
		_5g_config                      = &(pconfig->radio_data[pconfig->radio_data_nr - 1]);
		_5g_vendor_data                 = &(pconfig->per_radio_config[pconfig->config_data_nr - 1]);
		_5g_vendor_data->vendor_data_nr = 0;
		_5g_vendor_data->vendor_datas   = NULL;
		_5g_config->radio_channel       = atoi(value);
		_5g_config->radio_band          = EASYMESH_RADIO_5G;
		printf("[CONFIG] agent 5g channel is %d\n", _5g_config->radio_channel);
	} else if (MATCH("5g_setmib_config_data", "channel_changed")) {
		_5g_config->need_change_channel = atoi(value);
	} else if (MATCH("5g_setmib_config_data", "repeater_ssid")) {
		_5g_config->repeater_ssid = strdup(value);
		printf("[CONFIG] agent 5g repeater ssid is %s\n", _5g_config->repeater_ssid);
	} else if (MATCH("5g_setmib_config_data", "number")) {
		_5g_config->interface_nr = atoi(value);
		printf("[CONFIG] 5g_config_data number : %d\n", atoi(value));
		_5g_config->interface_mib = (struct easymesh_interface_mib *)malloc(_5g_config->interface_nr * sizeof(struct easymesh_interface_mib));
		for (i = 0; i < _5g_config->interface_nr; i++) {
			_5g_config->interface_mib[i].interface_index = i;
		}
	} else if (MATCH("5g_setmib_config_data", "ssid")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].ssid = strdup(token);
			printf("[CONFIG] 5G ssid %d: %s\n", str_num, _5g_config->interface_mib[str_num].ssid);
			str_num += 1;
		}
	} else if (MATCH("5g_setmib_config_data", "network_key")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].network_key = strdup(token);
			printf("[CONFIG] 5G Network key %d: %s\n", str_num, _5g_config->interface_mib[str_num].network_key);
			str_num += 1;
		}
	} else if (MATCH("5g_setmib_config_data", "is_enabled")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].is_enabled = atoi(token);
			printf("[CONFIG] agent 5G is_enabled %d: %d\n", str_num, _5g_config->interface_mib[str_num].is_enabled);
			str_num += 1;
		}
	} else if (MATCH("5g_setmib_config_data", "need_configure")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].need_configure = atoi(token);
			printf("[CONFIG] agent 5G need_configure %d: %d\n", str_num, _5g_config->interface_mib[str_num].need_configure);
			str_num += 1;
		}
	} else if (MATCH("5g_setmib_config_data", "bss_type")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].network_type = atoi(token);
			printf("[CONFIG] agent 5G bss_type %d: %d\n", str_num, _5g_config->interface_mib[str_num].network_type);
			str_num += 1;
		}
	} else if (MATCH("5g_setmib_config_data", "encrypt_type")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_config->interface_mib[str_num].encrypt_type = atoi(token);
			printf("[CONFIG] agent 5G encrypt_type %d: %d\n", str_num, _5g_config->interface_mib[str_num].encrypt_type);
			str_num += 1;
		}
	}

	else if (MATCH("2.4g_setmib_config_data", "radio_channel")) {
		pconfig->radio_data_nr += 1;
		pconfig->config_data_nr += 1;
		pconfig->radio_data             = (struct easymesh_radio_mib *)realloc(pconfig->radio_data, pconfig->radio_data_nr * sizeof(struct easymesh_radio_mib));
		pconfig->per_radio_config       = (struct radio_config_data *)realloc(pconfig->per_radio_config, pconfig->config_data_nr * sizeof(struct radio_config_data));
		_2g_config                      = &(pconfig->radio_data[pconfig->radio_data_nr - 1]);
		_2g_vendor_data                 = &(pconfig->per_radio_config[pconfig->config_data_nr - 1]);
		_2g_vendor_data->vendor_data_nr = 0;
		_2g_vendor_data->vendor_datas   = NULL;
		_2g_config->radio_channel       = atoi(value);
		_2g_config->radio_band          = EASYMESH_RADIO_2G;
		printf("[CONFIG] agent 2.4g channel is %d\n", _2g_config->radio_channel);
	} else if (MATCH("2.4g_setmib_config_data", "channel_changed")) {
		_2g_config->need_change_channel = atoi(value);
	} else if (MATCH("2.4g_setmib_config_data", "repeater_ssid")) {
		_2g_config->repeater_ssid = strdup(value);
		printf("[CONFIG] agent 2.4g repeater ssid is %s\n", _2g_config->repeater_ssid);
	} else if (MATCH("2.4g_setmib_config_data", "number")) {
		_2g_config->interface_nr = atoi(value);
		printf("[CONFIG] 24g_config_data number : %d\n", atoi(value));
		_2g_config->interface_mib = (struct easymesh_interface_mib *)malloc(_2g_config->interface_nr * sizeof(struct easymesh_interface_mib));
		for (i = 0; i < _2g_config->interface_nr; i++) {
			_2g_config->interface_mib[i].interface_index = i;
		}
	} else if (MATCH("2.4g_setmib_config_data", "ssid")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].ssid = strdup(token);
			printf("[CONFIG] 24G ssid %d: %s\n", str_num, _2g_config->interface_mib[str_num].ssid);
			str_num += 1;
		}
	} else if (MATCH("2.4g_setmib_config_data", "network_key")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].network_key = strdup(token);
			printf("[CONFIG] 24G Network key %d: %s\n", str_num, _2g_config->interface_mib[str_num].network_key);
			str_num += 1;
		}
	} else if (MATCH("2.4g_setmib_config_data", "is_enabled")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].is_enabled = atoi(token);
			printf("[CONFIG] agent 2G is_enabled %d: %d\n", str_num, _2g_config->interface_mib[str_num].is_enabled);
			str_num += 1;
		}
	} else if (MATCH("2.4g_setmib_config_data", "need_configure")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].need_configure = atoi(token);
			printf("[CONFIG] agent 2G need_configure %d: %d\n", str_num, _2g_config->interface_mib[str_num].need_configure);
			str_num += 1;
		}
	} else if (MATCH("2.4g_setmib_config_data", "bss_type")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].network_type = atoi(token);
			printf("[CONFIG] agent 2G bss_type %d: %d\n", str_num, _2g_config->interface_mib[str_num].network_type);
			str_num += 1;
		}
	} else if (MATCH("2.4g_setmib_config_data", "encrypt_type")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_config->interface_mib[str_num].encrypt_type = atoi(token);
			printf("[CONFIG] agent 2G encrypt_type %d: %d\n", str_num, _2g_config->interface_mib[str_num].encrypt_type);
			str_num += 1;
		}
	} else if (MATCH("vendor_data_2.4g", "vendor_data_nr")) {
		_2g_vendor_data->vendor_data_nr = atoi(value);
		_2g_vendor_data->vendor_datas   = (struct vendor_specific_data *)malloc(_2g_vendor_data->vendor_data_nr * sizeof(struct vendor_specific_data));
		printf("[CONFIG] 2G vendor setmib data number is: %d\n", _2g_vendor_data->vendor_data_nr);
	} else if (MATCH("vendor_data_2.4g", "vendor_load_len")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_vendor_data->vendor_datas[str_num].payload_len = atoi(token);
			printf("[CONFIG] 2G vendor payload data len is: %d\n", _2g_vendor_data->vendor_datas[str_num].payload_len);
			str_num += 1;
		}
	} else if (MATCH("vendor_data_2.4g", "vendor_payload")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_2g_vendor_data->vendor_datas[str_num].payload                                                     = (uint8_t *)strdup(token);
			_2g_vendor_data->vendor_datas[str_num].payload[_2g_vendor_data->vendor_datas[str_num].payload_len] = '\0';
			printf("[CONFIG] 2G vendor payload data %d: %s\n", str_num, _2g_vendor_data->vendor_datas[str_num].payload);
			str_num += 1;
		}
	} else if (MATCH("vendor_data_2.4g", "vendor_oui")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;

			for (i = 0; i < 3; i++) {
				char oui_B[3] = { '\0' };
				memcpy(oui_B, token + i * 2, 2);
				_2g_vendor_data->vendor_datas[str_num].vendor_oui[i] = _atoi(oui_B, 16);
			}
			printf("[CONFIG] 2G vendor config data oui %d: %02x%02x%02x\n", str_num, _2g_vendor_data->vendor_datas[str_num].vendor_oui[0], _2g_vendor_data->vendor_datas[str_num].vendor_oui[1], _2g_vendor_data->vendor_datas[str_num].vendor_oui[2]);
			str_num += 1;
		}
	} else if (MATCH("vendor_data_5g", "vendor_data_nr")) {
		_5g_vendor_data->vendor_data_nr = atoi(value);
		_5g_vendor_data->vendor_datas   = (struct vendor_specific_data *)malloc(_5g_vendor_data->vendor_data_nr * sizeof(struct vendor_specific_data));
		printf("[CONFIG] 5G vendor setmib data number is: %d\n", _5g_vendor_data->vendor_data_nr);
	} else if (MATCH("vendor_data_5g", "vendor_load_len")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_vendor_data->vendor_datas[str_num].payload_len = atoi(token);
			printf("[CONFIG] 5G vendor payload data len is: %d\n", _5g_vendor_data->vendor_datas[str_num].payload_len);
			str_num += 1;
		}
	} else if (MATCH("vendor_data_5g", "vendor_payload")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			_5g_vendor_data->vendor_datas[str_num].payload                                                     = (uint8_t *)strdup(token);
			_5g_vendor_data->vendor_datas[str_num].payload[_5g_vendor_data->vendor_datas[str_num].payload_len] = '\0';
			printf("[CONFIG] 5G vendor payload data %d: %s\n", str_num, _5g_vendor_data->vendor_datas[str_num].payload);
			str_num += 1;
		}
	} else if (MATCH("vendor_data_5g", "vendor_oui")) {
		char *  token;
		char *  saveptr;
		uint8_t str_num = 0;
		for (token = (char *)value;; token = NULL) {
			token = strtok_r(token, ",", &saveptr);
			if (NULL == token)
				break;
			for (i = 0; i < 3; i++) {
				char oui_B[3] = { '\0' };
				memcpy(oui_B, token + i * 2, 2);
				_5g_vendor_data->vendor_datas[str_num].vendor_oui[i] = _atoi(oui_B, 16);
			}
			printf("[CONFIG] 5G vendor config data oui %d: %02x%02x%02x\n", str_num, _5g_vendor_data->vendor_datas[str_num].vendor_oui[0], _5g_vendor_data->vendor_datas[str_num].vendor_oui[1], _5g_vendor_data->vendor_datas[str_num].vendor_oui[2]);
			str_num += 1;
		}
	} else {
		return 0; /* unknown section/name, error */
	}
	return 1;
}

uint8_t _write_mib_data(FILE *fp, const char *radio_name, struct easymesh_radio_mib *radio_mib)
{
	int i = 0;

	if (NULL == radio_name || NULL == radio_mib) {
		return 1;
	}

	fprintf(fp, "[%s_setmib_config_data]\n", radio_name);
	fprintf(fp, "radio_channel = %d\n", radio_mib->radio_channel);
	fprintf(fp, "channel_changed = %d\n", radio_mib->need_change_channel);

	unsigned char  config_number  = 0;
	char **        ssids          = (char **)malloc(1 * sizeof(char *));
	char **        network_keys   = (char **)malloc(1 * sizeof(char *));
	unsigned char *is_enabled     = (unsigned char *)malloc(1 * sizeof(unsigned char));
	unsigned char *need_configure = (unsigned char *)malloc(1 * sizeof(unsigned char));
	unsigned char *bss_type       = (unsigned char *)malloc(1 * sizeof(unsigned char));
	unsigned char *encrypt_type   = (unsigned char *)malloc(1 * sizeof(unsigned char));

	for (i = 0; i < radio_mib->interface_nr; i++) {
		config_number++;
		ssids                             = (char **)realloc(ssids, (config_number) * sizeof(char *));
		network_keys                      = (char **)realloc(network_keys, (config_number) * sizeof(char *));
		is_enabled                        = (unsigned char *)realloc(is_enabled, (config_number) * sizeof(unsigned char));
		bss_type                          = (unsigned char *)realloc(bss_type, (config_number) * sizeof(unsigned char));
		ssids[config_number - 1]          = strdup(radio_mib->interface_mib[i].ssid);
		network_keys[config_number - 1]   = strdup(radio_mib->interface_mib[i].network_key);
		is_enabled[config_number - 1]     = radio_mib->interface_mib[i].is_enabled;
		need_configure[config_number - 1] = radio_mib->interface_mib[i].need_configure;
		bss_type[config_number - 1]       = radio_mib->interface_mib[i].network_type;
		encrypt_type[config_number - 1]   = radio_mib->interface_mib[i].encrypt_type;
	}
	fprintf(fp, "number = %d\n", config_number);

	fprintf(fp, "ssid = ");
	_writeToConfig(fp, config_number, ssids);

	fprintf(fp, "network_key = ");
	_writeToConfig(fp, config_number, network_keys);
	fprintf(fp, "is_enabled = ");
	_writeToConfig_dec(fp, config_number, is_enabled);
	fprintf(fp, "need_configure = ");
	_writeToConfig_dec(fp, config_number, need_configure);
	fprintf(fp, "bss_type = ");
	_writeToConfig_dec(fp, config_number, bss_type);
	fprintf(fp, "encrypt_type = ");
	_writeToConfig_dec(fp, config_number, encrypt_type);

	free(ssids);
	free(network_keys);
	free(is_enabled);
	free(need_configure);
	free(bss_type);
	free(encrypt_type);

	return 0;
}

uint8_t _write_vendor_data(FILE *fp, const char *radio_name, struct radio_config_data *radio_config)
{
	int i = 0;

	if (NULL == radio_name || NULL == radio_config) {
		return 1;
	}

	if (radio_config->vendor_data_nr) {
		unsigned char * load_len   = (unsigned char *)malloc(radio_config->vendor_data_nr * sizeof(unsigned char));
		char **         payload    = (char **)malloc(radio_config->vendor_data_nr * sizeof(char *));
		unsigned char **vendor_oui = (unsigned char **)malloc(radio_config->vendor_data_nr * sizeof(unsigned char *));
		for (i = 0; i < radio_config->vendor_data_nr; i++) {
			load_len[i] = radio_config->vendor_datas[i].payload_len;
			payload[i] = strdup((char *)radio_config->vendor_datas[i].payload);
			vendor_oui[i] = (uint8_t *)malloc(3 * sizeof(uint8_t));
			memcpy(vendor_oui[i], radio_config->vendor_datas[i].vendor_oui, 3);
			printf("[%s_%d]check vendor OUI: %02x%02x%02x\n\n\n", __FUNCTION__, __LINE__, vendor_oui[i][0], vendor_oui[i][1], vendor_oui[i][2]); //qw
		}

		fprintf(fp, "[vendor_data_%s]\n", radio_name);

		fprintf(fp, "vendor_data_nr = %d\n", radio_config->vendor_data_nr);

		fprintf(fp, "vendor_load_len = ");
		_writeToConfig_dec(fp, radio_config->vendor_data_nr, load_len);

		fprintf(fp, "vendor_payload = ");
		_writeToConfig(fp, radio_config->vendor_data_nr, payload);

		fprintf(fp, "vendor_oui = ");
		_writeOuiToConfig(fp, radio_config->vendor_data_nr, vendor_oui);

		free(payload);
		free(vendor_oui);
		free(load_len);
	}

	return 0;
}

int write_config_file(struct easymesh_datamodel *data_container)
{
	struct easymesh_radio_mib *_5g_setmib = NULL;
	struct easymesh_radio_mib *_2g_setmib = NULL;

	struct radio_config_data *_5g_config = NULL;
	struct radio_config_data *_2g_config = NULL;
	int                       i;

	for (i = 0; i < data_container->radio_data_nr; i++) {
		if (EASYMESH_RADIO_2G == data_container->radio_data[i].radio_band) {
			_2g_setmib = &(data_container->radio_data[i]);
		} else if (EASYMESH_RADIO_5G == data_container->radio_data[i].radio_band) {
			_5g_setmib = &(data_container->radio_data[i]);
		}
	}

	for (i = 0; i < data_container->config_data_nr; i++) {
		if (MAP_CONFIG_2G == data_container->per_radio_config[i].radio_band) {
			_2g_config = &(data_container->per_radio_config[i]);
		} else if (MAP_CONFIG_5G == data_container->per_radio_config[i].radio_band) {
			_5g_config = &(data_container->per_radio_config[i]);
		}
	}

	FILE *fp = fopen("/var/multiap_mib.conf", "w");
	if (fp == NULL) {
		printf("Error opening config file!\n");
		return 0;
	}

	fprintf(fp, "[global]\n");
	fprintf(fp, "alme_port_number = %d\n", data_container->alme_port_number);
	fprintf(fp, "max_resend_time = %d\n", data_container->max_resend_time);
	fprintf(fp, "rssi_weightage = %d\n", data_container->rssi_weightage);
	fprintf(fp, "path_weightage = %d\n", data_container->path_weightage);
	fprintf(fp, "cu_weightage = %d\n", data_container->cu_weightage);
	fprintf(fp, "roam_score_difference = %d\n", data_container->roam_score_difference);
	fprintf(fp, "min_evaluation_interval = %d\n", data_container->min_evaluation_interval);
	fprintf(fp, "min_roam_interval = %d\n", data_container->min_roam_interval);
	fprintf(fp, "device_name = %s\n", data_container->device_name);
	fprintf(fp, "max_num_device_allowed = %d\n", data_container->max_num_device_allowed);

	fprintf(fp, "[setmib_global_data]\n");
	fprintf(fp, "configured_band = %d\n", data_container->configured_band);
	fprintf(fp, "hw_reg_domain = %d\n", data_container->hw_reg_domain);

	if (_5g_setmib && _5g_setmib->interface_nr) {
		_write_mib_data(fp, "5g", _5g_setmib);
	}

	if (_5g_config && _5g_config->vendor_data_nr) {
		_write_vendor_data(fp, "5g", _5g_config);
	}

	if (_2g_setmib && _2g_setmib->interface_nr) {
		_write_mib_data(fp, "2.4g", _2g_setmib);
	}

	if (_2g_config && _2g_config->vendor_data_nr) {
		_write_vendor_data(fp, "2.4g", _2g_config);
	}

	fclose(fp);

	return 0;
}
