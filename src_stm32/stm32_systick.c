/*
 * stm32_systick.h
 * 时间记录
 * 创建者：死猫
 * 创建时间：2015.03.13
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.03.13
 */

#include "system_define.h"
#include "stm32_systick.h"

volatile uint32_t systick_value;  //0xFFFFFFFF * 1ms = 4294967295ms =~= 1193hour 

void systick_init(void)
{
	//72MHz / 8 = 9MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick -> CTRL = 0x02;  //开启SysTick中断
	SysTick -> LOAD = 9000;  //重载值
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
