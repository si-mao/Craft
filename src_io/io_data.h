/*
 * io_data.h
 * 输入输出数据定义
 * 创建者：死猫
 * 创建时间：2015.01.25
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.02.13
 */
 
#ifndef IO_DATA_H_
#define IO_DATA_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#define PACKAGE_SIZE           64

extern uint8_t package_cc1101_recv_buff[PACKAGE_SIZE];
extern uint8_t package_cc1101_send_buff[PACKAGE_SIZE];

extern uint8_t package_uart_recv_buff[PACKAGE_SIZE];
extern uint8_t package_uart_send_buff[PACKAGE_SIZE];

extern void io_data_init(void);
#ifdef __cplusplus
};
#endif

#endif
