/*
 * imu_normalize.c
 * 归一化算法
 * 创建者：死猫
 * 创建时间：2015.02.01
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.26
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
