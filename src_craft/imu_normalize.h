/*
 * imu_normalize.c
 * ��һ���㷨
 * �����ߣ���è
 * ����ʱ�䣺2015.02.01
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.26
 */

#ifndef IMU_NORMALIZE_H_
#define IMU_NORMALIZE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

extern CALCU_TYPE imu_normalize(CALCU_TYPE* dst_A_G_M, const CALCU_TYPE* src_A_G_M);

#ifdef __cplusplus
};
#endif

#endif
