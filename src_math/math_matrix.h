/*
 * math_matrix.h
 * 矩阵
 * 创建者：死猫
 * 创建时间：2015.01.25
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.02.24
 */

/*
 * 可以保存一个transpose和inverse
 * swap开销小，而mov开销大
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#define MATRIX_TYPE CALCU_TYPE*

#define MATRIX_DECLARE(x, line, column) CALCU_TYPE (x)[(line) * (column) + 1];
#define MATRIX_INIT(x, line, column) {((uint8_t *)x)[0] = line; ((uint8_t *)x)[1] = column; ((uint8_t *)x)[2] = line * column;}
#define MATRIX_LINE(x) (((uint8_t *)x)[0])
#define MATRIX_COLU(x) (((uint8_t *)x)[1])
#define MATRIX_LEN(x)  (((uint8_t *)x)[2])

#define MATRIX_VALUE(x, line, column) x[(column) * MATRIX_LINE(x) + (line) + 1] //按列存储，可以任意扩展列

#define MATRIX_CLEAR(x) {for(i = 1; i < MATRIX_LEN(x) + 1; i++){x[i] = 0;}}



#define M_OP          (m_OP)
#define M_RESULT_TEMP (m_result_temp)
#define M_LU          (m_lu)
#define M_LSOLVE      (m_linear_solve)
#define M_TRANS       (m_trans)
#define M_USR0        (m_usr0)

extern CALCU_TYPE *M_OP;
extern CALCU_TYPE *M_RESULT_TEMP;
extern CALCU_TYPE *M_LU;
extern CALCU_TYPE *M_LSOLVE;
extern CALCU_TYPE *M_TRANS;
extern CALCU_TYPE *M_USR0;

#define SWAP_OP_RESULT  {swap(&M_OP, &M_RESULT_TEMP);}
#define SWAP_OP_INVERSE {swap(&M_OP, &M_LSOLVE);}
#define SWAP_OP_TRANS   {swap(&M_OP, &M_TRANS);}
#define SWAP_OP_USR0    {swap(&M_OP, &M_USR0);}

extern void swap(CALCU_TYPE** p_m_1, CALCU_TYPE** p_m_2);
extern void matrix_copy(MATRIX_TYPE m1, const MATRIX_TYPE m2);

extern void matrix_add(MATRIX_TYPE m1, const MATRIX_TYPE m2);
extern void matrix_dec(MATRIX_TYPE m1, const MATRIX_TYPE m2);
extern void matrix_inverse(const MATRIX_TYPE m);
extern void matrix_transpose(const MATRIX_TYPE m);

extern void matrix_op_mul(const MATRIX_TYPE m);
extern void matrix_mul_num(MATRIX_TYPE m, const CALCU_TYPE num);

//Set|Get OP
__INLINE void matrix_set_op(const MATRIX_TYPE m){ matrix_copy(M_OP, m); }
__INLINE void matrix_get_op(MATRIX_TYPE m){ matrix_copy(m, M_OP); }

//OP
__INLINE void matrix_op_add(const MATRIX_TYPE m2){ matrix_add(M_OP, m2); }
__INLINE void matrix_op_dec(const MATRIX_TYPE m2){ matrix_dec(M_OP, m2); }
__INLINE void matrix_op_inverse(void){ matrix_inverse(M_OP); }
__INLINE void matrix_op_transpose(void){ matrix_transpose(M_OP); }

//Inverse
__INLINE void matrix_swap_op_inverse(void){ SWAP_OP_INVERSE }
__INLINE void matrix_mov_op_inverse(void){ matrix_copy(M_LSOLVE, M_OP); }
__INLINE void matrix_op_mul_inverse(void){ matrix_op_mul(M_LSOLVE); }

//Transpose
__INLINE void matrix_swap_op_transpose(void){ SWAP_OP_TRANS}
__INLINE void matrix_mov_op_transpose(void){ matrix_copy(M_TRANS, M_OP); }
__INLINE void matrix_op_mul_transpose(void){ matrix_op_mul(M_TRANS); }

__INLINE void matrix_swap_op_usr0(void){ SWAP_OP_USR0 }
__INLINE void matrix_mov_op_usr0(void){ matrix_copy(M_USR0, M_OP); }
__INLINE void matrix_op_mul_usr0(void){ matrix_op_mul(M_USR0); }
__INLINE void matrix_op_add_usr0(void){ matrix_add(M_OP, M_USR0); }
__INLINE void matrix_op_dec_usr0(void){ matrix_dec(M_OP, M_USR0); }


#ifdef __cplusplus
};
#endif

#endif
