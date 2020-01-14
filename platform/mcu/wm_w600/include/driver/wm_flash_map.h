/**
 * @file    wm_flash_map.h
 *
 * @brief   flash zone map
 *
 * @author  dave
 *
 * Copyright (c) 2014 Winner Microelectronics Co., Ltd.
 */
#ifndef __WM_FLASH_MAP_H__
#define __WM_FLASH_MAP_H__

/**FLASH MAP**/

#define FLASH_BASE_ADDR						(0x08000000)
#define SECBOOT_HEADER_ADDR 	            (0x08002000) 			/*256bytes*/
#define SECBOOT_AREA_ADDR		            (0x08002100)            /*(56K-256)bytes*/
#define IMAGE_HEADER_ADDR1		            (0x08010000)
#define IMAGE_HEADER_ADDR2		            (0x080FC000)
#define TLS_FLASH_PARAM1_ADDR				(0x080FD000)
#define TLS_FLASH_PARAM2_ADDR				(0x080FE000)

#define CODE_START_ADDR			            (0x08010100)
#define CODE_END_ADDR			            (0x080F0000 - 1)
#define USER_ADDR_START			            (0x080F3000)
#define TLS_FLASH_PARAM_DEFAULT             ((0x080F3000))


#define TLS_BOOT_IMAGE_SIGNATURE_WORD      0xa0ffff9f
#define SIGNATURE_WORD      				0xa0ffff9f

#endif /*__WM_CONFIG_H__*/

