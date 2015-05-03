/*
 * imu_normalize.c
 * 归一化算法
 * 创建者：死猫
 * 创建时间：2015.02.01
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.26
 */

#include "system_define.h"

#include "my_math.h"

//返回向量模长的平方
CALCU_TYPE imu_normalize(CALCU_TYPE* dst, const CALCU_TYPE* src)
{
	CALCU_TYPE invsqrt;
	CALCU_TYPE norm_square;

	norm_square = src[0] * src[0] + src[1] * src[1] + src[2] * src[2];
	invsqrt = my_inv_sqrt(norm_square);
	dst[0] = src[0] * invsqrt;
	dst[1] = src[1] * invsqrt;
	dst[2] = src[2] * invsqrt;
	
	return norm_square;
}
