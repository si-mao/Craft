/*
 * file.c
 * 文件
 * 创建者：死猫
 * 创建时间：2015.03.06
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.06
 */

#include "system_define.h"
#include "queue.h"

#include "file.h"

#define FILE_UART_WO_BUFF_SIZE  512
#define FILE_SCREEN_WO_BUFF_SIZE  512
#define FILE_RF_WO_BUFF_SIZE 128

#ifdef FILE_UART_USE
	#include "stm32_usart.h"
	uint8_t file_uart_wo_buff[FILE_UART_WO_BUFF_SIZE];
	Queue_Type file_stream_uart_wo;
#endif

#ifdef FILE_SCREEN_USE
	#ifdef FILE_SCREEN_LCD12864_USE
		#include "lcd12864.h"
		uint8_t file_screen_wo_buff[FILE_SCREEN_WO_BUFF_SIZE];
		Queue_Type file_stream_screen_wo;
	#endif
#endif

Queue_Type* file_node[] =
{
#ifdef FILE_UART_USE
	&file_stream_uart_wo,
#endif
#ifdef FILE_SCREEN_USE
	#ifdef FILE_SCREEN_LCD12864_USE
	&file_stream_screen_wo,
	#endif
#endif
	NULL
};


void file_init(void)
{
#ifdef FILE_UART_USE
	queue_init(&file_stream_uart_wo, file_uart_wo_buff, FILE_UART_WO_BUFF_SIZE);
#endif

#ifdef FILE_SCREEN_USE
	#ifdef FILE_SCREEN_LCD12864_USE
		queue_init(&file_stream_screen_wo, file_screen_wo_buff, FILE_SCREEN_WO_BUFF_SIZE);
	#endif
#endif
}

void file_loop(void)
{
	static uint32_t errcode;
	static uint8_t c;
	static uint8_t ctrl_char[8];
	static uint8_t ctrl_char_cnt = 0;

#ifdef FILE_UART_USE
	c = queue_delete(&file_stream_uart_wo, &errcode);
	if(0 == errcode)
	{
		usart_put_char(c);
	}
#endif

#ifdef FILE_SCREEN_USE
#ifdef FILE_SCREEN_LCD12864_USE
	c = queue_delete(&file_stream_screen_wo, &errcode);
	if(0 == errcode)
	{
		if(c == '\033')
		{
			ctrl_char_cnt = 1;
			ctrl_char[0] = c;
		}else{
			if(ctrl_char_cnt == 0)
			{
				lcd12864_put_char(c);
			}
			else if(ctrl_char_cnt == 1)
			{
				ctrl_char[1] = c;
				ctrl_char_cnt = 2;
			}
			else
			{
				switch(ctrl_char[1])
				{
					case 1:
					{
						if(ctrl_char_cnt == 2)
						{
							ctrl_char[2] = c;
							ctrl_char_cnt = 3;
						}
						else if(ctrl_char_cnt == 3)
						{
							ctrl_char[3] = c;
							lcd12864_set_cursor(ctrl_char[3], ctrl_char[2]);
							ctrl_char_cnt = 0;
						}
						else
						{
							lcd12864_put_char(c);
							ctrl_char_cnt = 0;
						}
					}
					break;
					case 2:
					{
						if(ctrl_char_cnt == 2)
						{
							ctrl_char[2] = c;
							ctrl_char_cnt = 3;
						}
						else if(ctrl_char_cnt == 3)
						{
							ctrl_char[3] = c;
							ctrl_char_cnt = 4;
						}
						else if(ctrl_char_cnt == 4)
						{
							ctrl_char[4] = c;
							lcd12864_set_white(ctrl_char + 2, ctrl_char[4]);
							ctrl_char_cnt = 0;
						}
						else
						{
							lcd12864_put_char(c);
							ctrl_char_cnt = 0;
						}
					}
					break;
					case 3:
					{
						if(ctrl_char_cnt == 2)
						{
							ctrl_char[2] = c;
							ctrl_char_cnt = 3;
						}
						else if(ctrl_char_cnt == 3)
						{
							ctrl_char[3] = c;
							ctrl_char_cnt = 4;
						}
						else if(ctrl_char_cnt == 4)
						{
							ctrl_char[4] = c;
							lcd12864_clear_white(ctrl_char + 2, ctrl_char[4]);
							ctrl_char_cnt = 0;
						}
						else
						{
							lcd12864_put_char(c);
							ctrl_char_cnt = 0;
						}
					}
					break;
					default:
					{
						lcd12864_put_char(c);
						ctrl_char_cnt = 0;
					}
					break;
				}
			}
		}
	}
#endif
#endif
}

void file_clear(uint8_t file, const uint8_t* pos, int8_t len)
{
#ifdef FILE_SCREEN_USE
#ifdef FILE_SCREEN_LCD12864_USE
	if(FILE_SCREEN_WO == file)
	{
		file_set_pos(file, pos);
		if(len > 16 | len < 0)
			len = 16 - pos[1];
		while(len > 0)
		{
			len--;
			file_write_char(file, ' ');
		}
	}
#endif
#endif
}

//pos[0]: 行数, pos[1]列数
//LCD12864: [0~3], [0~15(even)]
//  行数必须为偶数
void file_set_pos(uint8_t file, const uint8_t* pos)
{
#ifdef FILE_SCREEN_USE
#ifdef FILE_SCREEN_LCD12864_USE
	if(FILE_SCREEN_WO == file)
	{
		file_write_char(file, '\033');
		file_write_char(file, 1);
		file_write_char(file, pos[0]);
		file_write_char(file, pos[1] / 2);
	}
#endif
#endif
}

