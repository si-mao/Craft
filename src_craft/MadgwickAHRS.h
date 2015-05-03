/*
 * MadgwickAHRS.h
 * MadgwickAHRS ��̬�㷨
 * �����ߣ���è
 * ����ʱ�䣺2015.03.01
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.03.01
 */

//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

extern volatile float beta;				// algorithm gain
extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame


void MadgwickAHRSupdate(CALCU_TYPE* dst_attitude_quat, const CALCU_TYPE* norm_data_A_G_M);

extern void MadgwickAHRSModeFast(void);
extern void MadgwickAHRSModeNormal(void);

#ifdef __cplusplus
};
#endif

#endif
