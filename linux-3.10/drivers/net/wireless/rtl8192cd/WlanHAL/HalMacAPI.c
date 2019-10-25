
/*++
Copyright (c) Realtek Semiconductor Corp. All rights reserved.

Module Name:
	HalMacAPI.c
	
Abstract:
	Define MAC function support 
	for Driver
	    
Major Change History:
	When       Who               What
	---------- ---------------   -------------------------------
	2015-07-07 Eric            Create.	
--*/

#include "HalPrecomp.h"

#ifdef CONFIG_WLAN_MACHAL_API
void associate_halMac_API(struct rtl8192cd_priv *priv)
{

    priv->pshare->use_macHalAPI = 1;
    priv->pHalmac_platform_api = (struct halmac_platform_api *)kmalloc(sizeof(struct halmac_platform_api), GFP_ATOMIC);
    //priv->pHalmac_adapter      = (PHALMAC_ADAPTER)kmalloc(sizeof(HALMAC_ADAPTER), GFP_ATOMIC);
    //priv->pHalmac_api          = (PHALMAC_API)kmalloc(sizeof(HALMAC_API), GFP_ATOMIC);   

    struct halmac_platform_api *              pMacHalFunc = priv->pHalmac_platform_api;


    //
    //Initialization Related
    //
#ifdef CONFIG_SDIO_HCI
    pMacHalFunc->SDIO_CMD52_READ            = HALAPI_SDIO_CMD52_READ;
    pMacHalFunc->SDIO_CMD53_READ_8          = HALAPI_SDIO_CMD53_READ_8;
    pMacHalFunc->SDIO_CMD53_READ_16         = HALAPI_SDIO_CMD53_READ_16;
    pMacHalFunc->SDIO_CMD53_READ_32         = HALAPI_SDIO_CMD53_READ_32;
    pMacHalFunc->SDIO_CMD53_READ_N          = MacHalGeneralDummy;
    pMacHalFunc->SDIO_CMD52_WRITE           = HALAPI_SDIO_CMD52_WRITE;
    pMacHalFunc->SDIO_CMD53_WRITE_8         = HALAPI_SDIO_CMD53_WRITE_8;
    pMacHalFunc->SDIO_CMD53_WRITE_16        = HALAPI_SDIO_CMD53_WRITE_16;
    pMacHalFunc->SDIO_CMD53_WRITE_32        = HALAPI_SDIO_CMD53_WRITE_32;
#endif
    pMacHalFunc->REG_READ_8                 = HALAPI_R8;
    pMacHalFunc->REG_READ_16                = HALAPI_R16;
    pMacHalFunc->REG_READ_32                = HALAPI_R32;
    pMacHalFunc->REG_WRITE_8                = HALAPI_W8;
    pMacHalFunc->REG_WRITE_16               = HALAPI_W16;
    pMacHalFunc->REG_WRITE_32               = HALAPI_W32;
    pMacHalFunc->SEND_RSVD_PAGE             = HAL_SEND_RSVD_PAGE;
#ifdef WLAN_SUPPORT_H2C_PACKET    
    pMacHalFunc->SEND_H2C_PKT               = HAL_SEND_H2C_PKT;
#endif
    pMacHalFunc->RTL_MALLOC                 = HALAPI_MALLOC;
    pMacHalFunc->RTL_FREE                   = HALAPI_FREE;
    pMacHalFunc->RTL_MEMCPY                 = HALAPI_MEMCPY;
    pMacHalFunc->RTL_MEMSET                 = HALAPI_MEMSET;
    pMacHalFunc->RTL_DELAY_US               = HALAPI_DELAY_US;
//    pMacHalFunc->SIGNAL_INIT                = MacHalGeneralDummy;
//    pMacHalFunc->SIGNAL_DEINIT              = MacHalGeneralDummy;
//    pMacHalFunc->SIGNAL_WAIT                = MacHalGeneralDummy;
//    pMacHalFunc->SIGNAL_SET                 = MacHalGeneralDummy;
    pMacHalFunc->MSG_PRINT                  = HALAPI_PRINT;
    pMacHalFunc->MUTEX_INIT                 = HALAPI_MUTEX_INIT;
    pMacHalFunc->MUTEX_DEINIT               = HALAPI_MUTEX_DEINIT;
    pMacHalFunc->MUTEX_LOCK                 = HALAPI_MUTEX_LOCK;
    pMacHalFunc->MUTEX_UNLOCK               = HALAPI_MUTEX_UNLOCK;
    pMacHalFunc->EVENT_INDICATION           = HALAPI_EVENT_INDICATION;
}

