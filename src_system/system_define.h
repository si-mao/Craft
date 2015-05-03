/*
 * system_define.h
 * 系统定义及数据类型定义
 * 创建者：死猫
 * 创建时间：2014.09.25
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.02.13
 */

#ifndef SYSTEM_DEFINE_H_
#define SYSTEM_DEFINE_H_

#define _LINUX    0
#define _WINDOWS  1
#define _STM32    2
#define _PLATFORM _STM32		//设置编译环境

#if (_PLATFORM == _LINUX)
	#define THREAD_RETURN void *
//定义数据类型
	typedef unsigned char           byte;
	#include <stdint.h>
#elif  (_PLATFORM == _WINDOWS)
	#define THREAD_RETURN DWORD WINAPI
//定义数据类型
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

//用户配置文件选择
#if defined USE_USER_CONFIG
	#include "user_config.h"
#endif

#endif
