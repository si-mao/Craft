

/*
 * MahonyAHRS.c
 * Mahony 姿态算法
 * 创建者：SOH Madgwick
 * 创建时间：2011.09.29
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     SOH Madgwick, 死猫,
 * 最终修改时间：2015.03.26
 */

//=====================================================================================================
//
// Madgwick's implementation of MAYhony's AHRS algorithm.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
//=====================================================================================================

#include "system_define.h"

#include "MahonyAHRS.h"

#include "my_math.h"

//#define USE_WHOLE_POSTURE_MAG

#define sampleFreq	1000.0f			// sample frequency in Hz
#define twoKpDef	(2.0f * 0.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0f)	// 2 * integral gain

#define GX    gyro[0]
#define GY    gyro[1]
#define GZ    gyro[2]

#define AX    norm_acc[0]
#define AY    norm_acc[1]
#define AZ    norm_acc[2]

#define MX    norm_mag[0]
#define MY    norm_mag[1]
#define MZ    norm_mag[2]

#define Q0 dst_attitude_quat[0]
#define Q1 dst_attitude_quat[1]
#define Q2 dst_attitude_quat[2]
#define Q3 dst_attitude_quat[3]


volatile CALCU_TYPE twoKp = twoKpDef;											// 2 * proportional gain (Kp)
volatile CALCU_TYPE twoKi = twoKiDef;											// 2 * integral gain (Ki)
volatile CALCU_TYPE integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// integral error terms scaled by Ki

void MahonyAHRSModeFast(void)
{
	twoKp = (2.0f * 800.0f);
	twoKi = 0.0f;
}

void MahonyAHRSModeNormal(void)
{
	twoKp = twoKpDef;
	twoKi = twoKiDef;
}

void MahonyAHRSClearInteValue(void)
{
	integralFBx = 0.0f;
	integralFBy = 0.0f;
	integralFBz = 0.0f;
}

//若(norm_acc == NULL)或(norm_mag == NULL)表明不使用该数据
//param0: 姿态
//param1: gyro数据(rad/s)
//param2: acc归一化数据
//param3: mag归一化数据
void MahonyAHRSupdate(CALCU_TYPE* dst_attitude_quat, const CALCU_TYPE* gyro, const CALCU_TYPE* norm_acc, const CALCU_TYPE* norm_mag)
{
	CALCU_TYPE recipNorm;
	CALCU_TYPE Q0Q0, Q0Q1, Q0Q2, Q1Q3, Q2Q3, Q3Q3;

#ifdef USE_WHOLE_POSTURE_MAG
	CALCU_TYPE Q0Q3, Q1Q1, Q1Q2, Q2Q2;
	CALCU_TYPE hx, hy, by, bz;
	CALCU_TYPE halfwx, halfwy;
#endif
	
	CALCU_TYPE halfvx, halfvy, halfvz;
	CALCU_TYPE halfex, halfey, halfez;
	CALCU_TYPE gcx, gcy, gcz;
	CALCU_TYPE qDot0, qDot1, qDot2, qDot3;

	// Auxiliary variables to avoid repeated arithmetic
	if(norm_acc != NULL)
	{
		Q0Q0 = Q0 * Q0;
		Q0Q1 = Q0 * Q1;
		Q0Q2 = Q0 * Q2;
		Q1Q3 = Q1 * Q3;
		Q2Q3 = Q2 * Q3;
		Q3Q3 = Q3 * Q3;
		
		halfvx = Q1Q3 - Q0Q2;
		halfvy = Q0Q1 + Q2Q3;
		halfvz = Q0Q0 - 0.5f + Q3Q3;
		
		halfex = (AY * halfvz - AZ * halfvy);
		halfey = (AZ * halfvx - AX * halfvz);
		halfez = (AX * halfvy - AY * halfvx);
	}
	else
	{
		halfex = 0;
		halfey = 0;
		halfez = 0;
	}

#ifdef USE_WHOLE_POSTURE_MAG
	if(norm_mag != NULL)
	{
		Q0Q2 = Q0 * Q2;
		Q0Q3 = Q0 * Q3;
		Q1Q1 = Q1 * Q1;
		Q1Q2 = Q1 * Q2;
		Q2Q2 = Q2 * Q2;

		hx = 2.0f * (MX * (0.5f - Q2Q2 - Q3Q3) + MY * (Q1Q2 - Q0Q3) + MZ * (Q1Q3 + Q0Q2));
		hy = 2.0f * (MX * (Q1Q2 + Q0Q3) + MY * (0.5f - Q1Q1 - Q3Q3) + MZ * (Q2Q3 - Q0Q1));
		
		by = my_sqrt(hx * hx + hy * hy);
		bz = 2.0f * (MX * (Q1Q3 - Q0Q2) + MY * (Q2Q3 + Q0Q1) + MZ * (0.5f - Q1Q1 - Q2Q2));

		halfwx = by * (Q0Q3 + Q1Q2) + bz * (Q1Q3 - Q0Q2);
		halfwy = by * (0.5f - Q1Q1 - Q3Q3) + bz * (Q0Q1 + Q2Q3);
		//halfwz = by * (Q2Q3 - Q0Q1) + bz * (0.5f - Q1Q1 - Q2Q2);
		
		//halfex += (MY * halfwz - MZ * halfwy);
		//halfey += (MZ * halfwx - MX * halfwz);
		halfez += (MX * halfwy - MY * halfwx);
	}
//#else
	//my_atan2(MZ * AX - MX * AZ, AZ * MY - AY * MZ);
#endif
	if(twoKi != 0)
	{
		integralFBx += twoKi * halfex * (1.0f / sampleFreq);
		integralFBy += twoKi * halfey * (1.0f / sampleFreq);
		integralFBz += twoKi * halfez * (1.0f / sampleFreq);
	}
	else
	{
		integralFBx = 0;
		integralFBy = 0;
		integralFBz = 0;
	}

	gcx = GX + integralFBx + twoKp * halfex;
	gcy = GY + integralFBy + twoKp * halfey;
	gcz = GZ + integralFBz + twoKp * halfez;

	gcx *= (0.5f * (1.0f / sampleFreq));
	gcy *= (0.5f * (1.0f / sampleFreq));
	gcz *= (0.5f * (1.0f / sampleFreq));

	// Integrate rate of change of quaternion
	qDot0 = (-Q1 * gcx - Q2 * gcy - Q3 * gcz);
	qDot1 = (Q0 * gcx + Q2 * gcz - Q3 * gcy);
	qDot2 = (Q0 * gcy - Q1 * gcz + Q3 * gcx);
	qDot3 = (Q0 * gcz + Q1 * gcy - Q2 * gcx);

	Q0 += qDot0;
	Q1 += qDot1;
	Q2 += qDot2;
	Q3 += qDot3;

	// Normalise quaternion
	recipNorm = my_inv_sqrt(Q0 * Q0 + Q1 * Q1 + Q2 * Q2 + Q3 * Q3);
	Q0 *= recipNorm;
	Q1 *= recipNorm;
	Q2 *= recipNorm;
	Q3 *= recipNorm;
	

}
