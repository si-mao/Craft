/*
 * menu_redir.h
 * menu_redir.c重定向
 * 创建者：死猫
 * 创建时间：2015.03.04
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.04
 */

#ifndef MENU_REDIR_H_
#define MENU_REDIR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#include "file.h"

__INLINE void menu_set_white(const uint8_t* pos, uint8_t width)
{
	file_set_white(FILE_SCREEN_WO, pos, width);
}

__INLINE void menu_clear_white(const uint8_t* pos, uint8_t width)
{
	file_clear_white(FILE_SCREEN_WO, pos, width);
}

//width = 0表示不清除后面
__INLINE uint8_t menu_put_str(const uint8_t* pos, int8_t width, const uint8_t* str)
{
	uint8_t ret;
	int8_t i;
	
	file_set_pos(FILE_SCREEN_WO, pos);
	ret = file_write(FILE_SCREEN_WO, str);
	for(i = 0; i <  width - ret; i++)
	{
		file_write_char(FILE_SCREEN_WO, ' ');
	}
	return ret;
}

__INLINE uint8_t menu_put_num(const uint8_t* pos, int8_t width, const void *data, uint8_t data_type)
{
	uint8_t ret;
	int8_t i;
	
	file_set_pos(FILE_SCREEN_WO, pos);
	ret = file_write_num(FILE_SCREEN_WO, data, data_type);
	for(i = 0; i <  width - ret; i++)
	{
		file_write_char(FILE_SCREEN_WO, ' ');
	}
	return ret;
}

#ifdef __cplusplus
};
#endif

#endif
