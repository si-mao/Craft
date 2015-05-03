/*
 * MadgwickAHRS.h
 * MadgwickAHRS 姿态算法
 * 创建者：死猫
 * 创建时间：2015.03.01
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.03.01
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
