/*
 * system_define.h
 * ϵͳ���弰�������Ͷ���
 * �����ߣ���è
 * ����ʱ�䣺2014.09.25
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.02.13
 */

#ifndef SYSTEM_DEFINE_H_
#define SYSTEM_DEFINE_H_

#define _LINUX    0
#define _WINDOWS  1
#define _STM32    2
#define _PLATFORM _STM32		//���ñ��뻷��

#if (_PLATFORM == _LINUX)
	#define THREAD_RETURN void *
//������������
	typedef unsigned char           byte;
	#include <stdint.h>
#elif  (_PLATFORM == _WINDOWS)
	#define THREAD_RETURN DWORD WINAPI
//������������
	#ifdef WIN32
		typedef unsigned char           byte;
		typedef signed char             int8_t;
		typedef signed short            int16_t;
		typedef signed int              int32_t;
		typedef unsigned char           uint8_t;
		typedef unsigned short          uint16_t;
		typedef unsigned int            uint32_t;

		#if __WORDSIZE == 64
			typedef long int                int64_t;
			typedef unsigned long int       uint64_t;
		#else
			typedef long long int           int64_t;
			typedef unsigned long long int  uint64_t;
		#endif
	#endif
#elif  (_PLATFORM == _STM32)
	#include "stm32f10x.h"
	
	typedef float            float32_t;
	typedef double           float64_t;
	
	#define CALCU_TYPE float
	
	#define NULL 0
#endif

//�û������ļ�ѡ��
#if defined USE_USER_CONFIG
	#include "user_config.h"
#endif

#endif
