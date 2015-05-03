/*
 * math_matrix.c
 * 矩阵
 * 创建者：死猫
 * 创建时间：2015.01.25
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.02.01
 */

#include "math_matrix.h"

#define MATRIX_OP_LEN 36

//求乘法时操作数
CALCU_TYPE m_reg0[MATRIX_OP_LEN + 1];
CALCU_TYPE m_reg1[MATRIX_OP_LEN + 1];
CALCU_TYPE m_reg2[MATRIX_OP_LEN + 1];
CALCU_TYPE m_reg3[MATRIX_OP_LEN + 1];
CALCU_TYPE m_reg4[MATRIX_OP_LEN + 1];
CALCU_TYPE m_reg5[MATRIX_OP_LEN + 1];

CALCU_TYPE *m_OP           = m_reg0;  //作为操作数和结果存放
CALCU_TYPE *m_result_temp  = m_reg1;  //外部不可见，内部作为临时结果存放，计算完成后使用swap交换
//求逆时的中间变量
CALCU_TYPE *m_lu           = m_reg2;  //lu分解后结果存在此处
CALCU_TYPE *m_linear_solve = m_reg3;  //matrix_linear_solve()结果存放，每一列存放一个结果，也作为matrix_inverse结果存放
//求转置时的中间变量
CALCU_TYPE *m_trans        = m_reg4;  //转置后结果存在此处
//用户使用临时存储变量0
CALCU_TYPE *m_usr0         = m_reg5;

void swap(CALCU_TYPE** p_m_1, CALCU_TYPE** p_m_2)
{
	CALCU_TYPE* tem;
	tem = *p_m_1;
	*p_m_1 = *p_m_2;
	*p_m_2 = tem;
}

void matrix_copy(MATRIX_TYPE m1, const MATRIX_TYPE m2)
{
	uint8_t i;
	for(i = 0; i < MATRIX_LEN(m2) + 1; i++)
	{
		m1[i] = m2[i];
	}
}

void matrix_add(MATRIX_TYPE m1, const MATRIX_TYPE m2)
{
	uint8_t i;
	for(i = 1; i < MATRIX_LEN(m2) + 1; i++)  //若目标内存小于源内存，则溢出
	{
		m1[i] = m1[i] + m2[i];
	}
}

void matrix_dec(MATRIX_TYPE m1, const MATRIX_TYPE m2)
{
	uint8_t i;
	for(i = 1; i < MATRIX_LEN(m2) + 1; i++)  //若目标内存小于源内存，则溢出
	{
		m1[i] = m1[i] - m2[i];
	}
}

void matrix_mul_num(MATRIX_TYPE m, const CALCU_TYPE num)
{
	uint8_t i;
	for(i = 1; i < MATRIX_LEN(m) + 1; i++)
	{
		m[i] = m[i] * num;
	}
}

void matrix_op_mul(const MATRIX_TYPE m)
{
	uint8_t i, j, k;
	CALCU_TYPE tem;
	MATRIX_INIT(M_RESULT_TEMP, MATRIX_LINE(M_OP), MATRIX_COLU(m))
	for(i = 0; i < MATRIX_LINE(M_RESULT_TEMP); i++)
	{
		for(j = 0; j < MATRIX_COLU(M_RESULT_TEMP); j++)
		{
			tem = 0;
			for(k = 0; k < MATRIX_COLU(M_OP); k++)
			{
				tem += MATRIX_VALUE(M_OP, i, k) * MATRIX_VALUE(m, k, j);
			}
			MATRIX_VALUE(M_RESULT_TEMP, i, j) = tem;
		}
	}
	SWAP_OP_RESULT
}

void matrix_transpose(const MATRIX_TYPE m)
{
	uint8_t i, j;
	MATRIX_INIT(M_TRANS, MATRIX_COLU(m), MATRIX_LINE(m))
	for(i = 0; i < MATRIX_LINE(M_TRANS); i++)
	{
		for(j = 0; j < MATRIX_COLU(M_TRANS); j++)
		{
			MATRIX_VALUE(M_TRANS, i, j) = MATRIX_VALUE(m, j, i);
		}
	}
	//计算完毕
}

void matrix_lu(const MATRIX_TYPE m);
void matrix_linear_solve(const MATRIX_TYPE lu, const MATRIX_TYPE y, uint8_t column);