void disassociate_halMac_API(struct rtl8192cd_priv *priv)
{
    if (priv->pHalmac_platform_api != NULL) {
		kfree(priv->pHalmac_platform_api);
    }
 
    priv->pshare->use_macHalAPI = 0;
}

#ifdef WLAN_SUPPORT_H2C_PACKET    
u8 HAL_SEND_H2C_PKT(
    IN VOID *pDriver_adapter, 
    IN u8 *pBuf, 
    IN u32 size
)
{
#if 1
    //TX_DESC_DATA_88XX               desc_data;
    PHCI_TX_DMA_QUEUE_STRUCT_88XX   cur_q;
	PHCI_TX_DMA_MANAGER_88XX        ptx_dma;
    PTX_BUFFER_DESCRIPTOR           cur_txbd;    
    HAL_PADAPTER    pAdapter =      (HAL_PADAPTER)pDriver_adapter;
    pu1Byte                         cur_h2c_buffer;
    u2Byte                          host_wrtie,hw_read;
    u4Byte                          DMA_QUEUE_CMD;	

#if (IS_EXIST_RTL8197FEM || IS_EXIST_RTL8822BE || IS_EXIST_RTL8197GEM)
    if(IS_HARDWARE_TYPE_8197F(Adapter) || IS_HARDWARE_TYPE_8822B(Adapter)) {    
        DMA_QUEUE_CMD = HCI_TX_DMA_QUEUE_CMD;  
    }
#endif  //(IS_EXIST_RTL8197FEM || IS_EXIST_RTL8822BE || IS_EXIST_RTL8197GEM)

#if IS_EXIST_RTL8198FEM
    if ( IS_HARDWARE_TYPE_8198F(Adapter) ) {
        DMA_QUEUE_CMD = HCI_TX_DMA_QUEUE_CMD_V1; 
    }
#endif  //IS_EXIST_RTL8198FEM	

    PH2C_PAYLOAD_88XX   h2c_buf_start = (PH2C_PAYLOAD_88XX)_GET_HAL_DATA(pAdapter)->h2c_buf;
    ptx_dma     = (PHCI_TX_DMA_MANAGER_88XX)(_GET_HAL_DATA(pAdapter)->PTxDMA88XX);
    cur_q       = &(ptx_dma->tx_queue[DMA_QUEUE_CMD]);
    cur_txbd    = cur_q->pTXBD_head + cur_q->host_idx;
    
    cur_h2c_buffer = &(h2c_buf_start[cur_q->host_idx]);

    //memset(&desc_data, 0, sizeof(TX_DESC_DATA_88XX));
    hw_read = GetTxQueueHWIdx88XX(pDriver_adapter,DMA_QUEUE_CMD);
    host_wrtie = cur_q->host_idx;

    //printk("h2c send packet \n");
    if(CIRC_SPACE_RTK(host_wrtie,hw_read,H2C_ENTERY_MAX_NUM) < 1)
    {
        printk("h2c page full \n");
        return FALSE;
    }

    memcpy(cur_h2c_buffer,pBuf,size);
    // 1. Fill desc_data
    //desc_data.hdrLen = size;
    //desc_data.pHdr = pBuf;
    // 2. FillTXDESC
    //FillH2CTxDesc88XX(pDriver_adapter, HCI_TX_DMA_QUEUE_CMD);

    // 3. SetTXBufferDesc
    //SetTxBufferDesc88XX(pDriver_adapter, HCI_TX_DMA_QUEUE_CMD, (PTX_DESC_DATA_88XX)&desc_data);
    
    if (size) {
        _dma_cache_wback(((GET_DESC_FIELD(cur_txbd->TXBD_ELE[1].Dword1, TXBD_DW1_PHYADDR_LOW_MSK, TXBD_DW1_PHYADDR_LOW_SH)|0x80000000) - CONFIG_LUNA_SLAVE_PHYMEM_OFFSET_HAL),
            (u4Byte)size);
    }


    //printk("cur_h2c_buffer adr = %x \n",cur_h2c_buffer);
    //RT_PRINT_DATA(COMP_SEND, DBG_TRACE, "H2C Payload:\n", cur_h2c_buffer, 32);    

    
    UpdateSWTXBDHostIdx88XX(pDriver_adapter, cur_q);
    SyncSWTXBDHostIdxToHW88XX(pDriver_adapter, DMA_QUEUE_CMD);
    return TRUE;
    
#endif
}
#endif 

