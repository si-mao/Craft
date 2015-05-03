/*
 * imu_sensor.h
 * 获取传感器及坐标变换
 * 创建者：死猫
 * 创建时间：2015.03.22
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.21
 */

#ifndef IMU_SENSOR_H_
#define IMU_SENSOR_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

extern void imu_sensor_get_gyro_acc(int16_t *dst_gyro, int16_t *dst_acc);
extern void imu_sensor_get_gyro_acc_mag(int16_t *dst_gyro, int16_t *dst_acc, int16_t *dst_mag);

#ifdef __cplusplus
};
#endif

#endif
