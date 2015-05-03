/*
 * imu_attitude.c
 * 姿态解算算法
 * 创建者：死猫
 * 创建时间：2015.01.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.02
 */

#include "system_define.h"


#include "craft_data.h"
#include "imu_normalize.h"
#include "MahonyAHRS.h"
#include "MadgwickAHRS.h"

#include "quat_unit.h"
#include "my_math.h"

/*
CALCU_TYPE Filed_A_G_M[9];
CALCU_TYPE Norm_Filed_A_G_M[9];

void calcu_atti_init(void)
{
	uint8_t i;
	kalman_init();
	for(i = 0; i < 9; i++)
	{
		Filed_A_G_M[i] = 0;
		Norm_Filed_A_G_M[i] = 0;
	}
}
*/

void imu_attitude_init(void)
{
}

void rotateV(CALCU_TYPE *v, const CALCU_TYPE* delta)
{
	CALCU_TYPE v_tmp[3];
	v_tmp[0] = v[0];
	v_tmp[1] = v[1];
	v_tmp[2] = v[2];

  v[2] -= -delta[1] * v_tmp[0] + delta[0] * v_tmp[1];
  v[0] -=  delta[1] * v_tmp[2] - delta[2] * v_tmp[1];
  v[1] -= -delta[0] * v_tmp[2] + delta[2] * v_tmp[0];
}

CALCU_TYPE EstG[3];
void imu_attitude(void)
{
	uint8_t i;
	CALCU_TYPE acc_norm_square;
	CALCU_TYPE tmp;
	CALCU_TYPE deltaGyroAngle[3];
	static CALCU_TYPE t[2];

	const CALCU_TYPE dt = 0.001;
	const CALCU_TYPE k = 0.9983;
	for(i = 0; i < 3; i++)
	{
		deltaGyroAngle[i] = dt * craft_data.GYRO_physics[i];
	}
	rotateV(EstG, deltaGyroAngle);
	acc_norm_square = 0;
	for(i = 0; i < 3; i++)
	{
		acc_norm_square += craft_data.ACC_smooth[i] * craft_data.ACC_smooth[i];
	}
	if(acc_norm_square > (0.85f * 0.85f * ACC_1G_SQ) && acc_norm_square < (1.15f * 1.15f * ACC_1G_SQ))
	{
		for(i = 0; i < 3; i++)
		{
			EstG[i] = k * EstG[i] + (1 - k) * craft_data.ACC_smooth[i];
		}
	}

	tmp = EstG[0] * EstG[0] + EstG[2] * EstG[2];

	craft_data.Attitude_Eular[0] = my_atan2(EstG[1], my_inv_sqrt(tmp) * tmp);
	craft_data.Attitude_Eular[1] = my_atan2(-EstG[0], EstG[2]);
	
	//校准Trim
	if(craft_data.Calibrate_TRIM > 0)
	{
		if(craft_data.Calibrate_TRIM == CALIBRATE_LEVEL_TRIM)
		{
			t[0] = 0;
			t[1] = 0;
		}
		else
		{
			t[0] += craft_data.Attitude_Eular[0];
			t[1] += craft_data.Attitude_Eular[1];
			if(craft_data.Calibrate_TRIM == 1)
			{
				conf.AngleTrim[0] = (CALCU_TYPE)t[0] / (CALCU_TYPE)CALIBRATE_LEVEL_TRIM;
				conf.AngleTrim[1] = (CALCU_TYPE)t[1] / (CALCU_TYPE)CALIBRATE_LEVEL_TRIM;

				craft_data.Calibrate_TRIM = 0;  //校准完毕后Calibrate_X为-1
			}
		}
		craft_data.Calibrate_TRIM--;
	}
}

// #define ACCX data_A_G_M[0]
// #define ACCY data_A_G_M[1]
// #define ACCZ data_A_G_M[2]

// #define GYROX data_A_G_M[3]
// #define GYROY data_A_G_M[4]
// #define GYROZ data_A_G_M[5]

// #define MAGX data_A_G_M[6]
// #define MAGY data_A_G_M[7]
// #define MAGZ data_A_G_M[8]

// //CALCU_TYPE* dst_attitude_quat 必须指向当前姿态数据
// void imu_attitude_eular(CALCU_TYPE* dst_atti_eular, const CALCU_TYPE* data_A_G_M)
// {
// 	const CALCU_TYPE dt = 0.001;
// 	const CALCU_TYPE k = 0.998;
// 	
// 	dst_atti_eular[0] = 0;
// 	dst_atti_eular[1] = k * (dst_atti_eular[1] + dt * GYROX) + (1 - k) * ACCY / 9.8;  //x
// 	dst_atti_eular[2] = k * (dst_atti_eular[2] + dt * GYROY) - (1 - k) * ACCX / 9.8;  //y
// 	dst_atti_eular[3] = 0;  //z
// }

