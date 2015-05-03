/*
 * imu_smooth.c
 * �⻬�㷨
 * �����ߣ���è
 * ����ʱ�䣺2015.01.15
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.22
 */
 
#include "system_define.h"

void imu_smooth_init(void)
{
}

void imu_smooth(CALCU_TYPE* dst, const CALCU_TYPE* src, CALCU_TYPE a)
{
	//a = Tf / (Tf + T), Tf�˲�����, T��������
	//const CALCU_TYPE a = 0.998f;
	//const CALCU_TYPE a = 0.0625;  //�ο�Multiwii
	
	//��ͨ�˲�
	dst[0] = (1 - a) * dst[0] + a * src[0];
	dst[1] = (1 - a) * dst[1] + a * src[1];
	dst[2] = (1 - a) * dst[2] + a * src[2];
}
