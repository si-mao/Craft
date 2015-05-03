/*
 * stm32_adc.h
 * adc
 * �����ߣ���è
 * ����ʱ�䣺2015.03.15
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.15
 */
 
#ifndef STM32_ADC_H_
#define STM32_ADC_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

extern void adc_init(void);
extern void adc_start_convert(uint8_t ch);
extern uint8_t adc_is_convert_finish(void);
extern uint16_t adc_get_value(void);

#ifdef __cplusplus
};
#endif

#endif