//设置反白，同时清除其余反白
void file_set_white(uint8_t file, const uint8_t* pos, uint8_t width)
{
#ifdef FILE_SCREEN_USE
#ifdef FILE_SCREEN_LCD12864_USE
	if(FILE_SCREEN_WO == file)
	{
		file_write_char(file, '\033');
		file_write_char(file, 2);
		file_write_char(file, pos[0]);
		file_write_char(file, pos[1]);
		file_write_char(file, width);
	}
#endif
#endif
}

//清除反白
void file_clear_white(uint8_t file, const uint8_t* pos, uint8_t width)
{
#ifdef FILE_SCREEN_USE
#ifdef FILE_SCREEN_LCD12864_USE
	if(FILE_SCREEN_WO == file)
	{
		file_write_char(file, '\033');
		file_write_char(file, 3);
		file_write_char(file, pos[0]);
		file_write_char(file, pos[1]);
		file_write_char(file, width);
	}
#endif
#endif
}

uint8_t file_write_array(uint8_t file, uint8_t* data ,uint8_t length)
{
	uint8_t i;
	for (i = 0; i < length; i++)
	{
		file_write_char(file, data[i]);
	}
	return length;
}

// 逆序输出一个数组
uint8_t file_write_array_reverse(uint8_t file, uint8_t* data ,uint8_t length)
{
	int8_t i;
	for (i = length - 1; i >= 0; i--)
	{
		file_write_char(file, data[i]);
	}
	return length;
}

uint8_t file_write(uint8_t file, const uint8_t* str)
{
	uint8_t i;
	i = 0;
	while(str[i] != '\0')
	{
		file_write_char(file, str[i]);
		i++;
	}
	return i;
}

uint8_t file_write_num(uint8_t file, const void *data, uint8_t data_type)
{
	switch(data_type)
	{
		case TYPE_NULL:    return 0;
		case TYPE_UINT8:   return file_write_uint(file, *((uint8_t  *)data));
		case TYPE_INT8:    return file_write_int(file,  *((int8_t   *)data));
		case TYPE_UINT16:  return file_write_uint(file, *((uint16_t *)data));
		case TYPE_INT16:   return file_write_int(file,  *((int16_t  *)data));
		case TYPE_UINT32:  return file_write_uint(file, *((uint32_t *)data));
		case TYPE_INT32:   return file_write_int(file,  *((int32_t  *)data));
		case TYPE_FLOAT32: return file_write_float32(file, *((float32_t *)data), 2);
		case TYPE_FLOAT64: return file_write_float32(file, *((float32_t *)data), 2);
// 		case TYPE_BIN:     return file_write_bin(file, *((uint8_t *)data));
// 		case TYPE_HEX:     return file_write_hex(file, *((uint32_t *)data), 8);
		default: break;
	}
	return 0;
}

uint8_t file_write_uint(uint8_t file, uint32_t data)
{
	uint8_t temp[10];  //最大4294967295
	uint8_t i;
	uint8_t ret;
	i = 0;
	do
	{
		temp[i++] = data % 10 + 0x30;
		data /= 10;
	}while(data);

	ret = file_write_array_reverse(file, temp, i);
	return ret;
}

uint8_t file_write_int(uint8_t file, int32_t data)
{
	uint8_t ret;
	ret = 0;
	if(data < 0)
	{
		file_write_char(file, '-');
		data = -data;
		ret++;
	}
	ret += file_write_uint(file, data);
	return ret;
}

//输出十六进制数
//param 1 : 文件
//param 2 : 显示数据
//param 3 : 十六进制字符数
uint8_t file_write_hex(uint8_t file, uint32_t data, uint8_t hex_len)
{
	uint8_t ch;
	uint8_t tmp;
	uint8_t ret;

	file_write_char(file, '0');
	file_write_char(file, 'x');            //0x

	ret = 2;
	while(hex_len > 0)
	{
		tmp = hex_len * 4 - 4;
		ch = (data & (0x0000000F << tmp)) >> tmp;
		if(ch > 9) ch += 0x37;          /*0-9 BCD X+0X30 */
		else ch += 0x30 ;               /*A-F BCD X+0X37 */
		file_write_char(file, ch);
		ret++;
		hex_len--;
	}
	return ret;
}

// 输出二进制数
uint8_t file_write_bin(uint8_t file, uint8_t data)
{
	uint8_t temp;
	uint8_t i;
	uint8_t ret;
	for(i = 0; i < 7; i++)
	{
		temp = ((data & (0x80 >> i)) >> (7 - i)) + 0x30;
		file_write_char(file, temp);
	}
	ret = 8;
	return ret;
}

// 输出浮点数, uint8_t fract_len:小数点位数
uint8_t file_write_float32(uint8_t file, float32_t data, uint8_t fract_len)
{
	uint8_t i;
	int32_t intpart;
	float32_t fracpart;
	uint8_t ret;
	ret = 0;

	if(fract_len > 8) fract_len = 8;
	else if(fract_len < 1) fract_len = 1;

	if(data < 0)  //data' = abs(data)
	{
		file_write_char(file, '-');
		ret++;
		data = -data;
	}

	intpart = (uint32_t)data;
	fracpart = data - (float32_t)intpart;

	ret += file_write_uint(file, (uint32_t)intpart);

	file_write_char(file, '.');
	ret++;

	for(i = 0; i < fract_len; i++)
	{
		fracpart = fracpart * 10;
		file_write_char(file, ((uint8_t)fracpart) % 10 + 0x30);
		ret++;
	}
	return ret;
}
