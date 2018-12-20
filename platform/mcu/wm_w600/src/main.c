#include <stdio.h>

#include <aos/aos.h>
#include <k_api.h>
#include <aos/kernel.h>
#include "hal/ota.h"
#include <hal/soc/soc.h>

#include "wm_regs.h"
#include "wm_io.h"
#include "wm_gpio.h"
#include "wm_params.h"
#include "wm_netif2.0.3.h"


#define MCU_CLOCK_HZ        (80000000)

#define AOS_START_STACK 	1536
#define AOS_MM_STACK	1024

#ifdef CONFIG_AOS_CLI
#include "aos/types.h"
#include "aos/cli.h"
extern int csp_printf(const char *fmt, ...);
#define print aos_cli_printf
#else
#define print printf
#endif

ktask_t *g_aos_init;

ktask_t *g_aos_mm_info;

extern void board_init(void);

extern int tls_ethernet_init();

extern void wifi_net_status_changed(u8 status);

extern hal_ota_module_t  w600_ota_module;

extern uart_dev_t uart_1;
extern uart_dev_t uart_0;


void SystemInit()
{

}

static kinit_t kinit;


void board_cli_init(void)
{
    kinit.argc = 0;
    kinit.argv = NULL;
    kinit.cli_enable = 1;
}

void OS_CPU_SysTickHandler(void)
{
	krhino_intrpt_enter();
	krhino_tick_proc();
	krhino_intrpt_exit();	
}

void do_mminfo(void)
{
	k_mm_head *mmhead = g_kmm_head;

//		MM_CRITICAL_ENTER(g_kmm_head);    
    printf("\r\n");
    printf("------------------------- memory allocation statistic ------------------------------ \r\n");

    int i;

    if (!mmhead) {
        return;
    }

    printf("     free     |     used     |     maxused\r\n");
    printf("  %10d  |  %10d  |  %10d\r\n", 
          mmhead->free_size, mmhead->used_size, mmhead->maxused_size);
    printf("\r\n");   
 
//    MM_CRITICAL_EXIT(g_kmm_head);	
}

static void wifi_rx_info(char *buf, int len, int argc, char **argv)
{
}
static void wifi_tx_info(char *buf, int len, int argc, char **argv)
{
    u8* buff = NULL;
    printf("wifi_tx_info enter\n");
    buff = tls_wifi_buffer_acquire(100);
    printf("tls_wifi_buffer_acquire buf %p\n", buff);
    tls_wifi_buffer_release(false, buff);
    printf("tls_wifi_buffer_release buf %p\n", buff);
}
static cb_fun(void* ctx)
{
    printf("\n@@@@@@@@@@@@@@@@@@@@@@ lwip task is alive! @@@@@@@@@@@@@@@@@@@@@@@@@@\n");    
}
extern void * lwip_fun_ptr;
static void lwip_info(char *buf, int len, int argc, char **argv)
{
    tcpip_callback_with_block(cb_fun, NULL, 1);
}

struct cli_command w600_cli_cmd[] = {
    { "wifi_rx", "dump wifi rx task info", wifi_rx_info },
    { "wifi_tx", "dump wifi tx task info", wifi_tx_info },
    { "lwip", "dump lwip task info", lwip_info },//tcpip_callback_with_block
};


void tls_w600_cli_init(void)
{
    aos_cli_register_commands(&w600_cli_cmd[0],sizeof(w600_cli_cmd) / sizeof(struct cli_command));
}

void sys_init_func(void)
{    
  board_cli_init();		
	tls_fls_init();

#if TLS_CONFIG_HARD_CRYPTO
	tls_crypto_init();
#endif

	tls_param_load_factory_default();
	tls_param_init();	

	w600_wifi_register();
  hal_wifi_init();

	tls_ethernet_init();
	tls_sys_init();
	hal_ota_register_module(&w600_ota_module);

	tls_netif_add_status_event(wifi_net_status_changed);

    board_init();
//	dumpsys_mm_info_func(0);
//	printf("%d\r\n", RHINO_CONFIG_MM_BLK_SIZE);

//	test_wifi_start();

//	test_wifi_start_ap();
	
//	test_start_scan();
//		krhino_task_dyn_create(&g_aos_mm_info, "aos-mm_info", 0, 20, 0, AOS_MM_STACK, (task_entry_t)mm_info, 1);
    aos_kernel_init(&kinit);

}

int main(void)
{    
    aos_init();
		tls_sys_clk_set(0);
		hal_uart_init(&uart_0);
//		printf("alios start\n");
    krhino_task_dyn_create(&g_aos_init, "aos-init", 0, 30, 0, AOS_START_STACK, (task_entry_t)sys_init_func, 1);
		SysTick_Config(MCU_CLOCK_HZ / 100);
    aos_start();
    return 0;
}



