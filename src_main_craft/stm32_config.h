/*
 * stm32_config1.h
 * STM32ģ������1 : Craft
 * �����ߣ���è
 * ����ʱ�䣺2015.02.13
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.04.21
 */

#ifndef STM32_CONFIG_H_
#define STM32_CONFIG_H_

//******************* SPI Config ****************************************//
#define NVIC_SYSTICK_EN 1
#define NVIC_INIT_LIST_DATA { \
	TIM2_IRQn,   1, 0, \
	TIM3_IRQn,   3, 1, \
	EXTI0_IRQn,  3, 0, \
	0xFF}

//******************* SPI Config ****************************************//
#define SPI1_EN 1                                  //Craft: ʹ��SPI1
#define SPI1_BAUD_SCALER SPI_BaudRatePrescaler_8   //Craft: 8 <-> 4.5MHz

#define SPI2_EN 1                                  //Craft: ʹ��SPI2
#define SPI2_BAUD_SCALER SPI_BaudRatePrescaler_64   //Craft: 64 <-> 0.5MHz, д������ʱʱ�ӱ���<1MHz
#define SPI2_BAUD_SCALER_READ SPI_BaudRatePrescaler_2   //Craft: 2 <-> 18MHz, ��һ��Ĵ���ʱ��<?us

//******************* TIM Config ****************************************//

//sensor read
#define TIM2_EN          1          //Craft: ʹ��TIM2
#define TIM2_INIT_EN     1          //Craft: ʹ��TIM2�ж�
#define TIM2_PRESCALER  (72 - 1)    //Craft: ����ʱ��     72MHz/72 = 1MHz
#define TIM2_PERIOD     (1000 - 1)  //Craft: �������ؼ��� 1MHz/1000 = 1Khz = 1ms

//control
#define TIM3_EN          1          //Craft: ʹ��TIM3
#define TIM3_INIT_EN     1          //Craft: ʹ��TIM3�ж�
#define TIM3_PRESCALER  (7200 - 1)  //Craft: ����ʱ��     72MHz/7200 = 10KHz
#define TIM3_PERIOD     (100 - 1)   //Craft: �������ؼ��� 10KHz/100 = 100Hz = 10ms

//pwm
#define TIM4_EN          1          //Craft: ʹ��TIM4
#define TIM4_INIT_EN     0          //Craft: ʹ��TIM4�ж�
#define TIM4_PRESCALER  (72 - 1)    //Craft: ����ʱ��     72MHz/72 = 1MHz
#define TIM4_PERIOD     (20000 - 1) //Craft: �������ؼ��� 1MHz/20000 = 200Hz = 5ms

//
#define TIM5_EN          0          //Craft: ��ֹTIM5
#define TIM5_INIT_EN     1          //Craft: ʹ��TIM5�ж�
#define TIM5_PRESCALER  (7200 - 1)  //Craft: ����ʱ��     72MHz/7200 = 10KHz
#define TIM5_PERIOD     (200 - 1)   //Craft: �������ؼ��� 10KHz/200 = 50Hz = 20ms

//******************* PWM Config ****************************************//
//TIM3 PWM: PA[6..7] PB[0..1] remap -> PC[6..9]
//TIM4 PWM: PB[6..9] remap -> PD[12..15]
//TIM5 PWM: PA[0..3]

/*
 * PWM ʹ����Դ��
 *     TIM4, PB[6..9]
 */

#define PWM_EN             1     //Craft: ʹ��PWM
#define PWM_TIMER          TIM4  //Craft: ����PWMʱ��ԴΪTIM4
#define PWM_TIMER_REMAP_EN 0     //Craft: ��ֹREMAP
//#define PWM_TIMER_REMAP GPIO_FullRemap_TIM4

#endif
