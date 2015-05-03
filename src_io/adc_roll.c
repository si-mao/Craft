/*
 * adc_roll.c
 * ADC旋钮输入
 * 创建者：死猫
 * 创建时间：2015.03.16
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.24
 */

#include "system_define.h"

#include "adc_roll.h"
#include "stm32_adc.h"

uint16_t ADC_Elem_Level;         //跳一步的阈值
uint8_t ADC_Roll_Start_Flag;     //计算ADC Roll使能标志，非0使能

uint16_t ADC_Value[2];           //ADC值
int16_t ADC_Roll_Data[2];        //ADC Roll值

uint8_t ADC_Ch_Step;             //表征当前已经转化完成的通道

uint16_t ADC_Value_Old[2];       //ADC上次值，中间变量
uint16_t ADC_Value_After_Clear[2];  //在调用roll_clear函数后保存的ADC值, 在本模块中不使用

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

//输入: ADC_Value, ADC_Elem_Level, ADC_Value_Old
//更改: ADC_Value_Old
//输出: ADC_Roll_Data
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
				roll_flag = 1;  //滚动一步
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
				roll_flag = -1;  //滚动一步
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

//adc_roll_loop()放置在定时器中断中
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

		//读完所有通道
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
