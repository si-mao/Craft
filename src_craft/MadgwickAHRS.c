/*
 * MadgwickAHRS.c
 * MadgwickAHRS 姿态算法
 * 创建者：死猫
 * 创建时间：2015.03.01
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.03.01
 */

//=====================================================================================================
// MadgwickAHRS.c
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
// 19/02/2012	SOH Madgwick	Magnetometer measurement is normalised
//
//=====================================================================================================

//---------------------------------------------------------------------------------------------------
// Header files

#include "MadgwickAHRS.h"
#include "MY_math.h"

//---------------------------------------------------------------------------------------------------
// Definitions

#define sampleFreq	1000.0f		// sample frequency in Hz
#define betaDef		0.1f		// 2 * proportional gain

#define AX    norm_data_A_G_M[0]
#define AY    norm_data_A_G_M[1]
#define AZ    norm_data_A_G_M[2]
#define GX    norm_data_A_G_M[3]
#define GY    norm_data_A_G_M[4]
#define GZ    norm_data_A_G_M[5]
#define MX    norm_data_A_G_M[6]
#define MY    norm_data_A_G_M[7]
#define MZ    norm_data_A_G_M[8]

#define Q0 dst_attitude_quat[0]
#define Q1 dst_attitude_quat[1]
#define Q2 dst_attitude_quat[2]
#define Q3 dst_attitude_quat[3]

//---------------------------------------------------------------------------------------------------
// Variable definitions

volatile float beta = betaDef;								// 2 * proportional gain (Kp)

void MadgwickAHRSModeFast(void)
{
	beta = 0.9f;
}

void MadgwickAHRSModeNormal(void)
{
	beta = betaDef;
}


