/*
 * my_math.c
 * my math
 * �����ߣ���è
 * ����ʱ�䣺2015.02.10
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.29
 */
 
#ifndef MY_MATH_H_
#define MY_MATH_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

//#define USE_MATH_STDLIB  //ʹ�ñ�׼��

#ifdef USE_MATH_STDLIB

	#include <math.h>

	#define my_sin sin
	#define my_cos cos

	#define my_asin asin
	#define my_acos acos
	#define my_atan2 atan2

	#define my_inv_sqrt(x) (1.0f/sqrt(x))
	#define my_sqrt(x) (sqrt(x))
#endif

#ifndef USE_MATH_STDLIB

	#define M_PI    3.1415926535898f
	#define M_2PI   6.2831853071796f
	#define M_PI_2  1.5707963267949f
	#define M_PI_4  0.7853981633974f
	#define M_2_PI  0.6366197723676f
	#define M_4_PI  1.2732395447352f
		
	#define MY_ABS(x) (((x) < 0) ? (-(x)) : (x))

	#define RAD2DEG(r) ((r) * (180.0f / PI))
	#define DEG2RAD(a) ((a) * (PI / 180.0f))

	CALCU_TYPE my_sin(CALCU_TYPE x);
	CALCU_TYPE my_cos(CALCU_TYPE x);

	CALCU_TYPE my_asin(CALCU_TYPE x);
	CALCU_TYPE my_acos(CALCU_TYPE x);
	CALCU_TYPE my_atan2(CALCU_TYPE x_sin, CALCU_TYPE x_cos);

	float32_t my_inv_sqrt(float32_t x);
	CALCU_TYPE my_sqrt(CALCU_TYPE x);

#endif

#define MY_CONSTRAIN_UP(x, up)     {(x) = ((x) > (up)) ? (up) : (x);}
#define MY_CONSTRAIN_DOWN(x, down) {(x) = ((x) < (down)) ? (down) : (x);}
#define MY_CONSTRAIN(x, down, up)  {MY_CONSTRAIN_UP((x), (up)); MY_CONSTRAIN_DOWN((x), (down));}

#ifdef __cplusplus
};
#endif

#endif
