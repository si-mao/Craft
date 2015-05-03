/*
 * craft_data.c
 * 数据定义
 * 创建者：死猫
 * 创建时间：2015.01.31
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.04.06
 */

#include "system_define.h"

#include "craft_data.h"

#include "pid.h"

int16_t rcData[RC_CHANS];
int16_t rcCommand[4];

conf_t conf;
flags_t flags;
craft_data_t craft_data;

void craft_data_init(void)
{
	uint8_t i;
	CALCU_TYPE Xtmp, ThrMIDtmp;

	//flags
	flags.power     = POWER_OFF;
	flags.run       = RUN_POWER_ON;
	flags.echo      = ECHO_NULL;
	flags.algori    = ALGORI_DEFAULT;
	flags.rc_recved = 0;
	flags.error     = 0;

	//craft_data
	for(i = 0; i < sizeof(craft_data_t); i++)
	{
		*((uint8_t *)&craft_data) = 0;
	}
	craft_data.Attitude_Quat_ENU[0] = 1;
	craft_data.Attitude_Quat[0] = 1;

	//rcData
	for(i = 0; i < RC_CHANS; i++)
	{
		rcData[i] = 0;
	}

	//rcCommand
	for(i = 0; i < 4; i++)
	{
		rcCommand[i] = 0;
	}
	
	//初始化数据
	
	//陀螺零偏
	conf.GYRO_zero[0] = 121.7813;
	conf.GYRO_zero[1] = -8.759766;
	conf.GYRO_zero[2] = -15.85156;
	
	//加速度零偏
	conf.ACC_zero[0] = 40.16797;
	conf.ACC_zero[1] = -114.3672;
	conf.ACC_zero[2] = 64.07813;
	
// 	//安装误差
// 	conf.AngleTrim[0] = -0.06159664;
// 	conf.AngleTrim[1] = -0.01677201;
	
	//曲线
	//油门曲线
#define ThrMID 450.0f
#define ThrEXP 0.35f
	for(i = 0; i < 11; i++)
	{
		Xtmp = i * 100 - ThrMID;
		ThrMIDtmp = ThrMID;
		if(Xtmp > 0)
		{
			ThrMIDtmp = 1000 - ThrMID;
		}
		conf.lookupThrottleRC[i] = Xtmp * (ThrEXP + Xtmp * Xtmp *(1.0f - ThrEXP) / (ThrMIDtmp * ThrMIDtmp)) + ThrMID;
	}
	
	
	//PID
// 	conf.pid[PITCH].kp     = 295.6f; conf.pid[PITCH].ki     = 429.9f; conf.pid[PITCH].kd     = 0.0f;
// 	conf.pid[ROLL].kp      = 295.6f; conf.pid[ROLL].ki      = 429.9f; conf.pid[ROLL].kd      = 0.0f;
// 	conf.pid[PID_LEVEL].kp = 806.1f; conf.pid[PID_LEVEL].ki = 1.4;    conf.pid[PID_LEVEL].kd = 100.0f;

	pid_set_dt(PID_PITCH,      0.001f);
	pid_set_dt(PID_ROLL,       0.001f);
	pid_set_dt(PID_YAW,        0.001f);

	pid_set_dt(PID_PITCH_RATE, 0.001f);
	pid_set_dt(PID_ROLL_RATE,  0.001f);
	pid_set_dt(PID_YAW_RATE,   0.001f);
	
	pid_set_pid(PID_PITCH,       100.0f,  400.0f, 0.0f);
	pid_set_pid(PID_ROLL,        100.0f,  400.0f, 0.0f);
	pid_set_pid(PID_YAW,         0.0f,    0.0f, 0.0f);
	pid_set_pid(PID_LEVEL,       400.0f,  140.0f, 100.0f);



// 	pid_set_dt(PID_PITCH,      0.005f);
// 	pid_set_dt(PID_ROLL,       0.005f);
// 	pid_set_dt(PID_YAW,        0.005f);

// 	pid_set_dt(PID_PITCH_RATE, 0.001f);
// 	pid_set_dt(PID_ROLL_RATE,  0.001f);
// 	pid_set_dt(PID_YAW_RATE,   0.001f);
// 	
// 	pid_set_pid(PID_PITCH,       3.5f,  0.5f, 0.0f);
// 	pid_set_pid(PID_ROLL,        3.5f,  0.5f, 0.0f);
// 	pid_set_pid(PID_YAW,         0.0f,  0.0f, 0.0f);
// 	
// 	pid_set_pid(PID_PITCH_RATE, 200.0f,  0.0f, 0.0f);
// 	pid_set_pid(PID_ROLL_RATE,  200.0f,  0.0f, 0.0f);
// 	pid_set_pid(PID_YAW_RATE,   50.0f, 25.0f, 0.0f);
// 	
// 	pid_set_limit_i(PID_PITCH,      -20.0f * 3.14f / 180.0f, 20.0f * 3.14f / 180.0f);
// 	pid_set_limit_i(PID_ROLL,       -20.0f * 3.14f / 180.0f, 20.0f * 3.14f / 180.0f);
// 	pid_set_limit_i(PID_YAW,        -360.0f * 3.14f / 180.0f, 360.0f * 3.14f / 180.0f);
// 	pid_set_limit_i(PID_PITCH_RATE, -100.0f * 3.14f / 180.0f, 100.0f * 3.14f / 180.0f);
// 	pid_set_limit_i(PID_ROLL_RATE,  -100.0f * 3.14f / 180.0f, 100.0f * 3.14f / 180.0f);
// 	pid_set_limit_i(PID_YAW_RATE,   -500.0f * 3.14f / 180.0f, 500.0f * 3.14f / 180.0f);

// 	pid_reset_data(PID_PITCH     );
// 	pid_reset_data(PID_ROLL      );
// 	pid_reset_data(PID_YAW       );
// 	pid_reset_data(PID_PITCH_RATE);
// 	pid_reset_data(PID_ROLL_RATE );
// 	pid_reset_data(PID_YAW_RATE  );
}
