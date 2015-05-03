/*
 * imu_sensor.h
 * 获取传感器及坐标变换
 * 创建者：死猫
 * 创建时间：2015.03.22
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.21
 */

#include "system_define.h"
#include "craft_data.h"
#include "craft_conf.h"
#include "mpu9250.h"

void imu_sensor_calibrate(int16_t *gyro, int16_t *acc)
{
	static int32_t g[3];
	static int32_t a[3];

//########################### Gyro ###########################//
	if(craft_data.Calibrate_GYRO > 0)
	{
		if(craft_data.Calibrate_GYRO == CALIBRATE_LEVEL_GYRO)
		{
			g[0] = 0;
			g[1] = 0;
			g[2] = 0;
		}
		g[0] += gyro[0];
		g[1] += gyro[1];
		g[2] += gyro[2];
		if(craft_data.Calibrate_GYRO == 1)
		{
			conf.GYRO_zero[0] = (CALCU_TYPE)g[0] / (CALCU_TYPE)CALIBRATE_LEVEL_GYRO;
			conf.GYRO_zero[1] = (CALCU_TYPE)g[1] / (CALCU_TYPE)CALIBRATE_LEVEL_GYRO;
			conf.GYRO_zero[2] = (CALCU_TYPE)g[2] / (CALCU_TYPE)CALIBRATE_LEVEL_GYRO;
			
			craft_data.Calibrate_GYRO = 0;  //校准完毕后Calibrate_X为-1
		}

		craft_data.Calibrate_GYRO--;
	}
//########################### Acc ###########################//
	if(craft_data.Calibrate_ACC > 0)
	{
		if(craft_data.Calibrate_ACC == CALIBRATE_LEVEL_ACC)
		{
			a[0] = 0;
			a[1] = 0;
			a[2] = 0;
		}
		a[0] += acc[0];
		a[1] += acc[1];
		a[2] += acc[2];
		if(craft_data.Calibrate_ACC == 1)
		{
			conf.ACC_zero[0] = (CALCU_TYPE)a[0] / (CALCU_TYPE)CALIBRATE_LEVEL_ACC;
			conf.ACC_zero[1] = (CALCU_TYPE)a[1] / (CALCU_TYPE)CALIBRATE_LEVEL_ACC;
			conf.ACC_zero[2] = (CALCU_TYPE)a[2] / (CALCU_TYPE)CALIBRATE_LEVEL_ACC;
			conf.ACC_zero[2] -= ACC_ADC_1G;
			craft_data.Calibrate_ACC = 0;
		}

		craft_data.Calibrate_ACC--;
	}
}

void imu_sensor_get_gyro_acc(int16_t *dst_gyro, int16_t *dst_acc)
{
	mpu9250_get_gyro_acc((uint16_t *)dst_gyro, (uint16_t *)dst_acc);
	imu_sensor_calibrate(dst_gyro, dst_acc);
}

void imu_sensor_get_gyro_acc_mag(int16_t *dst_gyro, int16_t *dst_acc, int16_t *dst_mag)
{
	mpu9250_get_gyro_acc_mag((uint16_t *)dst_gyro, (uint16_t *)dst_acc, (uint16_t *)dst_mag);
	imu_sensor_calibrate(dst_gyro, dst_acc);
}
