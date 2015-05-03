/*
 * imu_sensor.h
 * ��ȡ������������任
 * �����ߣ���è
 * ����ʱ�䣺2015.03.22
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.04.21
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
