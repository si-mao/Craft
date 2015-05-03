/*
 * quat_unit.c
 * 四元数
 * 创建者：死猫
 * 创建时间：2015.01.18
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.04.02
 */

#include "my_math.h"

#include "quat_unit.h"

void quat_unit_mul(QUAT_TYPE q1, const QUAT_TYPE q2)
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

void quat_unit_to_eular3(const QUAT_TYPE q, QUAT_TYPE eular)
{
	/* Roll */  eular[0] = my_atan2(2 * (q[0] * q[1] + q[2] * q[3]), (1 - 2 * (q[1] * q[1] + q[2] * q[2])));
	/* Pitch */ eular[1] = my_asin(2 * (q[0] * q[2] - q[1] * q[3]));
	/* Yaw */   eular[2] = my_atan2(2 * (q[0] * q[3] + q[1] * q[2]), (1 - 2 * (q[2] * q[2] + q[3] * q[3])));
}

void quat_unit_from_eular3(QUAT_TYPE qout, const QUAT_TYPE eular)
{
	CALCU_TYPE eular1_sin = my_sin(eular[0] / 2);
	CALCU_TYPE eular2_sin = my_sin(eular[1] / 2);
	CALCU_TYPE eular3_sin = my_sin(eular[2] / 2);
	CALCU_TYPE eular1_cos = my_cos(eular[0] / 2);
	CALCU_TYPE eular2_cos = my_cos(eular[1] / 2);
	CALCU_TYPE eular3_cos = my_cos(eular[2] / 2);
	qout[0] = eular1_cos * eular2_cos * eular3_cos + eular1_sin * eular2_sin * eular3_sin;
	qout[1] = eular1_sin * eular2_cos * eular3_cos - eular1_cos * eular2_sin * eular3_sin;
	qout[2] = eular1_cos * eular2_sin * eular3_cos + eular1_sin * eular2_cos * eular3_sin;
	qout[3] = eular1_cos * eular2_cos * eular3_sin - eular1_sin * eular2_sin * eular3_cos;
}

void quat_to_unit(QUAT_TYPE q)
{
	CALCU_TYPE inv_sqrt;
	inv_sqrt = my_inv_sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	q[0] *= inv_sqrt;
	q[1] *= inv_sqrt;
	q[2] *= inv_sqrt;
	q[3] *= inv_sqrt;
}

void quat_unit_inverse(QUAT_TYPE q)
{
	q[0] = q[0];
	q[1] = -q[1];
	q[2] = -q[2];
	q[3] = -q[3];
}

void quat_unit_conjugation(QUAT_TYPE q)
{
	q[0] = q[0];
	q[1] = -q[1];
	q[2] = -q[2];
	q[3] = -q[3];
}
