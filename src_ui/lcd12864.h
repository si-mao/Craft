/*
 * lcd12864.h
 * lcd12864
 * �����ߣ���è
 * ����ʱ�䣺2015.03.04
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.04
 */

#ifndef LCD12864_H_
#define LCD12864_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#include "stm32_delay.h"

//�ӳٿ���
#define T_WAIT_POWER_UP delayus(40000); //40ms
#define T_WAIT_CLEAR    delayus(1700);  //clear command: 1.6ms
#define T_WAIT          delayus(80);    //other command: 72us

extern void lcd12864_write_data(uint8_t data);
extern void lcd12864_write_cmd(uint8_t data);

extern void lcd12864_init(void);
extern void lcd12864_set_cursor(uint8_t CurY, uint8_t CurX);
//���÷���
extern void lcd12864_set_white(const uint8_t *pos, uint8_t width);
//��շ���
extern void lcd12864_clear_all_white(void);
extern void lcd12864_clear_white(const uint8_t *pos, uint8_t width);

//����+DDRAM��ַ��������
__INLINE void lcd12864_clear(void){ lcd12864_write_cmd(0x01); T_WAIT_CLEAR }
__INLINE void lcd12864_put_char(uint8_t data){ lcd12864_write_data(data); }
//��ʾ״̬��������ʾON+�α�OFF+�α귴��ON
__INLINE void lcd12864_open_cursor(void){ lcd12864_write_cmd(0x0D); }
  //��ʾ״̬��������ʾON+�α�OFF+�α귴��OFF
__INLINE void lcd12864_close_cursor(void){ lcd12864_write_cmd(0x0C); }


#ifdef __cplusplus
};
#endif

#endif
