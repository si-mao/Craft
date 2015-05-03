/*
 * stm32_delay.h
 * 延迟
 * 创建者：死猫
 * 创建时间：2014.xx.xx
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.14
 */

#ifndef STM32_DELAY_H_
#define STM32_DELAY_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

extern void delayms(uint32_t timems);
extern void delayus(uint32_t timeus);
extern void delay100ns(uint32_t times);
	
#ifdef __cplusplus
};
#endif

#endif
