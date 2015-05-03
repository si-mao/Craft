/*
 * error_callback.h
 * 错误处理回调
 * 创建者：死猫
 * 创建时间：2015.03.14
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.14
 */

#include "system_define.h"
#include "error.h"

#include "file.h"

void error_report_callback(uint32_t error_code)
{
	uint8_t pos[2];
	pos[0] = 0, pos[1] = 0;
	file_set_pos(FILE_SCREEN_WO, pos);
	file_write(FILE_SCREEN_WO, "错误");
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
