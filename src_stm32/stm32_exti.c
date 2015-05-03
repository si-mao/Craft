/*
 * stm32_exti.c
 * 外部中断模块
 * 创建者：死猫
 * 创建时间：2015.01.23
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.01.24
 */
 
#include "system_define.h"

void EXTI_Init_My(EXTI_InitTypeDef* EXTI_InitStruct)
{
  uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
  assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

  tmp = (uint32_t)EXTI_BASE;
	tmp += EXTI_InitStruct->EXTI_Mode;
     
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* Clear EXTI line configuration */
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;

    *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;
	}
	else
	{
		/* Disable the selected external lines */
		*(__IO uint32_t *) tmp &= ~EXTI_InitStruct->EXTI_Line;
	}

	/* Clear Rising Falling edge configuration */
	EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
	EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;
	
	/* Select the trigger for the selected external interrupts */
	if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
	{
		/* Rising Falling edge */
		EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
		EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
	}
	else
	{
		tmp = (uint32_t)EXTI_BASE;
		tmp += EXTI_InitStruct->EXTI_Trigger;

		*(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;
	}
}

void exti_init_PA0(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;
	EXTI_ClearITPendingBit(EXTI_Line0);  //清中断

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //时钟时能

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);  //PA0

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //触发沿
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
 	EXTI_Init_My(&EXTI_InitStructure);
}

void exti_init(void)
{
	exti_init_PA0();
}
