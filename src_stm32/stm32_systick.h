/*
 * stm32_systick.h
 * ʱ���¼
 * �����ߣ���è
 * ����ʱ�䣺2015.03.13
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.03.13
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
