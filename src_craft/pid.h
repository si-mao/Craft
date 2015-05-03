/*
 * pid.h
 * PID
 * 创建者：死猫
 * 创建时间：2015.01.19
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.01
 */
 
#ifndef PID_H_
#define PID_H_

#include "system_define.h"

#include "craft_type.h"

#ifdef __cplusplus
extern "C"{
#endif

extern CALCU_TYPE pid_update(PID_Type* PIDx, CALCU_TYPE feedback);

#define _KP            PIDx -> kp
#define _KI            PIDx -> ki
#define _KD            PIDx -> kd
#define _DT            PIDx -> dt
#define _LIMIT_O_UP    PIDx -> limit_o_up
#define _LIMIT_O_DOWN  PIDx -> limit_o_down
#define _LIMIT_I_UP    PIDx -> limit_i_up
#define _LIMIT_I_DOWN  PIDx -> limit_i_down
#define _EXPECT        PIDx -> expect
#define _ERROR1        PIDx -> error1
#define _IERROR        PIDx -> iError
#define _DATAOUT       PIDx -> data_out

__INLINE void pid_set_pid(PID_Type* PIDx, 
				CALCU_TYPE kp, CALCU_TYPE ki, CALCU_TYPE kd)
{
	_KP = kp;
	_KI = ki;
	_KD = kd;
}

__INLINE void pid_set_dt(PID_Type* PIDx, 
				CALCU_TYPE dt)
{
	_DT = dt;
}

// __INLINE void pid_set_limit_o(PID_Type* PIDx, 
// 				CALCU_TYPE limit_o_down, CALCU_TYPE limit_o_up)
// {
// 	_LIMIT_O_UP = limit_o_up;
// 	_LIMIT_O_DOWN = limit_o_down;
// }

__INLINE void pid_set_limit_i(PID_Type* PIDx,
				CALCU_TYPE limit_i_down, CALCU_TYPE limit_i_up)
{
	_LIMIT_I_UP = limit_i_up;
	_LIMIT_I_DOWN = limit_i_down;
}

__INLINE void pid_reset_data(PID_Type* PIDx)
{
	_EXPECT = 0;
	_ERROR1 = 0;
	_IERROR = 0;
}

__INLINE void pid_set_expect(PID_Type* PIDx, CALCU_TYPE expect)
{
	_EXPECT = expect;
}

#ifdef __cplusplus
};
#endif

#endif
