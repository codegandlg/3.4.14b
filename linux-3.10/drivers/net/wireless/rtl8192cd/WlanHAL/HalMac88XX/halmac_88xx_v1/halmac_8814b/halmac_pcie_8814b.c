/******************************************************************************
 *
 * Copyright(c) 2016 - 2019 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 ******************************************************************************/

#include "halmac_pcie_8814b.h"
#include "halmac_pwr_seq_8814b.h"
#include "../halmac_init_88xx_v1.h"
#include "../halmac_common_88xx_v1.h"
#include "../halmac_efuse_88xx_v1.h"

#if (HALMAC_8814B_SUPPORT && HALMAC_PCIE_SUPPORT)

/**
 * mac_pwr_switch_pcie_8814b() - switch mac power
 * @adapter : the adapter of halmac
 * @pwr : power state
 * Author : KaiYuan Chang / Ivan Lin
 * Return : enum halmac_ret_status
 * More details of status code can be found in prototype document
 */
enum halmac_ret_status
mac_pwr_switch_pcie_8814b(struct halmac_adapter *adapter,
			  enum halmac_mac_power pwr)
{
	u8 value8;
	u8 rpwm;
	struct halmac_api *api = (struct halmac_api *)adapter->halmac_api;

	PLTFM_MSG_TRACE("[TRACE]%s ===>\n", __func__);
	PLTFM_MSG_TRACE("[TRACE]pwr = %x\n", pwr);
	PLTFM_MSG_TRACE("[TRACE]8814B pwr seq ver = %s\n",
			HALMAC_8814B_PWR_SEQ_VER);

	adapter->rpwm = HALMAC_REG_R8(REG_DBGSEL_PCIE_HRPWM1_HCPWM1_V1 + 1);

	/* Check FW still exist or not */
	if (HALMAC_REG_R16(REG_MCUFW_CTRL) == 0xC078) {
		/* Leave 32K */
		rpwm = (u8)((adapter->rpwm ^ BIT(7)) & 0x80);
		HALMAC_REG_W8(REG_DBGSEL_PCIE_HRPWM1_HCPWM1_V1 + 1, rpwm);
	}

	value8 = HALMAC_REG_R8(REG_CR);
	if (value8 == 0xEA)
		adapter->halmac_state.mac_pwr = HALMAC_MAC_POWER_OFF;
	else
		adapter->halmac_state.mac_pwr = HALMAC_MAC_POWER_ON;

	/* Check if power switch is needed */
	if (pwr == HALMAC_MAC_POWER_ON &&
	    adapter->halmac_state.mac_pwr == HALMAC_MAC_POWER_ON) {
		PLTFM_MSG_WARN("[WARN]power state unchange!!\n");
		return HALMAC_RET_PWR_UNCHANGE;
	}

	if (pwr == HALMAC_MAC_POWER_OFF) {
		if (pwr_seq_parser_88xx_v1(adapter, card_dis_flow_8814b) !=
		    HALMAC_RET_SUCCESS) {
			PLTFM_MSG_ERR("[ERR]Handle power off cmd error\n");
			return HALMAC_RET_POWER_OFF_FAIL;
		}

		adapter->halmac_state.mac_pwr = HALMAC_MAC_POWER_OFF;
		adapter->halmac_state.dlfw_state = HALMAC_DLFW_NONE;
		init_adapter_dynamic_param_88xx_v1(adapter);
	} else {
		if (pwr_seq_parser_88xx_v1(adapter, card_en_flow_8814b) !=
		    HALMAC_RET_SUCCESS) {
			PLTFM_MSG_ERR("[ERR]Handle power on cmd error\n");
			return HALMAC_RET_POWER_ON_FAIL;
		}

		adapter->halmac_state.mac_pwr = HALMAC_MAC_POWER_ON;
	}

	PLTFM_MSG_TRACE("[TRACE]%s <===\n", __func__);

	return HALMAC_RET_SUCCESS;
}

/**
 * halmac_pcie_switch_8814b() - pcie gen1/gen2 switch
 * @adapter : the adapter of halmac
 * @cfg : gen1/gen2 selection
 * Author : KaiYuan Chang / Ivan Lin
 * Return : enum halmac_ret_status
 * More details of status code can be found in prototype document
 */
enum halmac_ret_status
pcie_switch_8814b(struct halmac_adapter *adapter, enum halmac_pcie_cfg cfg)
{
	return HALMAC_RET_NOT_SUPPORT;
}

/**
 * phy_cfg_pcie_8814b() - phy config
 * @adapter : the adapter of halmac
 * Author : KaiYuan Chang / Ivan Lin
 * Return : enum halmac_ret_status
 * More details of status code can be found in prototype document
 */
enum halmac_ret_status
phy_cfg_pcie_8814b(struct halmac_adapter *adapter,
		   enum halmac_intf_phy_platform pltfm)
{
	return HALMAC_RET_SUCCESS;
}

/**
 * intf_tun_pcie_8814b() - pcie interface fine tuning
 * @adapter : the adapter of halmac
 * Author : Rick Liu
 * Return : enum halmac_ret_status
 * More details of status code can be found in prototype document
 */
enum halmac_ret_status
intf_tun_pcie_8814b(struct halmac_adapter *adapter)
{
	return HALMAC_RET_SUCCESS;
}

/**
 * cfgspc_set_pcie_8814b() - pcie configuration space setting
 * @adapter : the adapter of halmac
 * Author : Rick Liu
 * Return : enum halmac_ret_status
 * More details of status code can be found in prototype document
 */
enum halmac_ret_status
cfgspc_set_pcie_8814b(struct halmac_adapter *adapter,
		      struct halmac_pcie_cfgspc_param *param)
{
	return HALMAC_RET_NOT_SUPPORT;
}

#endif /* HALMAC_8814B_SUPPORT */