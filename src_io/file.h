/*
 * file.h
 * 文件
 * 创建者：死猫
 * 创建时间：2015.03.06
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.21
 */
 
#ifndef FILE_H_
#define FILE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"
#include "queue.h"

//See "user_config.h"
#ifdef FILE_UART_USE
	#define FILE_UART_WO   0
	#define FILE_UART_WO_BUF 
#endif

#ifdef FILE_SCREEN_USE
	#define FILE_SCREEN_WO 1
#endif

#define FILE_RO         0x01
#define FILE_WO         0x02
#define FILE_RW         0x03


#define TYPE_BITS_MASK   0x000F
#define TYPE_INT_MASK    0x0030
#define TYPE_SIGNED_MASK 0x00C0

#define TYPE_1BITS    0x0001
#define TYPE_8BITS    0x0002
#define TYPE_16BITS   0x0003
#define TYPE_32BITS   0x0004
#define TYPE_64BITS   0x0005
#define TYPE_INT      0x0010
#define TYPE_FLOAT    0x0020
#define TYPE_UNSIGNED 0x0040
#define TYPE_SIGNED   0x0080

#define TYPE_NULL    0x0
#define TYPE_BOOL    (TYPE_UNSIGNED | TYPE_INT   | TYPE_1BITS )
#define TYPE_UINT8   (TYPE_UNSIGNED | TYPE_INT   | TYPE_8BITS )
#define TYPE_INT8    (TYPE_SIGNED   | TYPE_INT   | TYPE_8BITS )
#define TYPE_UINT16  (TYPE_UNSIGNED | TYPE_INT   | TYPE_16BITS)
#define TYPE_INT16   (TYPE_SIGNED   | TYPE_INT   | TYPE_16BITS)
#define TYPE_UINT32  (TYPE_UNSIGNED | TYPE_INT   | TYPE_32BITS)
#define TYPE_INT32   (TYPE_SIGNED   | TYPE_INT   | TYPE_32BITS)
#define TYPE_FLOAT32 (TYPE_SIGNED   | TYPE_FLOAT | TYPE_32BITS)
#define TYPE_FLOAT64 (TYPE_SIGNED   | TYPE_FLOAT | TYPE_64BITS)
//#define TYPE_BIN     (TYPE_UNSIGNED | TYPE_INT   | TYPE_8BITS )
//#define TYPE_HEX     (TYPE_UNSIGNED | TYPE_INT   | TYPE_32BITS)

#define IS_TYPE_INT(x)      (((x) & TYPE_INT_MASK)    == TYPE_INT      ? 1 : 0)
#define IS_TYPE_FLOAT(x)    (((x) & TYPE_INT_MASK)    == TYPE_FLOAT    ? 1 : 0)
#define IS_TYPE_UNSIGNED(x) (((x) & TYPE_SIGNED_MASK) == TYPE_UNSIGNED ? 1 : 0)
#define IS_TYPE_SIGNED(x)   (((x) & TYPE_SIGNED_MASK) == TYPE_SIGNED   ? 1 : 0)

void file_init(void);
void file_loop(void);

void file_set_pos(uint8_t file, const uint8_t* pos);
void file_clear(uint8_t file, const uint8_t* pos, int8_t len);
void file_set_white(uint8_t file, const uint8_t* pos, uint8_t width);
void file_clear_all_white(uint8_t file);
void file_clear_white(uint8_t file, const uint8_t* pos, uint8_t width);

extern Queue_Type* file_node[];
__INLINE void file_write_char(uint8_t file, uint8_t ch) { queue_enter(file_node[file], ch); }

extern uint8_t file_write(uint8_t file, const uint8_t* str);
extern uint8_t file_write_array(uint8_t file, uint8_t* data ,uint8_t length);

extern uint8_t file_write_num(uint8_t file, const void *data, uint8_t data_type);
extern uint8_t file_write_int(uint8_t file, int32_t data);
extern uint8_t file_write_uint(uint8_t file, uint32_t data);
extern uint8_t file_write_hex(uint8_t file, uint32_t data, uint8_t hex_len);
extern uint8_t file_write_bin(uint8_t file, uint8_t data);
extern uint8_t file_write_float32(uint8_t file, float32_t data, uint8_t n);


#ifdef __cplusplus
};
#endif

#endif
