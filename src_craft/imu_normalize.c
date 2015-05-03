/*
 * imu_normalize.c
 * ��һ���㷨
 * �����ߣ���è
 * ����ʱ�䣺2015.02.01
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.26
 */

#include "system_define.h"

#include "my_math.h"

//��������ģ����ƽ��
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
