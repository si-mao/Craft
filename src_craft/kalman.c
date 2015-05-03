/*
 * kalman.c
 * 卡尔曼滤波
 * 创建者：死猫
 * 创建时间：2015.01.26
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.01.26
 */
 
 
//输入：
//QI~, 通过陀螺仪得到的姿态
//QS~, 通过加速度得到的姿态
//wb~, 当前陀螺仪的输入（或是期望角速度?）
//输出：
//Q^, 期望姿态

#include "matrix.h"
#include "quat.h"

QUAT_DECLARE(QI)
QUAT_DECLARE(QS)
QUAT_DECLARE(WB)

MATRIX_DECLARE(X, 6, 1)
MATRIX_DECLARE(F, 6, 6)
MATRIX_DECLARE(G, 6, 6)
MATRIX_DECLARE(H, 6, 6)
MATRIX_DECLARE(Z, 6, 1)
MATRIX_DECLARE(P, 6, 6)  //误差相关矩阵，度量估计值的精确程度。
MATRIX_DECLARE(Q, 6, 6)  //噪声w的协方差矩阵
MATRIX_DECLARE(R, 6, 6)  //噪声v的协方差矩阵

void kalman_init(void)
{
	uint8_t i;
	QUAT_SET_VALUE(QI, 1, 0, 0, 0)
	QUAT_SET_VALUE(QS, 1, 0, 0, 0)
	QUAT_SET_VALUE(WB, 0, 0, 0, 0)

	MATRIX_INIT(X, 6, 1)
	MATRIX_INIT(F, 6, 6)
	MATRIX_INIT(G, 6, 6)
	MATRIX_INIT(H, 6, 6)
	MATRIX_INIT(Z, 6, 1)
	MATRIX_INIT(P, 6, 6)
	MATRIX_INIT(Q, 6, 6)
	MATRIX_INIT(R, 6, 6)
	
	MATRIX_CLEAR(X)
	MATRIX_CLEAR(F)
	MATRIX_CLEAR(G)
	MATRIX_CLEAR(H)
	MATRIX_CLEAR(Z)
	MATRIX_CLEAR(P)
	MATRIX_CLEAR(Q)
	MATRIX_CLEAR(R)

	
	//附矩阵初值
	//F
	MATRIX_VALUE(F, 0, 3) = -0.5;
	MATRIX_VALUE(F, 1, 4) = -0.5;
	MATRIX_VALUE(F, 2, 5) = -0.5;
	//G
	MATRIX_VALUE(G, 0, 0) = -0.5;
	MATRIX_VALUE(G, 1, 1) = -0.5;
	MATRIX_VALUE(G, 2, 2) = -0.5;
	MATRIX_VALUE(G, 3, 3) = 1;
	MATRIX_VALUE(G, 4, 4) = 1;
	MATRIX_VALUE(G, 5, 5) = 1;
	//H
	MATRIX_VALUE(H, 0, 0) = -1;
	MATRIX_VALUE(H, 1, 1) = -1;
	MATRIX_VALUE(H, 2, 2) = -1;
	//P
	MATRIX_VALUE(P, 0, 0) = 1;
	MATRIX_VALUE(P, 1, 1) = 1;
	MATRIX_VALUE(P, 2, 2) = 1;
	MATRIX_VALUE(P, 3, 3) = 1;
	MATRIX_VALUE(P, 4, 4) = 1;
	MATRIX_VALUE(P, 5, 5) = 1;
	//Q 越大加速度计占的比例越大
	MATRIX_VALUE(Q, 0, 0) = 0.1;
	MATRIX_VALUE(Q, 1, 1) = 0.1;
	MATRIX_VALUE(Q, 2, 2) = 0.1;
	MATRIX_VALUE(Q, 3, 3) = 0.1;
	MATRIX_VALUE(Q, 4, 4) = 0.1;
	MATRIX_VALUE(Q, 5, 5) = 0.1;
	//R
	MATRIX_VALUE(R, 0, 0) = 0.99;
	MATRIX_VALUE(R, 1, 1) = 0.99;
	MATRIX_VALUE(R, 2, 2) = 0.99;
	MATRIX_VALUE(R, 3, 3) = 0.99;
	MATRIX_VALUE(R, 4, 4) = 0.99;
	MATRIX_VALUE(R, 5, 5) = 0.99;
}

