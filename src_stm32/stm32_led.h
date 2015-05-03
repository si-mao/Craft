/*
 * stm32_led.h
 * led
 * �����ߣ���è
 * ����ʱ�䣺2014.xx.xx
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.14
 */

//led ���� - ��һ��STM32���İ�V3.0
//ledռ������D13, G14

#ifndef STM32_LED_H_
#define STM32_LED_H_

#include "system_define.h"
#include "hardware.h"		//io�˿ں궨��

#define LED1 PDout(13)			 //����LED��� D5
#define LED2 PGout(14)			 //����LED��� D2

#ifdef __cplusplus
extern "C"{
#endif

extern void led_init(void);

#ifdef __cplusplus
};
#endif

#endif
