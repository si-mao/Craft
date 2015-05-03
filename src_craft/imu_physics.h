/*
 * imu_physics.h
 * 物理量纲化
 * 创建者：死猫
 * 创建时间：2015.01.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.22
 */
 
#ifndef IMU_PHYSICS_H_
#define IMU_PHYSICS_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

//加速度
extern int16_t    Cali_Acc_B[3];
extern CALCU_TYPE Cali_Acc_K[3];

//陀螺仪
extern int16_t    Cali_Gyro_B[3];
extern CALCU_TYPE Cali_Gyro_K[3];

//磁场
extern int16_t    Cali_Magnet_B[3];
extern CALCU_TYPE Cali_Magnet_K[3];

extern void imu_physics_init(void);
//extern void imu_physics(CALCU_TYPE* dst_A_G_M, const int16_t* raw_A_G_M);
extern void imu_physics_gyro(CALCU_TYPE* dst_data, const int16_t* raw_data);
extern void imu_physics_acc(CALCU_TYPE* dst_data, const int16_t* raw_data);
extern void imu_physics_mag(CALCU_TYPE* dst_data, const int16_t* raw_data);

//磁场校准
void magn_cali_begin(void);
void magn_cali_input(const int16_t* raw_M);
void magn_cali_end(void);

#ifdef __cplusplus
};
#endif

#endif