void MadgwickAHRSupdate(CALCU_TYPE* dst_attitude_quat, const CALCU_TYPE* norm_data_A_G_M)
{
	CALCU_TYPE recipNorm;
	CALCU_TYPE s0, s1, s2, s3;
	CALCU_TYPE qDot1, qDot2, qDot3, qDot4;
	CALCU_TYPE hx, hy;
	CALCU_TYPE _2Q0MX, _2Q0MY, _2Q0MZ, _2Q1MX, _2bx, _2bz, _4bx, _4bz, _2Q0, _2Q1, _2Q2, _2Q3, _2Q0Q2, _2Q2Q3, Q0Q0, Q0Q1, Q0Q2, Q0Q3, Q1Q1, Q1Q2, Q1Q3, Q2Q2, Q2Q3, Q3Q3;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((MX == 0.0f) && (MY == 0.0f) && (MZ == 0.0f))
	{
		return;
	}

	// Rate of change of quaternion from GYroscope
	qDot1 = 0.5f * (-Q1 * GX - Q2 * GY - Q3 * GZ);
	qDot2 = 0.5f * (Q0 * GX + Q2 * GZ - Q3 * GY);
	qDot3 = 0.5f * (Q0 * GY - Q1 * GZ + Q3 * GX);
	qDot4 = 0.5f * (Q0 * GZ + Q1 * GY - Q2 * GX);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((AX == 0.0f) && (AY == 0.0f) && (AZ == 0.0f)))
	{

		// Auxiliary variables to avoid repeated arithmetic
		_2Q0MX = 2.0f * Q0 * MX;
		_2Q0MY = 2.0f * Q0 * MY;
		_2Q0MZ = 2.0f * Q0 * MZ;
		_2Q1MX = 2.0f * Q1 * MX;
		_2Q0 = 2.0f * Q0;
		_2Q1 = 2.0f * Q1;
		_2Q2 = 2.0f * Q2;
		_2Q3 = 2.0f * Q3;
		_2Q0Q2 = 2.0f * Q0 * Q2;
		_2Q2Q3 = 2.0f * Q2 * Q3;
		Q0Q0 = Q0 * Q0;
		Q0Q1 = Q0 * Q1;
		Q0Q2 = Q0 * Q2;
		Q0Q3 = Q0 * Q3;
		Q1Q1 = Q1 * Q1;
		Q1Q2 = Q1 * Q2;
		Q1Q3 = Q1 * Q3;
		Q2Q2 = Q2 * Q2;
		Q2Q3 = Q2 * Q3;
		Q3Q3 = Q3 * Q3;

		// Reference direction of Earth's magnetic field
		hx = MX * Q0Q0 - _2Q0MY * Q3 + _2Q0MZ * Q2 + MX * Q1Q1 + _2Q1 * MY * Q2 + _2Q1 * MZ * Q3 - MX * Q2Q2 - MX * Q3Q3;
		hy = _2Q0MX * Q3 + MY * Q0Q0 - _2Q0MZ * Q1 + _2Q1MX * Q2 - MY * Q1Q1 + MY * Q2Q2 + _2Q2 * MZ * Q3 - MY * Q3Q3;
		_2bx = my_sqrt(hx * hx + hy * hy);
		_2bz = -_2Q0MX * Q2 + _2Q0MY * Q1 + MZ * Q0Q0 + _2Q1MX * Q3 - MZ * Q1Q1 + _2Q2 * MY * Q3 - MZ * Q2Q2 + MZ * Q3Q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2Q2 * (2.0f * Q1Q3 - _2Q0Q2 - AX) + _2Q1 * (2.0f * Q0Q1 + _2Q2Q3 - AY) - _2bz * Q2 * (_2bx * (0.5f - Q2Q2 - Q3Q3) + _2bz * (Q1Q3 - Q0Q2) - MX) + (-_2bx * Q3 + _2bz * Q1) * (_2bx * (Q1Q2 - Q0Q3) + _2bz * (Q0Q1 + Q2Q3) - MY) + _2bx * Q2 * (_2bx * (Q0Q2 + Q1Q3) + _2bz * (0.5f - Q1Q1 - Q2Q2) - MZ);
		s1 = _2Q3 * (2.0f * Q1Q3 - _2Q0Q2 - AX) + _2Q0 * (2.0f * Q0Q1 + _2Q2Q3 - AY) - 4.0f * Q1 * (1 - 2.0f * Q1Q1 - 2.0f * Q2Q2 - AZ) + _2bz * Q3 * (_2bx * (0.5f - Q2Q2 - Q3Q3) + _2bz * (Q1Q3 - Q0Q2) - MX) + (_2bx * Q2 + _2bz * Q0) * (_2bx * (Q1Q2 - Q0Q3) + _2bz * (Q0Q1 + Q2Q3) - MY) + (_2bx * Q3 - _4bz * Q1) * (_2bx * (Q0Q2 + Q1Q3) + _2bz * (0.5f - Q1Q1 - Q2Q2) - MZ);
		s2 = -_2Q0 * (2.0f * Q1Q3 - _2Q0Q2 - AX) + _2Q3 * (2.0f * Q0Q1 + _2Q2Q3 - AY) - 4.0f * Q2 * (1 - 2.0f * Q1Q1 - 2.0f * Q2Q2 - AZ) + (-_4bx * Q2 - _2bz * Q0) * (_2bx * (0.5f - Q2Q2 - Q3Q3) + _2bz * (Q1Q3 - Q0Q2) - MX) + (_2bx * Q1 + _2bz * Q3) * (_2bx * (Q1Q2 - Q0Q3) + _2bz * (Q0Q1 + Q2Q3) - MY) + (_2bx * Q0 - _4bz * Q2) * (_2bx * (Q0Q2 + Q1Q3) + _2bz * (0.5f - Q1Q1 - Q2Q2) - MZ);
		s3 = _2Q1 * (2.0f * Q1Q3 - _2Q0Q2 - AX) + _2Q2 * (2.0f * Q0Q1 + _2Q2Q3 - AY) + (-_4bx * Q3 + _2bz * Q1) * (_2bx * (0.5f - Q2Q2 - Q3Q3) + _2bz * (Q1Q3 - Q0Q2) - MX) + (-_2bx * Q0 + _2bz * Q2) * (_2bx * (Q1Q2 - Q0Q3) + _2bz * (Q0Q1 + Q2Q3) - MY) + _2bx * Q1 * (_2bx * (Q0Q2 + Q1Q3) + _2bz * (0.5f - Q1Q1 - Q2Q2) - MZ);
		recipNorm = my_inv_sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	Q0 += qDot1 * (1.0f / sampleFreq);
	Q1 += qDot2 * (1.0f / sampleFreq);
	Q2 += qDot3 * (1.0f / sampleFreq);
	Q3 += qDot4 * (1.0f / sampleFreq);

	// Normalise quaternion
	recipNorm = my_inv_sqrt(Q0 * Q0 + Q1 * Q1 + Q2 * Q2 + Q3 * Q3);
	Q0 *= recipNorm;
	Q1 *= recipNorm;
	Q2 *= recipNorm;
	Q3 *= recipNorm;
}
