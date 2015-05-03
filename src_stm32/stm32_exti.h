/*
 * stm32_exti.h
 * 外部中断模块
 * 创建者：死猫
 * 创建时间：2015.01.23
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.01.24
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

