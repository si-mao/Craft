/*
 * stm32_timer.c
 * 定时器模块
 * 创建者：死猫
 * 创建时间：2015.01.16
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.02.13
 */

#include "system_define.h"
#include "stm32_config.h"

#if (TIM2_EN == 1)

void timer2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);         //使能定时器时钟
	TIM_DeInit(TIM2);                                            //将TIM初始化为复位值
	TIM_InternalClockConfig(TIM2);                               //配置TIM内部时钟
	TIM_BaseInitStructure.TIM_Period = TIM2_PERIOD;              //设置自动重载寄存器值为最大值, f_cycle = f_clk/(PERIOD + 1)
	TIM_BaseInitStructure.TIM_Prescaler = TIM2_PRESCALER;        //设置预分频系数, f_clk = 72MHz/(PRESCALER + 1)
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		       //此参数只适合于TIM1和TIM8
	TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure);              //根据指定参数初始化TIM时间基数寄存器
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                        //清中断，以免一启用中断后立即产生中断
	TIM_ARRPreloadConfig(TIM2, DISABLE);                         //禁止ARR 预装载缓冲器
#if (TIM2_INIT_EN == 1)
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                   //使能TIM中断源
#endif
	TIM_Cmd(TIM2, DISABLE);                                      //TIM总开关：关闭
}

#endif

#if (TIM3_EN == 1)

void timer3_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);         //使能定时器时钟
	TIM_DeInit(TIM3);                                            //将TIM初始化为复位值
	TIM_InternalClockConfig(TIM3);                               //配置TIM内部时钟
	TIM_BaseInitStructure.TIM_Period = TIM3_PERIOD;              //设置自动重载寄存器值为最大值, f_cycle = f_clk/(PERIOD + 1)
	TIM_BaseInitStructure.TIM_Prescaler = TIM3_PRESCALER;        //设置预分频系数, f_clk = 72MHz/(PRESCALER + 1)
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		       //此参数只适合于TIM1和TIM8
	TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);              //根据指定参数初始化TIM时间基数寄存器
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);                        //清中断，以免一启用中断后立即产生中断
	TIM_ARRPreloadConfig(TIM3, DISABLE);                         //禁止ARR 预装载缓冲器
#if (TIM3_INIT_EN == 1)
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
#endif
	TIM_Cmd(TIM3, DISABLE);                                      //TIM总开关：关闭
}

#endif

#if (TIM4_EN == 1)

void timer4_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_DeInit(TIM4);                                            //将TIM初始化为复位值
	TIM_InternalClockConfig(TIM4);                               //配置TIM内部时钟
	TIM_BaseInitStructure.TIM_Period = TIM4_PERIOD;              //设置自动重载寄存器值为最大值, f_cycle = f_clk/(PERIOD + 1)
	TIM_BaseInitStructure.TIM_Prescaler = TIM4_PRESCALER;        //设置预分频系数, f_clk = 72MHz/(PRESCALER + 1)
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		       //此参数只适合于TIM1和TIM8
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure);              //根据指定参数初始化TIM时间基数寄存器
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);                        //清中断，以免一启用中断后立即产生中断
	TIM_ARRPreloadConfig(TIM4, DISABLE);                         //禁止ARR 预装载缓冲器
#if (TIM4_INIT_EN == 1)
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
#endif
	TIM_Cmd(TIM4, DISABLE);                                      //TIM总开关：关闭
}

#endif

#if (TIM5_EN == 1)

void timer5_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_DeInit(TIM5);                                            //将TIM初始化为复位值
	TIM_InternalClockConfig(TIM5);                               //配置TIM内部时钟
	TIM_BaseInitStructure.TIM_Period = TIM5_PERIOD;              //设置自动重载寄存器值为最大值, f_cycle = f_clk/(PERIOD + 1)
	TIM_BaseInitStructure.TIM_Prescaler = TIM5_PRESCALER;        //设置预分频系数, f_clk = 72MHz/(PRESCALER + 1)
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //时钟分割为0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		       //此参数只适合于TIM1和TIM8
	TIM_TimeBaseInit(TIM5, &TIM_BaseInitStructure);              //根据指定参数初始化TIM时间基数寄存器
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);                        //清中断，以免一启用中断后立即产生中断
	TIM_ARRPreloadConfig(TIM5, DISABLE);                         //禁止ARR 预装载缓冲器
#if (TIM5_INIT_EN == 1)
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
#endif
	TIM_Cmd(TIM5, DISABLE);                                      //TIM总开关：关闭
}

#endif

void timer_init(void)
{
#if (TIM2_EN == 1)
	timer2_init();
#endif

#if (TIM3_EN == 1)
	timer3_init();
#endif

#if (TIM4_EN == 1)
	timer4_init();
#endif

#if (TIM5_EN == 1)
	timer5_init();
#endif
}