u8 HAL_SEND_RSVD_PAGE(
    IN VOID *pDriver_adapter, 
    IN u8 *pBuf, 
    IN u32 size
)
{
	u8 ret;
	HAL_PADAPTER pAdapter = (HAL_PADAPTER)pDriver_adapter;
#if IS_EXIST_EMBEDDED || IS_EXIST_PCI
	if (pAdapter->hci_type == RTL_HCI_PCIE) {
	    SigninBeaconTXBD88XX(pDriver_adapter,(pu4Byte)pBuf, size);    // fill TXDESC & TXBD

	    //RT_PRINT_DATA(COMP_INIT, DBG_LOUD, "DLtoTXBUFandDDMA88XX\n", pbuf, len);

	    if(_FALSE == DownloadRsvdPage88XX(pDriver_adapter,NULL,0,0)) {
	        RT_TRACE_F(COMP_INIT, DBG_WARNING,("Download to TXpktbuf fail ! \n"));
	        ret = FALSE;
	    }else{
	        ret = TRUE;
	    }
	}
#endif
#if IS_EXIST_USB || IS_EXIST_SDIO
	if (pAdapter->hci_type == RTL_HCI_USB || pAdapter->hci_type == RTL_HCI_SDIO) {
        #if !defined(CONFIG_RTL_TRIBAND_SUPPORT)
		ret = download_rsvd_page(pDriver_adapter, pBuf, size);
        #endif
	}
#endif

	return ret;
}

u8
HALAPI_R8(
    IN VOID *pDriver_adapter,
    IN u32 offset
)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    return RTL_R8(offset);
}

u16
HALAPI_R16(
    IN VOID *pDriver_adapter,
    IN u32 offset
)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    return RTL_R16(offset);  
}

u32
HALAPI_R32(
    IN VOID *pDriver_adapter,
    IN u32 offset
)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    return RTL_R32(offset);   
}


VOID
HALAPI_W8(
    IN VOID *pDriver_adapter,
    IN u32 offset,
    IN u8 value
)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    RTL_W8(offset,value);   
}

VOID
HALAPI_W16(
    IN VOID *pDriver_adapter,
    IN u32 offset,
    IN u16 value
)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    RTL_W16(offset,value);   
}

VOID
HALAPI_W32(
    IN VOID *pDriver_adapter,
    IN u32 offset,
    IN u32 value
)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    RTL_W32(offset,value);   
}

#ifdef CONFIG_SDIO_HCI
extern u8 rtw_sdio_read_cmd52(struct rtl8192cd_priv *priv, u32 addr, void *buf, size_t len);
extern u8 rtw_sdio_read_cmd53(struct rtl8192cd_priv *priv, u32 addr, void *buf, size_t len);
extern u8 rtw_sdio_write_cmd52(struct rtl8192cd_priv *priv, u32 addr, void *buf, size_t len);
extern u8 rtw_sdio_write_cmd53(struct rtl8192cd_priv *priv, u32 addr, void *buf, size_t len);


u8 HALAPI_SDIO_CMD52_READ(VOID *pDriver_adapter, u32 offset)
{
    struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    u8 val;

    rtw_sdio_read_cmd52(priv, offset, &val, 1);
    return val;
}

u8 HALAPI_SDIO_CMD53_READ_8(VOID *pDriver_adapter, u32 offset)
{
    struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    u8 val;
    
    rtw_sdio_read_cmd53(priv, offset, &val, 1);
    return val;
}

u16 HALAPI_SDIO_CMD53_READ_16(VOID *pDriver_adapter, u32 offset)
{
    struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    u16 val;
    
    rtw_sdio_read_cmd53(priv, offset, &val, 2);
    val = le16_to_cpu(val);
    return val;
}

u32 HALAPI_SDIO_CMD53_READ_32(VOID *pDriver_adapter, u32 offset)
{
    struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    u32 val;
    
    rtw_sdio_read_cmd53(priv, offset, &val, 4);
    val = le32_to_cpu(val);
    return val;
}

