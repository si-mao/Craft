/*
 * stm32_delay.c
 * �ӳ�
 * �����ߣ���è
 * ����ʱ�䣺2014.xx.xx
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.14
 */

// ������STM32F103ZE 72MHz

#include "system_define.h"
#include "stm32_systick.h"

__asm void delayms(uint32_t timems)
{
									  //CPU��תʱ������ȴ�flash���ڣ����Է�����ʵ������ͬ
	                  //����4����
DELAYMS_L2
	mov r2,#11999     //1����
	
DELAYMS_L1
	subs r2,r2,#1     //1����
	bgt DELAYMS_L1    //3����
	
	subs r0,r0,#1     //1����
	bgt DELAYMS_L2    //3����
	
	bx lr             //3����
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
									//CPU��תʱ������ȴ�flash���ڣ����Է�����ʵ������ͬ
	                //����4����
	mov r2,#12      //1����
	mul r0,r0,r2    //1����
	subs r0,r0,#10  //1����
DELAYUS_L1
	subs r0,r0,#1   //1����
	bgt DELAYUS_L1  //3����
	bx lr           //3����
}

void delay100ns(uint32_t times)
{
	while(times--);
}
