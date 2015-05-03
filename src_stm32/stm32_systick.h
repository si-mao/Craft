/*
 * stm32_systick.h
 * 时间记录
 * 创建者：死猫
 * 创建时间：2015.03.13
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.03.13
 */

#ifndef STM32_SYSTICK_H_
#define STM32_SYSTICK_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

extern volatile uint32_t systick_value;

extern void systick_init(void);

__INLINE uint32_t get_systick(void) { return systick_value; }

#define SYSTICK_START {SysTick -> CTRL |= 0x01;}

#ifdef __cplusplus
};
#endif

#endif
