/*
 * kalman.h
 * 卡尔曼滤波
 * 创建者：死猫
 * 创建时间：2015.01.26
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.01.26
 */

#ifndef KALMAN_H_
#define KALMAN_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#include "quat.h"

extern void kalman_init(void);
extern void kalman_calcu(QUAT_TYPE atti, const QUAT_TYPE atti_gyro, const QUAT_TYPE atti_field, const CALCU_TYPE* wb);

#ifdef __cplusplus
};
#endif

#endif
