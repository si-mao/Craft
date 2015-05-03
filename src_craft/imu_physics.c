/*
 * imu_physics.c
 * 物理量纲化
 * 创建者：死猫
 * 创建时间：2015.01.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.29
 */

#include "system_define.h"

#include "craft_data.h"

//X_New = X_K * (X_Old + X_B);

//加速度
int16_t    Cali_Acc_B[3];
CALCU_TYPE Cali_Acc_K[3];

//陀螺仪
int16_t    Cali_Gyro_B[3];
CALCU_TYPE Cali_Gyro_K[3];

//磁场
int16_t    Cali_Magnet_B[3];
CALCU_TYPE Cali_Magnet_K[3];

//磁场校准临时变量
int16_t MagnetCalibrate_XBbYBbZBb[6];

#define SENSOR_GYRO_MAX   250.0f
#define SENSOR_GYRO_RANGE 32767.0f
#define SENSOR_GYRO_SCALE (SENSOR_GYRO_MAX * 3.1415926f / 180.0f / SENSOR_GYRO_RANGE)   //??/s?????/s
#define SENSOR_ACC_MAX  (16.0f * 9.8f)
#define SENSOR_ACC_RANGE 32767.0f
#define SENSOR_ACC_SCALE (SENSOR_ACC_MAX / SENSOR_ACC_RANGE)

#define MAG_ASAX 173 //0xAD
#define MAG_ASAY 174 //0xAE
#define MAG_ASAZ 164 //0xA4

#define MAG_ADJX 1.17578125 //((ASA-128) * 0.5 / 128 + 1)
#define MAG_ADJY 1.1796875
#define MAG_ADJZ 1.140625

void imu_physics_init(void)
{
	Cali_Acc_B[0] = 0;
	Cali_Acc_B[1] = 0;
	Cali_Acc_B[2] = 0;
	
	Cali_Acc_K[0] = SENSOR_ACC_SCALE;
	Cali_Acc_K[1] = SENSOR_ACC_SCALE;
	Cali_Acc_K[2] = SENSOR_ACC_SCALE;

	Cali_Gyro_B[0] = -33;
	Cali_Gyro_B[1] = -240;
	Cali_Gyro_B[2] = 30;

	Cali_Gyro_K[0] = SENSOR_GYRO_SCALE;
	Cali_Gyro_K[1] = SENSOR_GYRO_SCALE;
	Cali_Gyro_K[2] = SENSOR_GYRO_SCALE;
	
	Cali_Magnet_B[0] = -50;
	Cali_Magnet_B[1] = 50;
	Cali_Magnet_B[2] = -310;
	
	Cali_Magnet_K[0] = MAG_ADJX;
	Cali_Magnet_K[1] = MAG_ADJY;
	Cali_Magnet_K[2] = MAG_ADJZ;

}

void imu_physics_gyro(CALCU_TYPE* dst_data, const int16_t* raw_data)
{
	dst_data[0] = SENSOR_GYRO_SCALE * ((CALCU_TYPE)raw_data[0] - conf.GYRO_zero[0]);
	dst_data[1] = SENSOR_GYRO_SCALE * ((CALCU_TYPE)raw_data[1] - conf.GYRO_zero[1]);
	dst_data[2] = SENSOR_GYRO_SCALE * ((CALCU_TYPE)raw_data[2] - conf.GYRO_zero[2]);
}

void imu_physics_acc(CALCU_TYPE* dst_data, const int16_t* raw_data)
{
	dst_data[0] = SENSOR_ACC_SCALE * ((CALCU_TYPE)raw_data[0] - conf.ACC_zero[0]);
	dst_data[1] = SENSOR_ACC_SCALE * ((CALCU_TYPE)raw_data[1] - conf.ACC_zero[1]);
	dst_data[2] = SENSOR_ACC_SCALE * ((CALCU_TYPE)raw_data[2] - conf.ACC_zero[2]);
}

void imu_physics_mag(CALCU_TYPE* dst_data, const int16_t* raw_data)
{
	dst_data[0] = Cali_Magnet_K[0] * (CALCU_TYPE)(raw_data[0] + Cali_Magnet_B[0]);
	dst_data[1] = Cali_Magnet_K[1] * (CALCU_TYPE)(raw_data[1] + Cali_Magnet_B[1]);
	dst_data[2] = Cali_Magnet_K[2] * (CALCU_TYPE)(raw_data[2] + Cali_Magnet_B[2]);
}

void imu_physics_calibrate_ACC(void)
{
}

void magn_cali_begin(void)
{
	MagnetCalibrate_XBbYBbZBb[0] = -1000;
	MagnetCalibrate_XBbYBbZBb[1] = 1000;
	MagnetCalibrate_XBbYBbZBb[2] = -1000;
	MagnetCalibrate_XBbYBbZBb[3] = 1000;
	MagnetCalibrate_XBbYBbZBb[4] = -1000;
	MagnetCalibrate_XBbYBbZBb[5] = 1000;
}

void magn_cali_input(const int16_t* raw_M)
{
	uint8_t i;
	for(i = 0; i < 3; i++)
	{
		if(raw_M[i] > MagnetCalibrate_XBbYBbZBb[i * 2]) MagnetCalibrate_XBbYBbZBb[i * 2] = raw_M[i];
		else if(raw_M[i] < MagnetCalibrate_XBbYBbZBb[i * 2 + 1]) MagnetCalibrate_XBbYBbZBb[i * 2 + 1] = raw_M[i];
	}
}

void magn_cali_end(void)
{
	Cali_Magnet_K[0] = MAG_ADJX;
	Cali_Magnet_K[1] = MAG_ADJY;
	Cali_Magnet_K[2] = MAG_ADJZ;
	Cali_Magnet_B[0] = -(MagnetCalibrate_XBbYBbZBb[0] + MagnetCalibrate_XBbYBbZBb[1]) / 2;
	Cali_Magnet_B[1] = -(MagnetCalibrate_XBbYBbZBb[2] + MagnetCalibrate_XBbYBbZBb[3]) / 2;
	Cali_Magnet_B[2] = -(MagnetCalibrate_XBbYBbZBb[4] + MagnetCalibrate_XBbYBbZBb[5]) / 2;
}
