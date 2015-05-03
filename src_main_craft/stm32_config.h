/*
 * stm32_config1.h
 * STM32模块配置1 : Craft
 * 创建者：死猫
 * 创建时间：2015.02.13
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.21
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
#define SPI1_EN 1                                  //Craft: 使能SPI1
#define SPI1_BAUD_SCALER SPI_BaudRatePrescaler_8   //Craft: 8 <-> 4.5MHz

#define SPI2_EN 1                                  //Craft: 使能SPI2
#define SPI2_BAUD_SCALER SPI_BaudRatePrescaler_64   //Craft: 64 <-> 0.5MHz, 写入配置时时钟必须<1MHz
#define SPI2_BAUD_SCALER_READ SPI_BaudRatePrescaler_2   //Craft: 2 <-> 18MHz, 读一遍寄存器时间<?us

//******************* TIM Config ****************************************//

//sensor read
#define TIM2_EN          1          //Craft: 使能TIM2
#define TIM2_INIT_EN     1          //Craft: 使能TIM2中断
#define TIM2_PRESCALER  (72 - 1)    //Craft: 配置时钟     72MHz/72 = 1MHz
#define TIM2_PERIOD     (1000 - 1)  //Craft: 配置重载计数 1MHz/1000 = 1Khz = 1ms

//control
#define TIM3_EN          1          //Craft: 使能TIM3
#define TIM3_INIT_EN     1          //Craft: 使能TIM3中断
#define TIM3_PRESCALER  (7200 - 1)  //Craft: 配置时钟     72MHz/7200 = 10KHz
#define TIM3_PERIOD     (100 - 1)   //Craft: 配置重载计数 10KHz/100 = 100Hz = 10ms

//pwm
#define TIM4_EN          1          //Craft: 使能TIM4
#define TIM4_INIT_EN     0          //Craft: 使能TIM4中断
#define TIM4_PRESCALER  (72 - 1)    //Craft: 配置时钟     72MHz/72 = 1MHz
#define TIM4_PERIOD     (20000 - 1) //Craft: 配置重载计数 1MHz/20000 = 200Hz = 5ms

//
#define TIM5_EN          0          //Craft: 禁止TIM5
#define TIM5_INIT_EN     1          //Craft: 使能TIM5中断
#define TIM5_PRESCALER  (7200 - 1)  //Craft: 配置时钟     72MHz/7200 = 10KHz
#define TIM5_PERIOD     (200 - 1)   //Craft: 配置重载计数 10KHz/200 = 50Hz = 20ms

//******************* PWM Config ****************************************//
//TIM3 PWM: PA[6..7] PB[0..1] remap -> PC[6..9]
//TIM4 PWM: PB[6..9] remap -> PD[12..15]
//TIM5 PWM: PA[0..3]

/*
 * PWM 使用资源：
 *     TIM4, PB[6..9]
 */

#define PWM_EN             1     //Craft: 使能PWM
#define PWM_TIMER          TIM4  //Craft: 配置PWM时钟源为TIM4
#define PWM_TIMER_REMAP_EN 0     //Craft: 禁止REMAP
//#define PWM_TIMER_REMAP GPIO_FullRemap_TIM4

#endif
