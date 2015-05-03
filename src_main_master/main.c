/*
 * main.c
 * 主函数
 * 创建者：死猫
 * 创建时间：2014.xx.xx
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.24
 */

#include "system_define.h"
#include "hardware.h"		//io端口宏定义
#include "error.h"

//STM32
#include "stm32_delay.h"
#include "stm32_sys.h"
#include "stm32_systick.h"
#include "stm32_spi.h"
#include "stm32_led.h"
#include "stm32_key.h"
#include "stm32_timer.h"
#include "stm32_exti.h"
#include "stm32_gpio.h"
#include "stm32_usart.h"
#include "stm32_adc.h"
#include "stm32_dma.h"

//IO
#include "package.h"
#include "io_data.h"
#include "cc1101.h"
#include "rf_mgr.h"
#include "file.h"
#include "adc_roll.h"
#include "usart_mgr.h"


//UI
#include "lcd12864.h"
#include "menu.h"


//Algori
#include "queue.h"


//Craft
#include "craft_data.h"

uint8_t HID_Buffer[4];

extern void inter_time_data_init(void);  //位于inter_inter.c文件

int main(void)
{
// 	uint8_t pos[2];
	uint32_t time, time_old;
	
	//系统级
	RCC_Configuration();
	NVIC_Configuration();


	//外设级
	systick_init();
	gpio_init();
	usart_init();
	spi_init();
	timer_init();
	exti_init();
	led_init();
	adc_init();
	dma_init();


	//变量级
	io_data_init();
	file_init();
	menu_init();
	adc_roll_init();
	key_init();
	inter_time_data_init();
	rf_mgr_init();
	usart_mgr_init();


	//接口级
	TSET_1 = 0;
	TSET_2 = 0;
	cc1101_init();
 	lcd12864_init();

 	//模式设置级

 	//延迟
 	delayms(200);

	//运行启动级
	adc_roll_start(ADC_ELEM_LEVEL_MENU);
	rf_mgr_recv_start();
	EXTI_INTER_EN               //外部中断使能
	TIM_Cmd(TIM2, ENABLE);      //TIM2总开关：开启
	TIM_Cmd(TIM3, ENABLE);      //TIM3总开关：开启
	USART_Cmd(USART1, ENABLE);  //USART使能

 	//延迟
 	delayms(200);

 	menu_refresh();
	while(1)
	{
		time = get_systick();
		if(time - time_old > 500)
		{
			time_old = time;
			LED_BREATH = ~LED_BREATH;
		}
		rf_mgr_loop();
		usart_mgr_loop();
		package_listen_loop();
		file_loop();
	}
	return 0;
}
