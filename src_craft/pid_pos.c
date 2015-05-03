/*
 * pid.c
 * PID
 * 创建者：死猫
 * 创建时间：2015.01.19
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.01
 */

#include "system_define.h"

#include "my_math.h"

#include "pid.h"

CALCU_TYPE pid_update(PID_Type* PIDx, CALCU_TYPE feedback)
{
	CALCU_TYPE error;
	CALCU_TYPE pterm, iterm, dterm;

 	error = _EXPECT - feedback;

	_IERROR += error * _DT;
	MY_CONSTRAIN(_IERROR, _LIMIT_I_DOWN, _LIMIT_I_UP)

	pterm = _KP * error;
	iterm = _KI * _IERROR;
	dterm = _KD * (error - _ERROR1) / _DT;
	_ERROR1 = error;

	_DATAOUT = pterm + iterm + dterm;

//	MY_CONSTRAIN(_DATAOUT, _LIMIT_O_DOWN, _LIMIT_O_UP)
	return _DATAOUT;
}