void matrix_inverse(const MATRIX_TYPE m)
{
	int8_t matrix_n;
	int8_t i;
	MATRIX_DECLARE(X, 6, 1)  //最大不超6行
	
	MATRIX_INIT(X, 6, 1)
	MATRIX_CLEAR(X)
	
	matrix_n = MATRIX_LINE(m);

	matrix_lu(m);  //LU分解

	for(i = 0; i < matrix_n; i++)
	{
		if(i != 0) MATRIX_VALUE(X, i - 1, 0) = 0;
		MATRIX_VALUE(X, i, 0) = 1;
		matrix_linear_solve(M_LU, X, i);
	}
	//计算完毕，结果存于m_linear_solve
}

void matrix_lu(const MATRIX_TYPE m)
{
	int8_t matrix_n;
	int8_t i, j, k, r;
	CALCU_TYPE tem;
	
	matrix_n = MATRIX_LINE(m);
	
	MATRIX_INIT(M_LU, matrix_n, matrix_n)

	//L矩阵附对角线元素1
	for(i = 0; i < matrix_n; i++) MATRIX_VALUE(M_LU, i, i) = 1;
	//L, U矩阵计算
	//U矩阵第一行，L矩阵第一列
	for(j = 0; j < matrix_n; j++) MATRIX_VALUE(M_LU, 0, j) = MATRIX_VALUE(m, 0, j);
	
	tem = MATRIX_VALUE(m, 0, 0);
	for(i = 1; i < matrix_n; i++) MATRIX_VALUE(M_LU, i, 0) = MATRIX_VALUE(m, i, 0) / tem;

	//LU其他行列计算
	for(r = 1; r < matrix_n; r++)
	{
		//U矩阵按r行计算
		for(j = r; j < matrix_n; j++)
		{
			tem = 0;
			for(k = 0; k < r; k++) tem += MATRIX_VALUE(M_LU, r, k) * MATRIX_VALUE(M_LU, k, j);
			MATRIX_VALUE(M_LU, r, j) = MATRIX_VALUE(m, r, j) - tem;
		}
		//L矩阵按r列计算
		for(i = r + 1; i < matrix_n; i++)
		{
			tem = 0;
			for(k = 0; k < r; k++) tem += MATRIX_VALUE(M_LU, i, k) * MATRIX_VALUE(M_LU, k, r);
			MATRIX_VALUE(M_LU, i, r) = (MATRIX_VALUE(m, i, r) - tem) / MATRIX_VALUE(M_LU, r, r);
		}
	}
}

void matrix_linear_solve(const MATRIX_TYPE lu, const MATRIX_TYPE y, uint8_t column)
{
	uint8_t matrix_n;
	int8_t i, k;
	CALCU_TYPE tem;

	matrix_n = MATRIX_LINE(lu);
	//求解算法
	//y = Ax
	//替换：A = LU，有y = LUx
	//令z = Ux，有y = Lz, z = Ux

	//求y = Lz的z
	MATRIX_INIT(M_RESULT_TEMP, matrix_n, 1)
	MATRIX_VALUE(M_RESULT_TEMP, 0, 0) = MATRIX_VALUE(y, 0, 0);
	for(i = 1; i < matrix_n; i++)
	{
		tem = 0;
		for(k = 0; k <= i - 1; k++) tem += MATRIX_VALUE(lu, i, k) * MATRIX_VALUE(M_RESULT_TEMP, k, 0);
		MATRIX_VALUE(M_RESULT_TEMP, i, 0) = MATRIX_VALUE(y, i, 0) - tem;
	}
	//求z = Ux的x
	MATRIX_INIT(M_LSOLVE, matrix_n, column + 1);  //由于列数可任意扩展，故可以直接这么初始化
	MATRIX_VALUE(M_LSOLVE, matrix_n - 1, column) = MATRIX_VALUE(M_RESULT_TEMP, matrix_n - 1, 0) / MATRIX_VALUE(lu, matrix_n - 1, matrix_n - 1);
	for(i = matrix_n - 2; i >= 0 ; i--)
	{
		tem = 0;
		for(k = i + 1; k < matrix_n; k++) tem += MATRIX_VALUE(lu, i, k) * MATRIX_VALUE(M_LSOLVE, k, column);
		MATRIX_VALUE(M_LSOLVE, i, column) = (MATRIX_VALUE(M_RESULT_TEMP, i, 0) - tem) / MATRIX_VALUE(lu, i, i);
	}
	//运算完毕
}
