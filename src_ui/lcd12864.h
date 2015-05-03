/*
 * lcd12864.h
 * lcd12864
 * 创建者：死猫
 * 创建时间：2015.03.04
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.04
 */

#ifndef LCD12864_H_
#define LCD12864_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#include "stm32_delay.h"

//延迟控制
#define T_WAIT_POWER_UP delayus(40000); //40ms
#define T_WAIT_CLEAR    delayus(1700);  //clear command: 1.6ms
#define T_WAIT          delayus(80);    //other command: 72us

extern void lcd12864_write_data(uint8_t data);
extern void lcd12864_write_cmd(uint8_t data);

extern void lcd12864_init(void);
extern void lcd12864_set_cursor(uint8_t CurY, uint8_t CurX);
//设置反白
extern void lcd12864_set_white(const uint8_t *pos, uint8_t width);
//清空反白
extern void lcd12864_clear_all_white(void);
extern void lcd12864_clear_white(const uint8_t *pos, uint8_t width);

//清屏+DDRAM地址计数归零
__INLINE void lcd12864_clear(void){ lcd12864_write_cmd(0x01); T_WAIT_CLEAR }
__INLINE void lcd12864_put_char(uint8_t data){ lcd12864_write_data(data); }
//显示状态，整体显示ON+游标OFF+游标反白ON
__INLINE void lcd12864_open_cursor(void){ lcd12864_write_cmd(0x0D); }
  //显示状态，整体显示ON+游标OFF+游标反白OFF
__INLINE void lcd12864_close_cursor(void){ lcd12864_write_cmd(0x0C); }


#ifdef __cplusplus
};
#endif

#endif
