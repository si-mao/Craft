/*
 * stm32_pwm.h
 * PWM产生模块
 * 创建者：死猫
 * 创建时间：2015.01.13
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.03
 */
 
/*
 * STM32F103
 * 使用资源：
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

//输入0~1000,输出1ms~2ms
extern void pwm_set(const int16_t *pwm4);
//输出1ms
extern void pwm_set_zero(void);

#ifdef __cplusplus
};
#endif

#endif
