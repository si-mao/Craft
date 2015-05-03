/*
 * stm32_delay.c
 * 延迟
 * 创建者：死猫
 * 创建时间：2014.xx.xx
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.14
 */

// 适用于STM32F103ZE 72MHz

#include "system_define.h"
#include "stm32_systick.h"

__asm void delayms(uint32_t timems)
{
									  //CPU跳转时会产生等待flash周期，所以仿真与实测结果不同
	                  //调用4周期
DELAYMS_L2
	mov r2,#11999     //1周期
	
DELAYMS_L1
	subs r2,r2,#1     //1周期
	bgt DELAYMS_L1    //3周期
	
	subs r0,r0,#1     //1周期
	bgt DELAYMS_L2    //3周期
	
	bx lr             //3周期
}

/*
void delayms(uint32_t timems)
{
	uint32_t time_end;
	timems *= 10;
	time_end = get_systick();
	time_end += timems;
	while(get_systick() < time_end);
}
*/
/*
void delayms(uint32_t timems)
{
	uint32_t time1ms;
	while(timems--)
	{
		time1ms = 1663;
		while(time1ms--);
	}
}
*/

__asm void delayus(uint32_t timeus)
{
									//CPU跳转时会产生等待flash周期，所以仿真与实测结果不同
	                //调用4周期
	mov r2,#12      //1周期
	mul r0,r0,r2    //1周期
	subs r0,r0,#10  //1周期
DELAYUS_L1
	subs r0,r0,#1   //1周期
	bgt DELAYUS_L1  //3周期
	bx lr           //3周期
}

void delay100ns(uint32_t times)
{
	while(times--);
}
