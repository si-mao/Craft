/*
 * main.c
 * ������
 * �����ߣ���è
 * ����ʱ�䣺2014.xx.xx
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.13
 */

#include "system_define.h"
#include "hardware.h"		//io�˿ں궨��
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

	//ϵͳ��
	RCC_Configuration();
	NVIC_Configuration();


	//���輶
	systick_init();
	gpio_init();
	spi_init();
	timer_init();
	pwm_init();
	exti_init();
	led_init();


	//������
	io_data_init();
	craft_data_init();
	imu_attitude_init();
	imu_physics_init();
	imu_smooth_init();
	//imu_normalize_init();  //�޳�ʼ��
	calcu_pwm_init();
//	atti_align_init();
	rf_mgr_init();

	//�ӿڼ�
	TSET_1 = 0;
	TSET_2 = 0;
	cc1101_init();
	mpu9250_init();
	spi_set_baud(SPI2, SPI2_BAUD_SCALER_READ);  //mpu9250��ʼ����Ϻ����spiƵ��
	pwm_set_zero();


	//ģʽ���ü�
	//atti_align_start();


	//�ӳ�
	delayms(10);


	//����������
	SYSTICK_START
	cc1101_mgr_recv_start();
	EXTI_INTER_EN           //�ⲿ�ж�ʹ��
	pwm_enable();
	TIM_Cmd(TIM2, ENABLE);  //TIM2�ܿ��أ�����
	TIM_Cmd(TIM3, ENABLE);  //TIM3�ܿ��أ�����

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
