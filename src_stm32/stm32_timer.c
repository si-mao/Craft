/*
 * stm32_timer.c
 * ��ʱ��ģ��
 * �����ߣ���è
 * ����ʱ�䣺2015.01.16
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.02.13
 */

#include "system_define.h"
#include "stm32_config.h"

#if (TIM2_EN == 1)

void timer2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);         //ʹ�ܶ�ʱ��ʱ��
	TIM_DeInit(TIM2);                                            //��TIM��ʼ��Ϊ��λֵ
	TIM_InternalClockConfig(TIM2);                               //����TIM�ڲ�ʱ��
	TIM_BaseInitStructure.TIM_Period = TIM2_PERIOD;              //�����Զ����ؼĴ���ֵΪ���ֵ, f_cycle = f_clk/(PERIOD + 1)
	TIM_BaseInitStructure.TIM_Prescaler = TIM2_PRESCALER;        //����Ԥ��Ƶϵ��, f_clk = 72MHz/(PRESCALER + 1)
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		       //�˲���ֻ�ʺ���TIM1��TIM8
	TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure);              //����ָ��������ʼ��TIMʱ������Ĵ���
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                        //���жϣ�����һ�����жϺ����������ж�
	TIM_ARRPreloadConfig(TIM2, DISABLE);                         //��ֹARR Ԥװ�ػ�����
#if (TIM2_INIT_EN == 1)
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                   //ʹ��TIM�ж�Դ
#endif
	TIM_Cmd(TIM2, DISABLE);                                      //TIM�ܿ��أ��ر�
}

#endif

#if (TIM3_EN == 1)

void timer3_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);         //ʹ�ܶ�ʱ��ʱ��
	TIM_DeInit(TIM3);                                            //��TIM��ʼ��Ϊ��λֵ
	TIM_InternalClockConfig(TIM3);                               //����TIM�ڲ�ʱ��
	TIM_BaseInitStructure.TIM_Period = TIM3_PERIOD;              //�����Զ����ؼĴ���ֵΪ���ֵ, f_cycle = f_clk/(PERIOD + 1)
	TIM_BaseInitStructure.TIM_Prescaler = TIM3_PRESCALER;        //����Ԥ��Ƶϵ��, f_clk = 72MHz/(PRESCALER + 1)
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		       //�˲���ֻ�ʺ���TIM1��TIM8
	TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);              //����ָ��������ʼ��TIMʱ������Ĵ���
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);                        //���жϣ�����һ�����жϺ����������ж�
	TIM_ARRPreloadConfig(TIM3, DISABLE);                         //��ֹARR Ԥװ�ػ�����
#if (TIM3_INIT_EN == 1)
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
#endif
	TIM_Cmd(TIM3, DISABLE);                                      //TIM�ܿ��أ��ر�
}

#endif

#if (TIM4_EN == 1)

void timer4_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_DeInit(TIM4);                                            //��TIM��ʼ��Ϊ��λֵ
	TIM_InternalClockConfig(TIM4);                               //����TIM�ڲ�ʱ��
	TIM_BaseInitStructure.TIM_Period = TIM4_PERIOD;              //�����Զ����ؼĴ���ֵΪ���ֵ, f_cycle = f_clk/(PERIOD + 1)
	TIM_BaseInitStructure.TIM_Prescaler = TIM4_PRESCALER;        //����Ԥ��Ƶϵ��, f_clk = 72MHz/(PRESCALER + 1)
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		       //�˲���ֻ�ʺ���TIM1��TIM8
	TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure);              //����ָ��������ʼ��TIMʱ������Ĵ���
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);                        //���жϣ�����һ�����жϺ����������ж�
	TIM_ARRPreloadConfig(TIM4, DISABLE);                         //��ֹARR Ԥװ�ػ�����
#if (TIM4_INIT_EN == 1)
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
#endif
	TIM_Cmd(TIM4, DISABLE);                                      //TIM�ܿ��أ��ر�
}

#endif

#if (TIM5_EN == 1)

void timer5_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_DeInit(TIM5);                                            //��TIM��ʼ��Ϊ��λֵ
	TIM_InternalClockConfig(TIM5);                               //����TIM�ڲ�ʱ��
	TIM_BaseInitStructure.TIM_Period = TIM5_PERIOD;              //�����Զ����ؼĴ���ֵΪ���ֵ, f_cycle = f_clk/(PERIOD + 1)
	TIM_BaseInitStructure.TIM_Prescaler = TIM5_PRESCALER;        //����Ԥ��Ƶϵ��, f_clk = 72MHz/(PRESCALER + 1)
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //ʱ�ӷָ�Ϊ0
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	//TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 		       //�˲���ֻ�ʺ���TIM1��TIM8
	TIM_TimeBaseInit(TIM5, &TIM_BaseInitStructure);              //����ָ��������ʼ��TIMʱ������Ĵ���
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);                        //���жϣ�����һ�����жϺ����������ж�
	TIM_ARRPreloadConfig(TIM5, DISABLE);                         //��ֹARR Ԥװ�ػ�����
#if (TIM5_INIT_EN == 1)
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
#endif
	TIM_Cmd(TIM5, DISABLE);                                      //TIM�ܿ��أ��ر�
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

