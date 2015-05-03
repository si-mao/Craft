/*
 * stm32_pwm.c
 * PWM产生模块
 * 创建者：死猫
 * 创建时间：2015.01.13
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.03
 */

#include "system_define.h"
#include "stm32_config.h"

#if (PWM_EN == 1)

void pwm_init_register(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_OCStructInit(&TIM_OCInitStructure);                        //设置缺省值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //PWM 模式 1 输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //TIM 输出比较极性高
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //使能输出状态  需要PWM输出才需要这行代码
	TIM_OCInitStructure.TIM_Pulse = 0;                             //设置占空比


	/* PWM1 Mode configuration: Channel1 */
	TIM_OC1Init(PWM_TIMER, &TIM_OCInitStructure);                  //根据参数初始化PWM寄存器
	TIM_OC1PreloadConfig(PWM_TIMER, TIM_OCPreload_Disable);        //禁止 TIMx在 CCRx 上的预装载寄存器

	/* PWM2 Mode configuration: Channel2 */
	TIM_OC2Init(PWM_TIMER, &TIM_OCInitStructure);                  //根据参数初始化PWM寄存器
	TIM_OC2PreloadConfig(PWM_TIMER, TIM_OCPreload_Disable);        //禁止 TIMx在 CCRx 上的预装载寄存器

	/* PWM3 Mode configuration: Channel3 */
	TIM_OC3Init(PWM_TIMER, &TIM_OCInitStructure);                  //根据参数初始化PWM寄存器
	TIM_OC3PreloadConfig(PWM_TIMER, TIM_OCPreload_Disable);        //禁止 TIMx在 CCRx 上的预装载寄存器

	/* PWM4 Mode configuration: Channel4 */
	TIM_OC4Init(PWM_TIMER, &TIM_OCInitStructure);                  //根据参数初始化PWM寄存器
	TIM_OC4PreloadConfig(PWM_TIMER, TIM_OCPreload_Disable);        //禁止 TIMx在 CCRx 上的预装载寄存器
}

void pwm_init_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);  //使能各个端口时钟，重要！！
	
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
	//输入0~TIM_Period(9999),输出0%~100%
	TIM_SetCompare1(PWM_TIMER, compare4[0]);
	TIM_SetCompare2(PWM_TIMER, compare4[1]);
	TIM_SetCompare3(PWM_TIMER, compare4[2]);
	TIM_SetCompare4(PWM_TIMER, compare4[3]);
}

void pwm_set(const int16_t *pwm4)
{
	//输入0~1000,输出1ms~2ms
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
	//输出1ms
	uint16_t compare4[4];
	compare4[0] = 1000;
	compare4[1] = 1000;
	compare4[2] = 1000;
	compare4[3] = 1000;
	pwm_compare_set(compare4);
}

#endif
