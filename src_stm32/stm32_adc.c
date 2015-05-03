/*
 * stm32_adc.c
 * adc
 * 创建者：死猫
 * 创建时间：2015.03.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.15
 */

#include "system_define.h"

void adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	  /* ADC1 configuration ------------------------------------------------------*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_DeInit(ADC1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

}

void adc_start_convert(uint8_t ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_13Cycles5);
// 	ADC1->SMPR2 = (ADC1->SMPR2 & 0XFFFFFFF8) | 7;  //直接操作寄存器方式存在通道串扰
// 	ADC1->SQR3 = (ADC1->SQR3 & 0XFFFFFFE0) | ch;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//0没有结束
uint8_t adc_is_convert_finish(void)
{
	if((ADC1->SR&0x02) == 0)
		return 0;
	else
		return 1;
}

uint16_t adc_get_value(void)
{
	return ADC1 -> DR;
}
