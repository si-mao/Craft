/*
 * stm32_pwm.h
 * PWM����ģ��
 * �����ߣ���è
 * ����ʱ�䣺2015.01.13
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.03
 */
 
/*
 * STM32F103
 * ʹ����Դ��
 *     TIM3, PC[6..9]
 */
 
#ifndef STM32_PWM_H_
#define STM32_PWM_H_

#ifdef __cplusplus
extern "C"{
#endif

extern void pwm_init(void);
extern void pwm_enable(void);
extern void pwm_disable(void);

//����0~1000,���1ms~2ms
extern void pwm_set(const int16_t *pwm4);
//���1ms
extern void pwm_set_zero(void);

#ifdef __cplusplus
};
#endif

#endif
