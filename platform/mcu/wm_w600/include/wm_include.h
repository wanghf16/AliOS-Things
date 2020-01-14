/**
 * @file    wm_include.h
 *
 * @brief   the configuration file of sdk
 *
 * @author  winnermicro
 *
 * Copyright (c) 2014 Winner Microelectronics Co., Ltd.
 */
#ifndef __WM_INCLUDE_H__
#define __WM_INCLUDE_H__

/**
 * @mainpage winnermicro venus sdk
 *
 * @section intro_sec winnermicro venus sdk APIs.
 *
 * This library provides easy to use APIs to use by hiding all details.
 *
 * @section usage_sec How to use the sdk
 *
 * @subsection source how to code
 *
 * void UserMain(void) is the entrance function of the application:
 * @code
 * void UserMain(void)
 * {
 *     printf("\n user task\n");
 *
 * #if DEMO_CONSOLE
 *	   CreateDemoTask();
 * #endif
 *
 *     //user's task
 * }
 * @endcode
 *
 * @subsection compile How to compile
 *
 * To build with the sdk you can use the keil tools.
 * Opening "Tools\Keil\Project\WM_SDK.uvproj" to compile.
 *
 * @section install_sec How to download the firmware
 *
 * @subsection fw_flash Download the "WM_SDK.img" image
 *
 * Go to write image to flash.
 *
 * @subsection fw_ram Download the "WM_SDK_UART.img" image
 *
 * Go to download image to ram.
 * @code
 * Press "ESC" and the reset key until the com0 output: "cc..."
 * @endcode
 *
 * \n
 */

#include <stdio.h>
#include <stdlib.h>
#include "wm_type_def.h"
#include "wm_uart.h"
#include "wm_gpio.h"
//#include "wm_flash.h"
#include "wm_hostspi.h"
#include "wm_socket.h"
#include "wm_sockets.h"
#include "wm_wifi.h"
#include "wm_hspi.h"
#include "wm_pwm.h"
#include "wm_params.h"
#include "wm_osal.h"
#include "wm_netif.h"
#include "wm_efuse.h"
#include "wm_mem.h"
#include "wm_regs.h"


#endif
