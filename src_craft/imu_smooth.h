/*
 * imu_smooth.h
 * �⻬�㷨
 * �����ߣ���è
 * ����ʱ�䣺2015.01.15
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.22
 */
 
#ifndef IMU_SMOOTH_H_
#define IMU_SMOOTH_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif


extern void imu_smooth_init(void);
extern void imu_smooth(CALCU_TYPE* dst_A_G_M, const CALCU_TYPE* src_A_G_M, CALCU_TYPE a);


#ifdef __cplusplus
};
#endif

#endif
