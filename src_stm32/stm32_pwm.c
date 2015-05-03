/*
 * stm32_pwm.c
 * PWM����ģ��
 * �����ߣ���è
 * ����ʱ�䣺2015.01.13
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.03
 */

#include "system_define.h"
#include "stm32_config.h"

#if (PWM_EN == 1)

void pwm_init_register(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_OCStructInit(&TIM_OCInitStructure);                        //����ȱʡֵ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //PWM ģʽ 1 ���
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //TIM ����Ƚϼ��Ը�
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //ʹ�����״̬  ��ҪPWM�������Ҫ���д���
	TIM_OCInitStructure.TIM_Pulse = 0;                             //����ռ�ձ�


	/* PWM1 Mode configuration: Channel1 */
	TIM_OC1Init(PWM_TIMER, &TIM_OCInitStructure);                  //���ݲ�����ʼ��PWM�Ĵ���
	TIM_OC1PreloadConfig(PWM_TIMER, TIM_OCPreload_Disable);        //��ֹ TIMx�� CCRx �ϵ�Ԥװ�ؼĴ���

	/* PWM2 Mode configuration: Channel2 */
	TIM_OC2Init(PWM_TIMER, &TIM_OCInitStructure);                  //���ݲ�����ʼ��PWM�Ĵ���
	TIM_OC2PreloadConfig(PWM_TIMER, TIM_OCPreload_Disable);        //��ֹ TIMx�� CCRx �ϵ�Ԥװ�ؼĴ���

	/* PWM3 Mode configuration: Channel3 */
	TIM_OC3Init(PWM_TIMER, &TIM_OCInitStructure);                  //���ݲ�����ʼ��PWM�Ĵ���
	TIM_OC3PreloadConfig(PWM_TIMER, TIM_OCPreload_Disable);        //��ֹ TIMx�� CCRx �ϵ�Ԥװ�ؼĴ���

	/* PWM4 Mode configuration: Channel4 */
	TIM_OC4Init(PWM_TIMER, &TIM_OCInitStructure);                  //���ݲ�����ʼ��PWM�Ĵ���
	TIM_OC4PreloadConfig(PWM_TIMER, TIM_OCPreload_Disable);        //��ֹ TIMx�� CCRx �ϵ�Ԥװ�ؼĴ���
}

void pwm_init_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);  //ʹ�ܸ����˿�ʱ�ӣ���Ҫ����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

#if (PWM_TIMER_REMAP_EN == 1)
	GPIO_PinRemapConfig(PWM_TIMER_REMAP, ENABLE);
#endif
}

void pwm_init(void)
{
	pwm_init_register();
	pwm_init_gpio();
}

void pwm_enable(void)
{
	TIM_CtrlPWMOutputs(PWM_TIMER, ENABLE);
	TIM_Cmd(PWM_TIMER, ENABLE);
}

void pwm_disable(void)
{
	TIM_CtrlPWMOutputs(PWM_TIMER, DISABLE);
	TIM_Cmd(PWM_TIMER, DISABLE);
}

void pwm_compare_set(uint16_t *compare4)
{
	//����0~TIM_Period(9999),���0%~100%
	TIM_SetCompare1(PWM_TIMER, compare4[0]);
	TIM_SetCompare2(PWM_TIMER, compare4[1]);
	TIM_SetCompare3(PWM_TIMER, compare4[2]);
	TIM_SetCompare4(PWM_TIMER, compare4[3]);
}

void pwm_set(const int16_t *pwm4)
{
	//����0~1000,���1ms~2ms
	uint16_t compare4[4];
	int i;
	for(i = 0; i < 4; i++)
	{
		if(pwm4[i] < 0) compare4[i] = 1000;
		else if(pwm4[i] > 1000) compare4[i] = 2000;
		else compare4[i] = 1000 + pwm4[i];
	}
	pwm_compare_set(compare4);
}

void pwm_set_zero(void)
{
	//���1ms
	uint16_t compare4[4];
	compare4[0] = 1000;
	compare4[1] = 1000;
	compare4[2] = 1000;
	compare4[3] = 1000;
	pwm_compare_set(compare4);
}

#endif
