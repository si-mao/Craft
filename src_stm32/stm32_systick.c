/*
 * stm32_systick.h
 * ʱ���¼
 * �����ߣ���è
 * ����ʱ�䣺2015.03.13
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.03.13
 */

#include "system_define.h"
#include "stm32_systick.h"

volatile uint32_t systick_value;  //0xFFFFFFFF * 1ms = 4294967295ms =~= 1193hour 

void systick_init(void)
{
	//72MHz / 8 = 9MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick -> CTRL = 0x02;  //����SysTick�ж�
	SysTick -> LOAD = 9000;  //����ֵ
	systick_value = 0;
	SYSTICK_START
}

__weak void systick_callback(void)
{
}

void SysTick_Handler(void)
{
	systick_value++;
	systick_callback();
}
