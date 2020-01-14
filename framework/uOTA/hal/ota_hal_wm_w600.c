#include "ota_hal_plat.h"
#include "wm_osal.h"
#include "wm_fwup.h"
#include "wm_crypto_hard.h"
#include "wm_internal_flash.h"
#include "wm_flash_map.h"
#include "wm_params.h"
#include "wm_param.h"
#include "wm_mem.h"
#include "hal/soc/flash.h"

#define FLASH_BASE_ADDR	(0x8000000)
#define FLASH_READ_SIZE	(1024)

struct demo_ota {
	u32 exp_len;
	u32 partition_addr;
    u32 org_checksum;
    u32 updated_len;
};
static struct demo_ota demo_ota;	
static int boot_part = HAL_PARTITION_OTA_TEMP;

static int ota_init(void *something)
{
    int ret = 0;
    ota_boot_param_t *param = (ota_boot_param_t *)something;
    hal_logic_partition_t *part_info = hal_flash_get_info(boot_part);

    param->off_bp = 0;
    if(part_info->partition_length < param->len || param->len == 0) {
        return -1;
    }

    if(TLS_FLS_STATUS_OK != tls_fls_fast_write_init()){
        return -1;
    }

    return 0;
}


static int ota_write(int* off, char* in_buf ,int in_buf_len)
{
    struct demo_ota *ota = &demo_ota;   
    static T_BOOTER booter;
    static u8 hd_offset = 0;
    u8 *buffer = NULL;
    u8 headerLen = sizeof(T_BOOTER);
    int req_len;
    int i = 0;

    buffer = (u8 *)in_buf;
    req_len = in_buf_len;   
    
    //obtain image's header.	
    if(ota->exp_len == 0)
    {
        if(req_len >= (headerLen - hd_offset)){
            memcpy(((u8 *)&booter)+hd_offset, buffer, headerLen-hd_offset);
            req_len -= (headerLen-hd_offset);
            buffer += (headerLen-hd_offset);
            if (!img_header_check(&booter)){
				printf("## head check error !!!!!\n");
                return -1;
            }				
            if (booter.zip_type == ZIP_FILE){
                ota->partition_addr = booter.zip_img_addr | FLASH_BASE_ADDR;
                ota->exp_len = booter.zip_img_len;
                ota->org_checksum = booter.zip_checksum;
            }
            else{
                ota->partition_addr = booter.img_addr| FLASH_BASE_ADDR;
                ota->exp_len = booter.img_len;
                ota->org_checksum = booter.org_checksum;
            }            
            ota->updated_len = 0;
            hd_offset = 0;
        }
        else{
            memcpy(&booter+hd_offset, buffer, req_len);
            hd_offset += req_len;
            return 0;
        }
    }

    if (req_len > 0) 
		{
        int ret = tls_fls_fast_write(ota->partition_addr + ota->updated_len, buffer, req_len);
        if(ret != TLS_FLS_STATUS_OK){
				return -1;
		}
		
		ota->updated_len += req_len;
        
        //download done.
		if(ota->updated_len >= ota->exp_len) 
		{
			u32 len_p=0, left=ota->exp_len, offset_p=0;
            u32 image_checksum = 0;
			psCrcContext_t	crcContext;

			tls_fls_fast_write_destroy();
			u8 *buffer_t = tls_mem_alloc(FLASH_READ_SIZE);
			if (buffer_t == NULL){
				return -1;
			}
			else{
				tls_crypto_crc_init(&crcContext, 0xFFFFFFFF, CRYPTO_CRC_TYPE_32, 3);
				while (left > 0) 
				{
					len_p = left > FLASH_READ_SIZE ? FLASH_READ_SIZE : left;
					tls_fls_read(ota->partition_addr + offset_p, buffer_t, len_p);
					tls_crypto_crc_update(&crcContext, buffer_t, len_p);
					offset_p += len_p;
					left -= len_p;
				}
				tls_crypto_crc_final(&crcContext, &image_checksum);
				tls_mem_free(buffer_t);
			}         
			if (ota->org_checksum != image_checksum){
				printf("varify incorrect[0x%02x, but 0x%02x]\n", ota->org_checksum, image_checksum);
				return -1;
			}
            img_update_header(&booter);		
		}
	}
    return 0;
}


static int ota_read(int* off, char* out_buf, int out_buf_len)
{}

static int ota_boot(void *something)
{}

static int ota_rollback(void *something)
{}


const char   *aos_get_app_version(void);
static const char *ota_get_version(unsigned char dev_type)
{
    if(dev_type) {
        return "v1.0.0-20180101-1000";//SYSINFO_APP_VERSION;
    } else {
        return aos_get_app_version();
    }
}

ota_hal_module_t ota_hal_module = {
    .init     = ota_init,
    .write    = ota_write,
    .read     = ota_read,
    .boot     = ota_boot,
    .rollback = ota_rollback,
    .version  = ota_get_version,
};

