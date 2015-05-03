/*
 * imu_attitude.h
 * ��̬�����㷨
 * �����ߣ���è
 * ����ʱ�䣺2015.01.15
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.22
 */
 
#ifndef IMU_ATTITUDE_H_
#define IMU_ATTITUDE_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

extern void imu_attitude_init(void);
extern void imu_attitude(void);
extern void set_field(const CALCU_TYPE* data_A_G_M);

#ifdef __cplusplus
};
#endif

#endif
