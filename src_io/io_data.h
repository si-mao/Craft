/*
 * io_data.h
 * ����������ݶ���
 * �����ߣ���è
 * ����ʱ�䣺2015.01.25
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.02.13
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
