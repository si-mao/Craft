/*
 * error_callback.h
 * ������ص�
 * �����ߣ���è
 * ����ʱ�䣺2015.03.14
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.14
 */

#include "system_define.h"
#include "error.h"

#include "file.h"

void error_report_callback(uint32_t error_code)
{
	uint8_t pos[2];
	pos[0] = 0, pos[1] = 0;
	file_set_pos(FILE_SCREEN_WO, pos);
	file_write(FILE_SCREEN_WO, "����");
	pos[0] = 1, pos[1] = 0;
	file_set_pos(FILE_SCREEN_WO, pos);
	file_write_hex(FILE_SCREEN_WO, error_code, 8);
	switch(error_code & 0x0000FFFF)
	{
		case ERROR_CODE_LOST_SIGNAL:
		{
		}
		break;
		default:
		break;
	}
}
