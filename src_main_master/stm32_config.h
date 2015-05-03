/*
 * stm32_config.h
 * STM32ģ������ : Master
 * �����ߣ���è
 * ����ʱ�䣺2015.02.13
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.02.13
 */

#ifndef STM32_CONFIG_H_
#define STM32_CONFIG_H_

//******************* NVIC Config ****************************************//
#define NVIC_SYSTICK_EN 1
#define NVIC_INIT_LIST_DATA { \
	TIM2_IRQn,   3, 0, \
	TIM3_IRQn,   3, 1, \
	EXTI0_IRQn,  1, 2, \
	USART1_IRQn, 1, 1, \
	DMA1_Channel4_IRQn, 2, 0, \
	0xFF}

//******************* UART Config ****************************************//
#define UART_DMA_EN 1

//******************* SPI Config ****************************************//
#define SPI1_EN 1                                  //Master: ʹ��SPI1
#define SPI1_BAUD_SCALER SPI_BaudRatePrescaler_8   //Master: 8 <-> 9MHz

#define SPI2_EN 1                                  //Master: ��ֹSPI2
#define SPI2_BAUD_SCALER SPI_BaudRatePrescaler_64  //Master: 64 <-> 1.125MHz

//******************* TIM Config ****************************************//
#define TIM2_EN          1          //Master: ��ֹTIM2
#define TIM2_INIT_EN     1          //Master: ʹ��TIM2�ж�
#define TIM2_PRESCALER  (72 - 1)    //Master: ����ʱ��     72MHz/72 = 1MHz
#define TIM2_PERIOD     (10000 - 1)  //Master: �������ؼ��� 1MHz/10000 = 100hz = 10ms


#define TIM3_EN          1          //Master: ��ֹTIM3
#define TIM3_INIT_EN     1          //Master: ʹ��TIM3�ж�
#define TIM3_PRESCALER  (7200 - 1)  //Master: ����ʱ��     72MHz/7200 = 10KHz
#define TIM3_PERIOD     (1000 - 1)  //Master: �������ؼ��� 10KHz/1000 = 10Hz = 100ms

#define TIM4_EN          0          //Master: ��ֹTIM4
#define TIM4_INIT_EN     1          //Master: ʹ��TIM4�ж�
#define TIM4_PRESCALER  (72 - 1)    //Master: ����ʱ��     72MHz/72 = 1MHz
#define TIM4_PERIOD     (10000 - 1) //Master: �������ؼ��� 1MHz/10000 = 100Hz = 10ms

#define TIM5_EN          0          //Master: ��ֹTIM5
#define TIM5_INIT_EN     1          //Master: ʹ��TIM5�ж�
#define TIM5_PRESCALER  (72 - 1)    //Master: ����ʱ��     72MHz/72 = 1MHz
#define TIM5_PERIOD     (10000 - 1) //Master: �������ؼ��� 1MHz/10000 = 100Hz = 10ms

//******************* PWM Config ****************************************//
//TIM3 PWM: PA[6..7] PB[0..1] remap -> PC[6..9]
//TIM4 PWM: PB[6..9] remap -> PD[12..15]
//TIM5 PWM: PA[0..3]

/*
 * PWM ʹ����Դ��
 *     TIM4, PB[6..9]
 */
#define PWM_EN             0     //Master: ��ֹPWM
#define PWM_TIMER          TIM4  //Master: ����PWMʱ��ԴΪTIM4
#define PWM_TIMER_REMAP_EN 0     //Master: ��ֹREMAP
//#define PWM_TIMER_REMAP GPIO_FullRemap_TIM4

#endif
