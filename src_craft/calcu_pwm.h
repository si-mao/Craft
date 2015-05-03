/*
 * calcu_pwm.h
 * 控制解算算法
 * 创建者：死猫
 * 创建时间：2015.01.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.01.18
 */

#ifndef CALCU_PWM_H_
#define CALCU_PWM_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

extern void calcu_pwm_init(void);
extern void calcu_pwm_reset(void);

extern void calcu_pwm_eular(int16_t* dst_pwm, const CALCU_TYPE* attitude_eular);
extern void calcu_pwm(void);

extern void calcu_pid_rate(void);
extern void calcu_pid_angle(void);
extern void pid_to_pwm(void);

#ifdef __cplusplus
};
#endif

#endif
