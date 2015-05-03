/*
 * craft_type.h
 * ��������
 * �����ߣ���è
 * ����ʱ�䣺2015.03.22
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.03.23
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
	uint8_t rc_recved;   //����������־, ���ж�RC��ʱ
	uint32_t error;      //�������
}flags_t;

typedef struct
{
	PID_Type PID[PID_ITEMS];   //PID����
	CALCU_TYPE GYRO_zero[3];      //GYRO��ƫ
	CALCU_TYPE ACC_zero[3];       //ACC��ƫ
	CALCU_TYPE AngleTrim[2];      //ˮƽ�ᴫ������װ�������
	CALCU_TYPE lookupThrottleRC[11];
}conf_t;

typedef struct
{
	//Sensor data
	
	int16_t    GYRO_raw[3];       //��ʼ����
	CALCU_TYPE GYRO_physics[3];   //��������
	CALCU_TYPE GYRO_smooth[3];    //ƽ������
	//CALCU_TYPE GYRO_norm[3];    //Not use
	CALCU_TYPE GYRO_sum[3];
	uint32_t GYRO_sum_count;


	int16_t    ACC_raw[3];        //��ʼ����
	CALCU_TYPE ACC_physics[3];    //��������
	CALCU_TYPE ACC_smooth[3];     //ƽ������
	CALCU_TYPE ACC_norm[3];       //��һ������

	int16_t    MAG_raw[3];        //��ʼ����
	CALCU_TYPE MAG_physics[3];    //��������
	//CALCU_TYPE MAG_smooth[3];   //Not use
	CALCU_TYPE MAG_norm[3];       //��һ������

	//Calibrate  ��ʼ��ʱΪ0, ����δУ׼
	//           У׼������Ϊ[1~CALIBRATE_LEVEL_X]
	//           У׼��Ϻ�Ϊ-1, ����У׼����
	int16_t   Calibrate_GYRO;
	int16_t   Calibrate_ACC;
	int16_t   Calibrate_TRIM;

	//PWM data
	int16_t pwm[4];

	//Attitude data
	CALCU_TYPE Attitude_Quat_ENU[4];       //����ڶ���������ϵ����̬��Ԫ��
	CALCU_TYPE Attitude_Quat[4];           //����õ�����̬��Ԫ��
	CALCU_TYPE Attitude_Eular[3];          //����õ�����̬ŷ����
	CALCU_TYPE Attitude_Eular_ENU_INIT[3]; //��ʼ��̬ŷ����

	//Control data
	CALCU_TYPE Attitude_Eular_Hold[4];     //����ŷ����

	//Middle/debug data
	CALCU_TYPE axisPID[3];  //pidֵ att_x, att_y, att_z
	CALCU_TYPE ratePID[3];  //λ�û������pid���
}craft_data_t;

#ifdef __cplusplus
};
#endif

#endif
