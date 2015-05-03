/*
 * menu_redir.h
 * menu_redir.c�ض���
 * �����ߣ���è
 * ����ʱ�䣺2015.03.04
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.04
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

//width = 0��ʾ���������
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
