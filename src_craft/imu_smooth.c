/*
 * imu_smooth.c
 * 光滑算法
 * 创建者：死猫
 * 创建时间：2015.01.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.22
 */
 
#include "system_define.h"

void imu_smooth_init(void)
{
}

void imu_smooth(CALCU_TYPE* dst, const CALCU_TYPE* src, CALCU_TYPE a)
{
	//a = Tf / (Tf + T), Tf滤波常数, T采样周期
	//const CALCU_TYPE a = 0.998f;
	//const CALCU_TYPE a = 0.0625;  //参考Multiwii
	
	//低通滤波
	dst[0] = (1 - a) * dst[0] + a * src[0];
	dst[1] = (1 - a) * dst[1] + a * src[1];
	dst[2] = (1 - a) * dst[2] + a * src[2];
}
