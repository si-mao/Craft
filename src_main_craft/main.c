/*
 * main.c
 * 主函数
 * 创建者：死猫
 * 创建时间：2014.xx.xx
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.13
 */

#include "system_define.h"
#include "hardware.h"		//io端口宏定义
#include "error.h"

#include "stm32_config.h"

//STM32
#include "stm32_delay.h"
#include "stm32_sys.h"
#include "stm32_systick.h"
#include "stm32_spi.h"
#include "stm32_led.h"
#include "stm32_key.h"
#include "stm32_pwm.h"
#include "stm32_timer.h"
#include "stm32_exti.h"
#include "stm32_gpio.h"

//IO
#include "package.h"
#include "io_data.h"
#include "cc1101.h"
#include "rf_mgr.h"

//Craft
#include "mpu9250.h"
#include "craft_data.h"
#include "imu_attitude.h"
#include "imu_physics.h"
#include "imu_smooth.h"
//#include "imu_normalize.h"
#include "calcu_pwm.h"
//#include "atti_align.h"

int main(void)
{
	uint32_t time, time_old;

	//系统级
	RCC_Configuration();
	NVIC_Configuration();


	//外设级
	systick_init();
	gpio_init();
	spi_init();
	timer_init();
	pwm_init();
	exti_init();
	led_init();


	//变量级
	io_data_init();
	craft_data_init();
	imu_attitude_init();
	imu_physics_init();
	imu_smooth_init();
	//imu_normalize_init();  //无初始化
	calcu_pwm_init();
//	atti_align_init();
	rf_mgr_init();

	//接口级
	TSET_1 = 0;
	TSET_2 = 0;
	cc1101_init();
	mpu9250_init();
	spi_set_baud(SPI2, SPI2_BAUD_SCALER_READ);  //mpu9250初始化完毕后提高spi频率
	pwm_set_zero();


	//模式设置级
	//atti_align_start();


	//延迟
	delayms(10);


	//运行启动级
	SYSTICK_START
	cc1101_mgr_recv_start();
	EXTI_INTER_EN           //外部中断使能
	pwm_enable();
	TIM_Cmd(TIM2, ENABLE);  //TIM2总开关：开启
	TIM_Cmd(TIM3, ENABLE);  //TIM3总开关：开启

	while(1)
	{
		time = get_systick();
		if(time - time_old > 500)
		{
			time_old = time;
			LED_BREATH = ~LED_BREATH;
			//pos[0] = 1, pos[1] = 0;
			//file_set_pos(FILE_SCREEN_WO, pos);
			//file_write_uint(FILE_SCREEN_WO, time);
		}
		package_listen_loop();
// 		rf_recv_loop();
// 		rf_send_loop();
		rf_mgr_loop();
		
	}
	return 0;
}
