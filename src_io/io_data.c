/*
 * io_data.c
 * ����������ݶ���
 * �����ߣ���è
 * ����ʱ�䣺2015.01.25
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.02.13
 */

#include "system_define.h"

#include "io_data.h"

//�������ݲ�����

uint8_t package_cc1101_recv_buff[PACKAGE_SIZE];
uint8_t package_cc1101_send_buff[PACKAGE_SIZE];


uint8_t package_uart_recv_buff[PACKAGE_SIZE];
uint8_t package_uart_send_buff[PACKAGE_SIZE];


void io_data_init(void)
{

}
