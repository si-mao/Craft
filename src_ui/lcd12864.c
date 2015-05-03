/*
 * lcd12864.c
 * lcd12864
 * �����ߣ���è
 * ����ʱ�䣺2015.03.04
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.13
 */

#include "system_define.h"
#include "hardware.h"		//io�˿ں궨��

#include "stm32_spi.h"

#include "lcd12864.h"

//1MHz 1�ַ�Լ3�ֽ����ݣ�Լ3us

//LCD����оƬ��ST7920
//�������ݸ�ʽ��
  //1st Byte: ���ڿ��Ƹ�ʽ 11111AB0
  //          A:���ݴ��ͷ�����ƣ�H:��LCD��MCU��L��֮
  //          B:��������ѡ��H���ݣ�Lָ��
  //2nd Byte: ���ݸ�4λ HHHH0000
  //3rd Byte: ���ݵ�4λ DDDD0000

//CS����ѡ����������λ��spi.c�ļ�
#define LCD12864_CS   { PBout(12) = 1; }
#define LCD12864_CS_DIS { PBout(12) = 0; }

//SPI1 SPI2ѡ��
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
	lcd12864_write_cmd(0x30);  //�����趨��8λ����+����ָ��
	lcd12864_write_cmd(0x0C);  //��ʾ״̬��������ʾON+�α�OFF+�α귴��OFF
	lcd12864_clear();          //������˳���ܱ�
	lcd12864_write_cmd(0x06);  //����д����������+���治��
	
	lcd12864_clear_all_white();
}

//CurX: ������, CurY������
void lcd12864_set_cursor(uint8_t CurX, uint8_t CurY)
{
	switch(CurY)
	{
	case 0: lcd12864_write_cmd(0x80+CurX); break; // д���һ��CurX�еĵ�ַ
	case 1: lcd12864_write_cmd(0x90+CurX); break; // д��ڶ���CurX�еĵ�ַ
	case 2: lcd12864_write_cmd(0x88+CurX); break; // д�������CurX�еĵ�ַ
	case 3: lcd12864_write_cmd(0x98+CurX); break; // д�������CurX�еĵ�ַ
	default: break;
	}
}

//pos[1]��������Ϊż��
//width����Ϊż��
void lcd12864_set_draw(const uint8_t *pos, uint8_t width, uint8_t dat)
{
	uint8_t i, j;

	lcd12864_write_cmd(0x36);  //�����ͼģʽ

	//��ѡ����
	for(i = 0; i < 16; i++)
	{
		if(pos[0] < 2)  //�ϰ���
		{
			lcd12864_write_cmd(0x80 + i + pos[0] * 16);       //y����
			lcd12864_write_cmd(0x80 + pos[1] / 2);            //x����
		}
		else
		{
			lcd12864_write_cmd(0x80 + i + (pos[0] - 2) * 16); //y����
			lcd12864_write_cmd(0x88 + pos[1] / 2);            //x����
		}
		for(j = 0; j < width; j++)
		{
			lcd12864_write_data(dat);
		}
	}
	
	lcd12864_write_cmd(0x30);  //�˳���ͼģʽ
}

//pos[1]��������Ϊż��
//width����Ϊż��
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

	lcd12864_write_cmd(0x36);  //�����ͼģʽ

	//��ѡ����
	for(i = 0; i < 32; i++)
	{
		lcd12864_write_cmd(0x80 + i);       //y����
		lcd12864_write_cmd(0x80);            //x����
		for(j = 0; j < 32; j++)
		{
			lcd12864_write_data(0x00);
		}
	}
	
	lcd12864_write_cmd(0x30);  //�˳���ͼģʽ
}

/*
void lcd12864_open_draw()
{
	lcd12864_write_cmd(0x36, SEL_SEND_CTRL);
	T_WAIT
}
//�ر�draw
void lcd12864_break_draw()
{
	lcd12864_write_cmd(0x34, SEL_SEND_CTRL);
	T_WAIT
}

void lcd12864_put_bmp(const uint8_t *puts)
{
	uint8_t byte_temp;    //Ϊ�ֽ�д�붨��
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
