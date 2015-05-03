/*
 * stm32_delay.h
 * �ӳ�
 * �����ߣ���è
 * ����ʱ�䣺2014.xx.xx
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.14
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
