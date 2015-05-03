/*
 * pid.c
 * PID
 * 创建者：死猫
 * 创建时间：2015.01.19
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.01.19
 */
 
#include "system_define.h"

#define PID_COUNT 4  //PID个数

#define _KP          pid[id].kp
#define _KI          pid[id].ki
#define _KD          pid[id].kd
#define _LIMIT_UP    pid[id].limit_up
#define _LIMIT_DOWN  pid[id].limit_down
#define _EXPECT      pid[id].expect
#define _DATA_OUT    pid[id].data_out
#define _ERROR1      pid[id].error1
#define _ERROR2      pid[id].error2

 typedef struct _PIDType
{
	CALCU_TYPE kp;
	CALCU_TYPE ki;
	CALCU_TYPE kd;
	CALCU_TYPE limit_up;
	CALCU_TYPE limit_down;
	CALCU_TYPE expect;
	CALCU_TYPE data_out;
	CALCU_TYPE error1;
	CALCU_TYPE error2;
}PIDType;

PIDType pid[PID_COUNT];

void pid_init(uint8_t id, 
				CALCU_TYPE kp, CALCU_TYPE ki, CALCU_TYPE kd, 
				CALCU_TYPE limit_up, CALCU_TYPE limit_down)
{
	_KP = kp;
	_KI = ki;
	_KD = kd;
	_LIMIT_UP = limit_up;
	_LIMIT_DOWN = limit_down;
	_EXPECT = 0;
	_DATA_OUT = 0;
	_ERROR1 = 0;
	_ERROR2 = 0;
}

void pid_set_pid(uint8_t id, CALCU_TYPE kp, CALCU_TYPE ki, CALCU_TYPE kd)
{
	_KP = kp;
	_KI = ki;
	_KD = kd;
}

void pid_set_expect(uint8_t id, CALCU_TYPE expect)
{
	_EXPECT = expect;
}

void pid_clear_data_out(uint8_t id)
{
	_DATA_OUT = 0;
	_ERROR1 = 0;
	_ERROR2 = 0;
}

CALCU_TYPE pid_calculate(uint8_t id, CALCU_TYPE feedback)
{
	CALCU_TYPE error;
	CALCU_TYPE deltaout;
	error = _EXPECT - feedback;
	deltaout = (_KP + _KI + _KD) * error - (_KP + _KD * 2) * _ERROR1 + _KD * _ERROR2;
	_ERROR2 = _ERROR1;
	_ERROR1 = error;
	_DATA_OUT = _DATA_OUT + deltaout;
	if(_DATA_OUT > _LIMIT_UP)
	{
		_DATA_OUT = _LIMIT_UP;
	}else if(_DATA_OUT < _LIMIT_DOWN){
		_DATA_OUT = _LIMIT_DOWN;
	}
	return _DATA_OUT;
}
