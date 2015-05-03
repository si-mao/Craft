/*
 * math_quat.c
 * 四元数
 * 创建者：死猫
 * 创建时间：2015.01.18
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.02.24
 */

#include "my_math.h"

#include "math_quat.h"

void quat_copy(QUAT_TYPE q1, const QUAT_TYPE q2)
{
	q1[0] = q2[0];
	q1[1] = q2[1];
	q1[2] = q2[2];
	q1[3] = q2[3];
}

void quat_add(QUAT_TYPE q1, const QUAT_TYPE q2)
{
	q1[0] += q2[0];
	q1[1] += q2[1];
	q1[2] += q2[2];
	q1[3] += q2[3];
}

void quat_dec(QUAT_TYPE q1, const QUAT_TYPE q2)
{
	q1[0] -= q2[0];
	q1[1] -= q2[1];
	q1[2] -= q2[2];
	q1[3] -= q2[3];
}

void quat_mul(QUAT_TYPE q1, const QUAT_TYPE q2)
{
	CALCU_TYPE d[4];
	d[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
	d[1] = q1[1] * q2[0] + q1[0] * q2[1] - q1[3] * q2[2] + q1[2] * q2[3];
	d[2] = q1[2] * q2[0] + q1[3] * q2[1] + q1[0] * q2[2] - q1[1] * q2[3];
	d[3] = q1[3] * q2[0] - q1[2] * q2[1] + q1[1] * q2[2] + q1[0] * q2[3];
	
	q1[0] = d[0];
	q1[1] = d[1];
	q1[2] = d[2];
	q1[3] = d[3];
}

void quat_mul_num(QUAT_TYPE q, const CALCU_TYPE num)
{
	q[0] *= num;
	q[1] *= num;
	q[2] *= num;
	q[3] *= num;
}

void quat_to_eular(const QUAT_TYPE q, QUAT_TYPE eular)
{
	eular[0] = 0;
	/* Roll */  eular[1] = my_atan2(2 * (q[0] * q[1] + q[2] * q[3]), (1 - 2 * (q[1] * q[1] + q[2] * q[2])));
	/* Pitch */ eular[2] = my_asin(2 * (q[0] * q[2] - q[1] * q[3]));
	/* Yaw */   eular[3] = my_atan2(2 * (q[0] * q[3] + q[1] * q[2]), (1 - 2 * (q[2] * q[2] + q[3] * q[3])));
}

void quat_from_eular(QUAT_TYPE qout, const QUAT_TYPE eular)
{
	CALCU_TYPE eular1_sin = my_sin(eular[1] / 2);
	CALCU_TYPE eular2_sin = my_sin(eular[2] / 2);
	CALCU_TYPE eular3_sin = my_sin(eular[3] / 2);
	CALCU_TYPE eular1_cos = my_cos(eular[1] / 2);
	CALCU_TYPE eular2_cos = my_cos(eular[2] / 2);
	CALCU_TYPE eular3_cos = my_cos(eular[3] / 2);
	qout[0] = eular1_cos * eular2_cos * eular3_cos + eular1_sin * eular2_sin * eular3_sin;
	qout[1] = eular1_sin * eular2_cos * eular3_cos - eular1_cos * eular2_sin * eular3_sin;
	qout[2] = eular1_cos * eular2_sin * eular3_cos + eular1_sin * eular2_cos * eular3_sin;
	qout[3] = eular1_cos * eular2_cos * eular3_sin - eular1_sin * eular2_sin * eular3_cos;
}

void quat_unit(QUAT_TYPE q)
{
	CALCU_TYPE inv_sqrt;
	inv_sqrt = my_inv_sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	q[0] *= inv_sqrt;
	q[1] *= inv_sqrt;
	q[2] *= inv_sqrt;
	q[3] *= inv_sqrt;
}

void quat_inverse(QUAT_TYPE q)
{
	CALCU_TYPE normsquare;
	normsquare = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
	q[0] = q[0] / normsquare;
	q[1] = -q[1] / normsquare;
	q[2] = -q[2] / normsquare;
	q[3] = -q[3] / normsquare;
}

void quat_conjugation(QUAT_TYPE q)
{
	q[0] = q[0];
	q[1] = -q[1];
	q[2] = -q[2];
	q[3] = -q[3];
}
