/*
 * lcd12864.c
 * lcd12864
 * 创建者：死猫
 * 创建时间：2015.03.04
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.13
 */

#include "system_define.h"
#include "hardware.h"		//io端口宏定义

#include "stm32_spi.h"

#include "lcd12864.h"

//1MHz 1字符约3字节数据，约3us

//LCD控制芯片：ST7920
//串行数据格式：
  //1st Byte: 串口控制格式 11111AB0
  //          A:数据传送方向控制，H:由LCD到MCU，L反之
  //          B:数据类型选择，H数据，L指令
  //2nd Byte: 数据高4位 HHHH0000
  //3rd Byte: 数据低4位 DDDD0000

//CS引脚选择，引脚配置位于spi.c文件
#define LCD12864_CS   { PBout(12) = 1; }
#define LCD12864_CS_DIS { PBout(12) = 0; }

//SPI1 SPI2选择
#define LCD12864_SPI 2  // 1 -> SPI1, 2-> SPI2

#if (LCD12864_SPI == 1)

#define SPI_RW spi1_rw

#elif (LCD12864_SPI == 2)

#define SPI_RW spi2_rw

#endif

#define SEL_SEND_DATA 0xFA //A_L, B_H
#define SEL_SEND_CTRL 0xF8 //A_L, B_L
#define SEL_GET_DATA 0xFE  //A_H, B_H
#define SEL_GET_CTRL 0xFC  //A_H, B_L



void lcd12864_write_cmd(uint8_t data)
{
	LCD12864_CS
	SPI_RW(SEL_SEND_CTRL);
	SPI_RW(data & 0xF0);
	SPI_RW(data << 4);
	LCD12864_CS_DIS
	T_WAIT
}

void lcd12864_write_data(uint8_t data)
{
	LCD12864_CS
	SPI_RW(SEL_SEND_DATA);
	SPI_RW(data & 0xF0);
	SPI_RW(data << 4);
	LCD12864_CS_DIS
}

void lcd12864_init(void)
{
	T_WAIT_POWER_UP
	lcd12864_write_cmd(0x30);  //功能设定，8位数据+基本指令
	lcd12864_write_cmd(0x0C);  //显示状态，整体显示ON+游标OFF+游标反白OFF
	lcd12864_clear();          //清屏，顺序不能变
	lcd12864_write_cmd(0x06);  //数据写入则光标右移+画面不动
	
	lcd12864_clear_all_white();
}

//CurX: 横坐标, CurY纵坐标
void lcd12864_set_cursor(uint8_t CurX, uint8_t CurY)
{
	switch(CurY)
	{
	case 0: lcd12864_write_cmd(0x80+CurX); break; // 写入第一行CurX列的地址
	case 1: lcd12864_write_cmd(0x90+CurX); break; // 写入第二行CurX列的地址
	case 2: lcd12864_write_cmd(0x88+CurX); break; // 写入第三行CurX列的地址
	case 3: lcd12864_write_cmd(0x98+CurX); break; // 写入第四行CurX列的地址
	default: break;
	}
}

//pos[1]列数必须为偶数
//width必须为偶数
void lcd12864_set_draw(const uint8_t *pos, uint8_t width, uint8_t dat)
{
	uint8_t i, j;

	lcd12864_write_cmd(0x36);  //进入绘图模式

	//对选择行
	for(i = 0; i < 16; i++)
	{
		if(pos[0] < 2)  //上半屏
		{
			lcd12864_write_cmd(0x80 + i + pos[0] * 16);       //y坐标
			lcd12864_write_cmd(0x80 + pos[1] / 2);            //x坐标
		}
		else
		{
			lcd12864_write_cmd(0x80 + i + (pos[0] - 2) * 16); //y坐标
			lcd12864_write_cmd(0x88 + pos[1] / 2);            //x坐标
		}
		for(j = 0; j < width; j++)
		{
			lcd12864_write_data(dat);
		}
	}
	
	lcd12864_write_cmd(0x30);  //退出绘图模式
}

//pos[1]列数必须为偶数
//width必须为偶数
void lcd12864_set_white(const uint8_t *pos, uint8_t width)
{
	lcd12864_set_draw(pos, width, 0xFF);
}

void lcd12864_clear_white(const uint8_t *pos, uint8_t width)
{
	lcd12864_set_draw(pos, width, 0x00);
}

void lcd12864_clear_all_white(void)
{
	/*
	lcd12864_set_draw("\x00\x00", 16, 0x00);
	lcd12864_set_draw("\x01\x00", 16, 0x00);
	lcd12864_set_draw("\x02\x00", 16, 0x00);
	lcd12864_set_draw("\x03\x00", 16, 0x00);
	*/
	uint8_t i, j;

	lcd12864_write_cmd(0x36);  //进入绘图模式

	//对选择行
	for(i = 0; i < 32; i++)
	{
		lcd12864_write_cmd(0x80 + i);       //y坐标
		lcd12864_write_cmd(0x80);            //x坐标
		for(j = 0; j < 32; j++)
		{
			lcd12864_write_data(0x00);
		}
	}
	
	lcd12864_write_cmd(0x30);  //退出绘图模式
}

/*
void lcd12864_open_draw()
{
	lcd12864_write_cmd(0x36, SEL_SEND_CTRL);
	T_WAIT
}
//关闭draw
void lcd12864_break_draw()
{
	lcd12864_write_cmd(0x34, SEL_SEND_CTRL);
	T_WAIT
}

void lcd12864_put_bmp(const uint8_t *puts)
{
	uint8_t byte_temp;    //为字节写入定义
	uint8_t i,j,k;
	uint32_t bb = 0;
	lcd12864_write_cmd(0x36, SEL_SEND_CTRL);
	lcd12864_write_cmd(0x32, SEL_SEND_CTRL);
	lcd12864_write_cmd(0x01, SEL_SEND_CTRL);
	lcd12864_write_cmd(0x40, SEL_SEND_CTRL);	//Set Display Start Line = com0
	for(i = 0; i <= 0x1F; i++)
	{  
		for(j = 0; j < 0x08; j++)
		{
			lcd12864_write_cmd(0x36, SEL_SEND_CTRL);

			lcd12864_write_cmd((0x80|i), SEL_SEND_CTRL);	//Set Page Address
			lcd12864_write_cmd((0x80|j), SEL_SEND_CTRL);	//Set Column Address = 0

			lcd12864_write_data(0x32, SEL_SEND_CTRL);
			for(k = 0; k < 2; k++)
			{
				byte_temp=(uint8_t)puts[bb];
				lcd12864_write_data(byte_temp, SEL_SEND_DATA);
				bb++;
			}
		}
	}
	for(i = 0; i <= 0x1F; i++)
	{  
		for(j = 0x08; j < 0x10; j++)
		{
			lcd12864_write_cmd(0x36, SEL_SEND_CTRL);
			lcd12864_write_cmd((0x80|i), SEL_SEND_CTRL);	//Set Page Address
			lcd12864_write_cmd((0x80|j), SEL_SEND_CTRL);	//Set Column Address = 0
			lcd12864_write_cmd(0x32, SEL_SEND_CTRL);
			for(k = 0; k < 2; k++)
			{
				byte_temp = (uint8_t)puts[bb];
				lcd12864_write_data(byte_temp, SEL_SEND_DATA);
				bb++;
			}
		}
	}
}
*/