// #include "kalman.h"
// #include "matrix.h"

// //融合算法 角度
// void mix1(CALCU_TYPE* attitude, const CALCU_TYPE* data_A_G_M)
// {
// 	const CALCU_TYPE dt = 0.02;
// 	const CALCU_TYPE k = 0.95;
// 	QUAT_DECLARE(atti_eular)
// 	QUAT_DECLARE(atti_eular_new)
// 	quat_to_eular(attitude, atti_eular);
// 	

// 	atti_eular_new[0] = 0;
// 	atti_eular_new[1] = k * (atti_eular[1] + dt * GYROX) + (1 - k) * ACCY / 9.8;  //x
// 	atti_eular_new[2] = k * (atti_eular[2] + dt * GYROY) - (1 - k) * ACCX / 9.8;  //y
// 	atti_eular_new[3] = atti_eular[3];  //z

// 	quat_from_eular(attitude, atti_eular_new);
// }

// void mix2(CALCU_TYPE* dst_attitude_quat, const CALCU_TYPE* data_A_G_M)
// {
// }

// void mix_kalman(CALCU_TYPE* dst_attitude_quat, const CALCU_TYPE* norm_data_A_G_M)
// {
// 	QUAT_DECLARE(qf)
// 	QUAT_DECLARE(qw)

// 	//dqf = Guass_Newton();
// 	//qf = (Attitude + dqf).Unit();
// 	guass_newton(qf, dst_attitude_quat, norm_data_A_G_M + 0,  norm_data_A_G_M + 6, Norm_Filed_A_G_M);
// 	quat_add(qf, dst_attitude_quat);
// 	quat_unit(qf);
// 	
// 	//dqw = StrapDown();
// 	//qw = (Attitude + dqw).Unit();
// 	strap_down(qw, dst_attitude_quat, norm_data_A_G_M + 3);
// 	quat_add(qw, dst_attitude_quat);
// 	quat_unit(qw);

// 	//kalman.SetGyroAttitude(qw);
// 	//kalman.SetAccAttitude(qf);
// 	//kalman.SetWB(Quat(0, AccelMagnetGyro[6], AccelMagnetGyro[7], AccelMagnetGyro[8]));
// 	//kalman.Calculate();
// 	//Attitude = kalman.GetAttitude();
// 	kalman_calcu(dst_attitude_quat, qw, qf, norm_data_A_G_M + 3);

// 	//结束
// }


// //返回根据陀螺仪计算的姿态偏差dqw
// //Attitude = (Attitude + dqw).Unit();
// void strap_down(QUAT_TYPE dqw, const QUAT_TYPE attitude, const CALCU_TYPE* gyro_X_Y_Z)
// {
// 	QUAT_DECLARE(GyroMeasure)
// 	
// 	GyroMeasure[0] = 0;
// 	GyroMeasure[1] = gyro_X_Y_Z[0];
// 	GyroMeasure[2] = gyro_X_Y_Z[1];
// 	GyroMeasure[3] = gyro_X_Y_Z[2];

// 	//dqw = 0.02 * 0.5 * Attitude * GyroMeasure;
// 	quat_copy(dqw, attitude);
// 	quat_mul(dqw, GyroMeasure);
// 	quat_mul_num(dqw, 0.5 * 0.02);  //0.5 为系数，0.02为dT
// 	//计算结束
// }

// //返回根据场值计算的姿态偏差dqf
// //Attitude = (Attitude + dqf).Unit();
// void guass_newton(QUAT_TYPE dqf, const QUAT_TYPE attitude, const CALCU_TYPE* acc_X_Y_Z, const CALCU_TYPE* mag_X_Y_Z, const CALCU_TYPE* field_A_G_M)
// {
// 	int8_t i, j;
// 	MATRIX_DECLARE(MeasureMat, 6, 1)
// 	MATRIX_DECLARE(fq, 6, 1)
// 	MATRIX_DECLARE(eq, 6, 1)
// 	MATRIX_DECLARE(Jaco,  6, 4)
// 	MATRIX_DECLARE(dqm,  4, 1)
// 	
// 	QUAT_DECLARE(Q)
// 	QUAT_DECLARE(QConj)
// 	QUAT_DECLARE(dq)
// 	QUAT_DECLARE(acc0)
// 	QUAT_DECLARE(mag0)
// 	QUAT_DECLARE(acc0_b)
// 	QUAT_DECLARE(mag0_b)
// 	
// 	MATRIX_INIT(MeasureMat, 6, 1)
// 	MATRIX_INIT(fq, 6, 1)
// 	MATRIX_INIT(eq, 6, 1)
// 	MATRIX_INIT(Jaco,  6, 4)
// 	MATRIX_INIT(dqm,  6, 1)
// 	
// 	QUAT_SET_VALUE(acc0, 0, field_A_G_M[0], field_A_G_M[1], field_A_G_M[2])
// 	QUAT_SET_VALUE(mag0, 0, field_A_G_M[6], field_A_G_M[7], field_A_G_M[8])