//strap_down()姿态值 guass_newton()姿态值 陀螺仪值
void kalman_calcu(QUAT_TYPE atti, const QUAT_TYPE atti_gyro, const QUAT_TYPE atti_field, const CALCU_TYPE* wb)
{
	int8_t i;
	MATRIX_DECLARE(Xkk_1, 6, 1)
	MATRIX_DECLARE(Pkk_1, 6, 6)
	MATRIX_DECLARE(S,     6, 6)
	MATRIX_DECLARE(K,     6, 6)
	MATRIX_DECLARE(M_E,   6, 6)  //单位矩阵
	
	QUAT_DECLARE(Y_q)
	QUAT_DECLARE(QIE)
	
	MATRIX_INIT(Xkk_1, 6, 1)
	MATRIX_INIT(Pkk_1, 6, 6)
	MATRIX_INIT(S,     6, 6)
	MATRIX_INIT(K,     6, 6)
	MATRIX_INIT(M_E,   6, 6)

	//设置单位矩阵M_E
	MATRIX_CLEAR(M_E)
	for(i = 0; i < MATRIX_LINE(M_E); i++) MATRIX_VALUE(M_E, i, i) = 1;
	
	//赋值
	quat_copy(QI, atti_gyro);
	quat_copy(QS, atti_field);
	
	//赋值-[wbx]
	MATRIX_VALUE(F, 1, 2) = wb[0];
	MATRIX_VALUE(F, 2, 1) = -wb[0];
	MATRIX_VALUE(F, 0, 2) = -wb[1];
	MATRIX_VALUE(F, 2, 0) = wb[1];
	MATRIX_VALUE(F, 0, 1) = wb[2];
	MATRIX_VALUE(F, 1, 0) = -wb[2];
	
	//预测
	
	//Xkk_1 = F * X;    //预测的值
	matrix_set_op(F);
	matrix_op_mul(X);
	matrix_get_op(Xkk_1);
	
	//Pkk_1 = F * P * F.Transpose() + Q;    //预测的偏差, Pkk_1越大S越大
	matrix_set_op(F);
	matrix_op_transpose();
	matrix_op_mul(P);
	matrix_op_mul_transpose();
	matrix_op_add(Q);
	matrix_get_op(Pkk_1);

	//更新
	
	//Y_q = QS.Inverse() * QI;
	quat_copy(Y_q, QS);
	quat_inverse(Y_q);
	quat_mul(Y_q, QI);
	
	MATRIX_VALUE(Z, 0, 0) = Y_q[1];  //测量余量
	MATRIX_VALUE(Z, 1, 0) = Y_q[2];
	MATRIX_VALUE(Z, 2, 0) = Y_q[3];
	
	//S = H * Pkk_1 * H.Transpose() + R;  //测量余量协方差， S越小K越大
	matrix_set_op(H);
	matrix_op_transpose();
	matrix_op_mul(Pkk_1);
	matrix_op_mul_transpose();
	matrix_op_add(R);
	matrix_get_op(S);
	
	
	//K = Pkk_1 * H.Transpose() * S.Inverse();  //最优卡尔曼增益, 越大越相信传感器
	matrix_set_op(Pkk_1);
	matrix_op_mul_transpose();  //仍然保存着H.Transpose()
	matrix_inverse(S);
	matrix_op_mul_inverse();
	matrix_get_op(K);
	
	//更新X与P
	
	//X = Xkk_1 + K * Z;
	//matrix_set_op(K);  //上个运算结束，OP已经保存着K的值
	matrix_op_mul(Z);
	matrix_get_op(X);
	matrix_add(X, Xkk_1);

	//P = (MatrixE - K * H) * Pkk_1;
	matrix_set_op(K);
	matrix_op_mul(H);
	matrix_swap_op_usr0();
	matrix_set_op(M_E);
	matrix_op_dec_usr0();
	matrix_op_mul(Pkk_1);

	//计算姿态
	//QIE = Quat(1, X[0][0], X[1][0], X[2][0]).Unit();
	QUAT_SET_VALUE(QIE, 1, MATRIX_VALUE(X, 0, 0), MATRIX_VALUE(X, 1, 0), MATRIX_VALUE(X, 2, 0));
	quat_unit(QIE);
	
	//Attitude = (QI * QIE).Unit();
	quat_copy(atti, QI);
	quat_mul(atti, QIE);
	quat_unit(atti);
	
	//结束
}
