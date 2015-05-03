/*
 * calcu_pwm.h
 * ���ƽ����㷨
 * �����ߣ���è
 * ����ʱ�䣺2015.01.15
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.01.18
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