// 	//赋值
// 	//Matrix MeasureMat(6, 1);
// 	//MeasureMat.CopyToColumn(AccelMagnetGyro, 0);
// 	for(i = 0; i < 3; i++)
// 	{
// 		MATRIX_VALUE(MeasureMat, i, 0) = field_A_G_M[i];
// 		MATRIX_VALUE(MeasureMat, i + 3, 0) = field_A_G_M[i + 6];
// 	}


// 	//body坐标系场值转化
// 	
// 	//Quat Q = Attitude;
// 	quat_copy(Q, attitude);

// 	/* 迭代 */ //SENSOR_CALCU_TYPE eps = 0.01;	//运算精度, 由于计算方便对精度平方进行判断
// 	/* 迭代 */ //SENSOR_CALCU_TYPE normsquare = 10;
// 	/* 迭代 */ //SENSOR_CALCU_TYPE epssquare = eps * eps;
// 	/* 迭代 */ //int ng_count = 3;
// 	/* 迭代 */ //while(normsquare > epssquare && ng_count > 0)
// 	for(i = 0; i < 1; i++)
// 	{
// 		/* 迭代 */ //ng_count--;
// 		//计算Jacobian矩阵
// 		
// 		//Jaco = Jacobian(Q, acc0, mag0);
// 		jacobian(Jaco, Q, acc0, mag0);

// 		//QConj = Q.Conjugation();
// 		quat_copy(QConj, Q);
// 		quat_conjugation(QConj);
// 		//acc0_b = QConj * acc0 * Q;
// 		quat_copy(acc0_b, QConj);
// 		quat_mul(acc0_b, acc0);
// 		quat_mul(acc0_b, Q);
// 		//mag0_b = QConj * mag0 * Q;
// 		quat_copy(mag0_b, QConj);
// 		quat_mul(mag0_b, mag0);
// 		quat_mul(mag0_b, Q);
// 		
// 		//fq[0][0] = Acc0_b[1]; fq[1][0] = Acc0_b[2]; fq[2][0] = Acc0_b[3];
// 		//fq[3][0] = Mag0_b[1]; fq[4][0] = Mag0_b[2]; fq[5][0] = Mag0_b[3];
// 		for(j = 0; j < 3; j++)
// 		{
// 			MATRIX_VALUE(fq, j, 0) = acc0_b[j + 1];
// 			MATRIX_VALUE(fq, j + 3, 0) = mag0_b[j + 1];
// 		}

// 		//eq = MeasureMat - fq;
// 		matrix_copy(eq, MeasureMat);
// 		matrix_dec(eq, fq);
// 		
// 		//Jtran = Jaco.Transpose();
// 		//dqm = (Jtran * Jaco).Inverse() * Jtran * eq;
// 		matrix_transpose(Jaco);   //Calulate Jtran
// 		matrix_swap_op_transpose(); //OP = Jtran
// 		matrix_mov_op_transpose();  //and save Jtran back to tran reg
// 		matrix_op_mul(Jaco);        //OP = Jtran * Jaco
// 		matrix_op_inverse();
// 		matrix_swap_op_inverse();   //OP = (Jtran * Jaco).Inverse()
// 		matrix_op_mul_transpose();  //OP = (Jtran * Jaco).Inverse() * Jtran
// 		matrix_op_mul(eq);          //OP = (Jtran * Jaco).Inverse() * Jtran * eq
// 		matrix_get_op(dqm);         //dqm = (Jtran * Jaco).Inverse() * Jtran * eq
// 		//dqf = Quat(-dqm[0][0], -dqm[1][0], -dqm[2][0], -dqm[3][0]);
// 		for(j = 0; j < 4; j++)
// 		{
// 			dq[j] = -MATRIX_VALUE(dqm, j, 0);
// 		}
// 		//Q = Q + dq_f;
// 		quat_add(Q, dq);
// 	}
// 	//return dqf = Q - Attitude;
// 	quat_copy(dqf, Q);
// 	quat_dec(dqf, attitude);
// }

