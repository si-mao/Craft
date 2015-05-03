/*
 * adc_roll.c
 * ADC��ť����
 * �����ߣ���è
 * ����ʱ�䣺2015.03.16
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.24
 */

#include "system_define.h"

#include "adc_roll.h"
#include "stm32_adc.h"

uint16_t ADC_Elem_Level;         //��һ������ֵ
uint8_t ADC_Roll_Start_Flag;     //����ADC Rollʹ�ܱ�־����0ʹ��

uint16_t ADC_Value[2];           //ADCֵ
int16_t ADC_Roll_Data[2];        //ADC Rollֵ

uint8_t ADC_Ch_Step;             //������ǰ�Ѿ�ת����ɵ�ͨ��

uint16_t ADC_Value_Old[2];       //ADC�ϴ�ֵ���м����
uint16_t ADC_Value_After_Clear[2];  //�ڵ���roll_clear�����󱣴��ADCֵ, �ڱ�ģ���в�ʹ��

void adc_roll_init(void)
{
	ADC_Roll_Start_Flag = 0;
	ADC_Ch_Step = 1;
	adc_start_convert(ADC_Ch_Step);
}

void adc_roll_start(uint16_t adc_elem_level)
{
	ADC_Elem_Level = adc_elem_level;
	ADC_Roll_Data[0] = 0;
	ADC_Roll_Data[1] = 0;
	ADC_Value_Old[0] = ADC_Value[0];
	ADC_Value_Old[1] = ADC_Value[1];
	
	ADC_Roll_Start_Flag = 1;
}

//����: ADC_Value, ADC_Elem_Level, ADC_Value_Old
//����: ADC_Value_Old
//���: ADC_Roll_Data
void adc_roll_process(void)
{
	uint16_t ADC_Value_abs[2];
	uint8_t i;
	int8_t roll_flag;
	
	for(i = 0; i < 2; i++)
	{
		roll_flag = 0;
		if(ADC_Value[i] > ADC_Value_Old[i])
		{
			ADC_Value_abs[i] = ADC_Value[i] - ADC_Value_Old[i];
			if(ADC_Value_abs[i] > ADC_CIRCLE_LEVEL)
			{
				ADC_Value_abs[i] = ADC_Value_Old[i] + ADC_FULL_RANGE - ADC_Value[i];
			}
			if(ADC_Value_abs[i] > ADC_Elem_Level)
			{
				roll_flag = 1;  //����һ��
				ADC_Value_Old[i] = ADC_Value[i];
			}
		}
		else
		{
			ADC_Value_abs[i] = ADC_Value_Old[i] - ADC_Value[i];
			if(ADC_Value_abs[i] > ADC_CIRCLE_LEVEL)
			{
				ADC_Value_abs[i] = ADC_Value[i] + ADC_FULL_RANGE - ADC_Value_Old[i];
			}
			if(ADC_Value_abs[i] > ADC_Elem_Level)
			{
				roll_flag = -1;  //����һ��
				ADC_Value_Old[i] = ADC_Value[i];
			}
		}
		if(roll_flag == 1)
		{
			ADC_Roll_Data[i]++;
		}
		else if(roll_flag == -1)
		{
			ADC_Roll_Data[i]--;
		}
	}
}

//adc_roll_loop()�����ڶ�ʱ���ж���
void adc_roll_loop(void)
{
	if(ADC_Ch_Step == 1)
	{
		//while(0 == adc_is_convert_finish());
		ADC_Value[0] = (ADC_Value[0] + adc_get_value())/2;
		ADC_Ch_Step = 2;
		adc_start_convert(ADC_Ch_Step);
	}
	else if(ADC_Ch_Step == 2)
	{
		//while(0 == adc_is_convert_finish());
		ADC_Value[1] = (ADC_Value[1] + adc_get_value())/2;
		ADC_Ch_Step = 1;
		adc_start_convert(ADC_Ch_Step);

		//��������ͨ��
		if(ADC_Roll_Start_Flag != 0)
		{
			adc_roll_process();
		}
	}
	else
	{
		ADC_Ch_Step = 1;
	}
}
