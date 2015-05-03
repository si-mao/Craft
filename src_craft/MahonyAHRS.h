/*
 * MahonyAHRS.h
 * Mahony 姿态算法
 * 创建者：死猫
 * 创建时间：2015.02.24
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.02.24
 */

//=====================================================================================================
//
// Madgwick's implementation of MAYhony's AHRS algorithm.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
//=====================================================================================================

#ifndef MahonyAHRS_h
#define MahonyAHRS_h

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern volatile CALCU_TYPE twoKp;			// 2 * proportional gain (Kp)
extern volatile CALCU_TYPE twoKi;			// 2 * integral gain (Ki)

//---------------------------------------------------------------------------------------------------
// Function declarations

extern void MahonyAHRSupdate(CALCU_TYPE* dst_attitude_quat, const CALCU_TYPE* gyro, const CALCU_TYPE* norm_acc, const CALCU_TYPE* norm_mag);
extern void MahonyAHRSClearInteValue(void);

extern void MahonyAHRSModeFast(void);
extern void MahonyAHRSModeNormal(void);

#ifdef __cplusplus
};
#endif

#endif