// //Quat q: 变量
// //Quat acc0, Quat mag0: 影响f(q)的参数，为初试场值，坐标系为导航坐标系
// //方程:
// //y = f(q) + e, e = y - f(q)
// //accb = q^-1 * (0 acc^T)^T *q
// //magb = q^-1 * (0 mag^T)^T *q
// //y = [accb(1) accb(2) accb(3) magb(1) magb(2) magb(3)]
// void jacobian(MATRIX_TYPE jaco, const QUAT_TYPE q, const QUAT_TYPE acc0, const QUAT_TYPE mag0)
// {
// 	CALCU_TYPE aq[4][4];
// 	CALCU_TYPE mq[4][4];
// 	int8_t i, j;
// 	for(i = 1; i < 4; i++)
// 	{
// 		for(j = 0; j < 4; j++)
// 		{
// 			//由于-f(q)有负号，故乘以-1作为修正
// 			aq[i][j] = -2 * acc0[i] * q[j];
// 			mq[i][j] = -2 * mag0[i] * q[j];
// 		}
// 	}
// 	//MATRIX_INIT(jaco, 6, 4)
// 	MATRIX_VALUE(jaco, 0, 0) = aq[1][0] + aq[2][3] - aq[3][2]; MATRIX_VALUE(jaco, 0, 1) = aq[1][1] + aq[2][2] + aq[3][3]; MATRIX_VALUE(jaco, 0, 2) = aq[2][1] - aq[1][2] - aq[3][0]; MATRIX_VALUE(jaco, 0, 3) = aq[2][0] - aq[1][3] + aq[3][1];
// 	MATRIX_VALUE(jaco, 1, 0) = aq[2][0] - aq[1][3] + aq[3][1]; MATRIX_VALUE(jaco, 1, 1) = aq[1][2] - aq[2][1] + aq[3][0]; MATRIX_VALUE(jaco, 1, 2) = aq[1][1] + aq[2][2] + aq[3][3]; MATRIX_VALUE(jaco, 1, 3) = aq[3][2] - aq[2][3] - aq[1][0];
// 	MATRIX_VALUE(jaco, 2, 0) = aq[1][2] - aq[2][1] + aq[3][0]; MATRIX_VALUE(jaco, 2, 1) = aq[1][3] - aq[2][0] - aq[3][1]; MATRIX_VALUE(jaco, 2, 2) = aq[1][0] + aq[2][3] - aq[3][2]; MATRIX_VALUE(jaco, 2, 3) = aq[1][1] + aq[2][2] + aq[3][3];
// 	MATRIX_VALUE(jaco, 3, 0) = mq[1][0] + mq[2][3] - mq[3][2]; MATRIX_VALUE(jaco, 3, 1) = mq[1][1] + mq[2][2] + mq[3][3]; MATRIX_VALUE(jaco, 3, 2) = mq[2][1] - mq[1][2] - mq[3][0]; MATRIX_VALUE(jaco, 3, 3) = mq[2][0] - mq[1][3] + mq[3][1];
// 	MATRIX_VALUE(jaco, 4, 0) = mq[2][0] - mq[1][3] + mq[3][1]; MATRIX_VALUE(jaco, 4, 1) = mq[1][2] - mq[2][1] + mq[3][0]; MATRIX_VALUE(jaco, 4, 2) = mq[1][1] + mq[2][2] + mq[3][3]; MATRIX_VALUE(jaco, 4, 3) = mq[3][2] - mq[2][3] - mq[1][0];
// 	MATRIX_VALUE(jaco, 5, 0) = mq[1][2] - mq[2][1] + mq[3][0]; MATRIX_VALUE(jaco, 5, 1) = mq[1][3] - mq[2][0] - mq[3][1]; MATRIX_VALUE(jaco, 5, 2) = mq[1][0] + mq[2][3] - mq[3][2]; MATRIX_VALUE(jaco, 5, 3) = mq[1][1] + mq[2][2] + mq[3][3];
// 	//Jaco.CopyArray((const double **)J);
// }

// void set_field(const CALCU_TYPE* data_A_G_M)
// {
// 	uint8_t i;
// 	for(i = 0; i < 9; i++)
// 	{
// 		Filed_A_G_M[i] = data_A_G_M[i];
// 	}
// 	normalize(Norm_Filed_A_G_M, Filed_A_G_M);
// }