VOID HALAPI_SDIO_CMD52_WRITE(VOID *pDriver_adapter, u32 offset, u8 value)
{
    struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    
    rtw_sdio_write_cmd52(priv, offset, &value, 1);
}

VOID HALAPI_SDIO_CMD53_WRITE_8(VOID *pDriver_adapter, u32 offset, u8 value)
{
    struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    
    rtw_sdio_write_cmd53(priv, offset, &value, 1);
}

VOID HALAPI_SDIO_CMD53_WRITE_16(VOID *pDriver_adapter, u32 offset, u16 value)
{
    struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;

    value = cpu_to_le16(value);
    rtw_sdio_write_cmd53(priv, offset, &value, 2);
}

VOID HALAPI_SDIO_CMD53_WRITE_32(VOID *pDriver_adapter, u32 offset, u32 value)
{
    struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)pDriver_adapter;
    
    value = cpu_to_le32(value);
    rtw_sdio_write_cmd53(priv, offset, &value, 4);
}
#endif

VOID
MacHalGeneralDummy(struct rtl8192cd_priv *priv)
{
}

u8 
HALAPI_PRINT(
    IN VOID *pDriver_adapter, 
    IN u32 msg_type, 
    IN u8 msg_level, 
    IN s8* lpszFormat,...
)
{
//#if HALMAC_DEBUG_MESSAGE
#if defined(__ECOS) || defined(__OSK__)
   printk(lpszFormat);
#else
   va_list args;
   va_start(args, lpszFormat);
   vprintk(lpszFormat, args);
   va_end(args);
#endif
//#endif
 return _TRUE;
}

VOID
HALAPI_MALLOC(
    IN VOID *pDriver_adapter, 
    IN u32 size
)
{
    kmalloc(size, GFP_ATOMIC);    
}


VOID 
HALAPI_FREE(
 IN VOID *pDriver_adapter, 
 IN VOID *pBuf, 
 IN u32 size
)
{
    kfree(pBuf);
}


VOID
HALAPI_MEMCPY(
    IN VOID *pDriver_adapter, 
    IN VOID *dest, 
    IN VOID *src, 
    IN u32 size
)
{
    memcpy(dest,src,size);    
}

VOID
HALAPI_MEMSET(
    IN VOID *pDriver_adapter, 
    IN VOID *pAddress, 
    IN u8 value, 
    IN u32 size
)
{
    memset(pAddress,value,size);    
}
VOID
HALAPI_DELAY_US(
    IN VOID *pDriver_adapter, 
    IN u32 us
)
{
    delay_us(us);  
}
#if 0
VOID
HALAPI_MSLEEP(
    IN VOID *pDriver_adapter, 
    IN u32 ms
)
{
    delay_ms(ms);  
}
#endif
VOID
HALAPI_MUTEX_INIT(
 IN VOID *pDriver_adapter, 
 IN HALMAC_MUTEX *pMutex
)
{
    return;
}    

VOID
HALAPI_MUTEX_DEINIT(
 IN VOID *pDriver_adapter, 
 IN HALMAC_MUTEX *pMutex
)
{
    return;
}   

VOID
HALAPI_MUTEX_LOCK(
 IN VOID *pDriver_adapter, 
 IN HALMAC_MUTEX *pMutex
)
{
    u32 flag;
    spin_lock_irqsave(pMutex, flag);
}    

 

VOID
HALAPI_MUTEX_UNLOCK(
 IN VOID *pDriver_adapter, 
 IN HALMAC_MUTEX *pMutex
)
{
    u32 flag;
    spin_unlock_irqrestore(pMutex, flag);
}    

VOID
HALAPI_EVENT_INDICATION(
 IN VOID *pDriver_adapter, 
 IN HALMAC_FEATURE_ID feature_id, 
 IN HALMAC_CMD_PROCESS_STATUS process_status, 
 IN u8* buf, u32 size
)
{
    u32 i;
    //printk("feature_id:%x process_status:%x size:%d\n",feature_id, process_status,size );
	if(feature_id == HALMAC_FEATURE_DUMP_LOGICAL_EFUSE 
		&& process_status == HALMAC_CMD_PROCESS_DONE){
		for(i = 0; i < size; i++, buf++){
			printk("0x%08x ",*buf);
			if((i+1) % 4 == 0)
				printk("\n");
		}
	}
	return;
}

#endif 



