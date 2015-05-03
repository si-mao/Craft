/*
 * adc_roll.h
 * ADC��ť����
 * �����ߣ���è
 * ����ʱ�䣺2015.03.16
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.24
 */
 
#ifndef ADC_ROLL_H_
#define ADC_ROLL_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

#define ADC_FULL_RANGE 4096
#define ADC_CIRCLE_LEVEL 3000

#define ADC_ELEM_LEVEL_MENU  150
#define ADC_ELEM_LEVEL_INPUT 40

extern uint16_t ADC_Value[2];           //ADCֵ
extern int16_t ADC_Roll_Data[2];        //ADC Rollֵ
extern uint16_t ADC_Value_After_Clear[2];  //�ڵ���roll_clear�����󱣴��ADCֵ, �ڱ�ģ���в�ʹ��

extern void adc_roll_init(void);
extern void adc_roll_start(uint16_t adc_elem_level);
extern void adc_roll_loop(void);

__INLINE int16_t adc_roll_get_data(uint8_t ch)
{
	return ADC_Roll_Data[ch];
}

__INLINE void adc_roll_clear_data(uint8_t ch)
{
	ADC_Roll_Data[ch] = 0;
	ADC_Value_After_Clear[ch] = ADC_Value[ch];
}

__INLINE uint16_t adc_get_data(uint8_t ch)
{
	return ADC_Value[ch];
}

__INLINE uint16_t adc_get_data_after_clear(uint8_t ch)
{
	return ADC_Value_After_Clear[ch];
}

#ifdef __cplusplus
};
#endif

#endif
