/*
 * stm32_led.h
 * led
 * 创建者：死猫
 * 创建时间：2014.xx.xx
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.14
 */

//led 驱动 - 贝一特STM32核心板V3.0
//led占用引脚D13, G14

#ifndef STM32_LED_H_
#define STM32_LED_H_

#include "system_define.h"
#include "hardware.h"		//io端口宏定义

#define LED1 PDout(13)			 //板上LED编号 D5
#define LED2 PGout(14)			 //板上LED编号 D2

#ifdef __cplusplus
extern "C"{
#endif

extern void led_init(void);

#ifdef __cplusplus
};
#endif

#endif
