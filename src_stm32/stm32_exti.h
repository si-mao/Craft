/*
 * stm32_exti.h
 * �ⲿ�ж�ģ��
 * �����ߣ���è
 * ����ʱ�䣺2015.01.23
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.01.24
 */
 
#ifndef STM32_EXIT_H_
#define STM32_EXIT_H_

#ifdef __cplusplus
extern "C"{
#endif

#define EXTI_INTER_EN  {EXTI_LINE0_EN}

#define EXTI_LINE0_EN  { EXTI->PR |= EXTI_Line0; EXTI->IMR |= EXTI_Line0; }
#define EXTI_LINE0_DIS { EXTI->IMR &= ~EXTI_Line0; }


	
extern void exti_init(void);

#ifdef __cplusplus
};
#endif

#endif

