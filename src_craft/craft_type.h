/*
 * craft_type.h
 * 数据类型
 * 创建者：死猫
 * 创建时间：2015.03.22
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.03.23
 */

#ifndef CRAFT_TYPE_H_
#define CRAFT_TYPE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#include "craft_conf.h"

#define RC_CHANS 8
#define PID_ITEMS 7

#define PID_ROLL_ID       0
#define PID_PITCH_ID      1
#define PID_YAW_ID        2
#define PID_PITCH_RATE_ID 3
#define PID_ROLL_RATE_ID  4
#define PID_YAW_RATE_ID   5
#define PID_LEVEL_ID      6

#define PID_ROLL       (&conf.PID[PID_ROLL_ID])
#define PID_PITCH      (&conf.PID[PID_PITCH_ID])
#define PID_YAW        (&conf.PID[PID_YAW_ID])
#define PID_PITCH_RATE (&conf.PID[PID_PITCH_RATE_ID])
#define PID_ROLL_RATE  (&conf.PID[PID_ROLL_RATE_ID])
#define PID_YAW_RATE   (&conf.PID[PID_YAW_RATE_ID])
#define PID_LEVEL      (&conf.PID[PID_LEVEL_ID])

enum rc  //rc: int16_t
{
	ROLL = 0,
	PITCH,
	YAW,
	THROTTLE,
	AUX1,
	AUX2,
	AUX3,
	AUX4,
	AUX5,
	AUX6,
	AUX7,
	AUX8
};

enum power
{
	POWER_OFF = 0,
	POWER_ON
};

enum run
{
	RUN_POWER_ON = 0,
	RUN_IDLE,
	RUN_MOV,
	RUN_PWM,
	RUN_MAG_CALI,
	RUN_ALIGN
};

enum echo
{
	ECHO_NULL = 0,
	ECHO_CMD,
	ECHO_RAW,
	ECHO_CALI,
	ECHO_MERG,
	ECHO_ATT_PWM,
	ECHO_ATT_PID
};

enum algori
{
	ALGORI_DEFAULT = 0,
	ALGORI_AUX1,
	ALGORI_AUX2
};

typedef struct _PID_Type
{
	CALCU_TYPE kp;
	CALCU_TYPE ki;
	CALCU_TYPE kd;
	CALCU_TYPE dt;
// 	CALCU_TYPE limit_o_up;
// 	CALCU_TYPE limit_o_down;
	CALCU_TYPE limit_i_up;
	CALCU_TYPE limit_i_down;
	CALCU_TYPE expect;
	CALCU_TYPE error1;
	CALCU_TYPE iError;
	CALCU_TYPE data_out;
}PID_Type;

typedef struct
{
	uint8_t power;
	uint8_t run;
	uint8_t echo;
	uint8_t algori;
	uint8_t rc_recved;   //飞行启动标志, 以判断RC超时
	uint32_t error;      //出错代码
}flags_t;

typedef struct
{
	PID_Type PID[PID_ITEMS];   //PID参数
	CALCU_TYPE GYRO_zero[3];      //GYRO零偏
	CALCU_TYPE ACC_zero[3];       //ACC零偏
	CALCU_TYPE AngleTrim[2];      //水平轴传感器安装误差修正
	CALCU_TYPE lookupThrottleRC[11];
}conf_t;

typedef struct
{
	//Sensor data
	
	int16_t    GYRO_raw[3];       //初始数据
	CALCU_TYPE GYRO_physics[3];   //物理数据
	CALCU_TYPE GYRO_smooth[3];    //平滑数据
	//CALCU_TYPE GYRO_norm[3];    //Not use
	CALCU_TYPE GYRO_sum[3];
	uint32_t GYRO_sum_count;


	int16_t    ACC_raw[3];        //初始数据
	CALCU_TYPE ACC_physics[3];    //物理数据
	CALCU_TYPE ACC_smooth[3];     //平滑数据
	CALCU_TYPE ACC_norm[3];       //归一化数据

	int16_t    MAG_raw[3];        //初始数据
	CALCU_TYPE MAG_physics[3];    //物理数据
	//CALCU_TYPE MAG_smooth[3];   //Not use
	CALCU_TYPE MAG_norm[3];       //归一化数据

	//Calibrate  初始化时为0, 表明未校准
	//           校准过程中为[1~CALIBRATE_LEVEL_X]
	//           校准完毕后为-1, 表明校准结束
	int16_t   Calibrate_GYRO;
	int16_t   Calibrate_ACC;
	int16_t   Calibrate_TRIM;

	//PWM data
	int16_t pwm[4];

	//Attitude data
	CALCU_TYPE Attitude_Quat_ENU[4];       //相对于东北天坐标系的姿态四元数
	CALCU_TYPE Attitude_Quat[4];           //计算得到的姿态四元数
	CALCU_TYPE Attitude_Eular[3];          //计算得到的姿态欧拉角
	CALCU_TYPE Attitude_Eular_ENU_INIT[3]; //初始姿态欧拉角

	//Control data
	CALCU_TYPE Attitude_Eular_Hold[4];     //保持欧拉角

	//Middle/debug data
	CALCU_TYPE axisPID[3];  //pid值 att_x, att_y, att_z
	CALCU_TYPE ratePID[3];  //位置环算出的pid输出
}craft_data_t;

#ifdef __cplusplus
};
#endif

#endif
