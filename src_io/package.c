/*
 * package.c
 * 数据包
 * 创建者：死猫
 * 创建时间：2015.01.24
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.25
 */

#include "system_define.h"

#include "package.h"

#include "io_data.h"

#include "rf_mgr.h"
#include "usart_mgr.h"

void package_listen_loop(void)
{
	uint32_t err;
#ifdef PACKAGE_UART_USE
	//UART
	usart_mgr_read(package_uart_recv_buff, &err);
	if(err == 0)  //收到一组数据
	{
		package_uart_recv(package_uart_recv_buff);
	}
#endif

#ifdef PACKAGE_RF_USE
	//RF
	rf_mgr_read(package_cc1101_recv_buff, &err);
	if(err == 0)  //收到一组数据
	{
		package_cc1101_recv(package_cc1101_recv_buff);
	}
#endif
}

//0无错误
uint8_t package_check(const uint8_t *src)
{
	uint8_t i;
	uint8_t sum;
	
	if((src[0] + src[1]) != CHECK_CODE)  //校验长度失败
	{
		return 1;
	}
	
	sum = 0;
	for(i = 0; i < src[0]; i++)
	{
		sum += src[i];
	}
	if(sum != CHECK_CODE)  //校验数据包失败
	{
		return 2;
	}
	return 0;
}

void package_en_check(uint8_t *dst)
{
	uint8_t i;
	uint8_t sum;
	uint8_t len;
	
	len = dst[0];

	dst[1] = CHECK_CODE - dst[0];  //产生长度校验
	
	sum = 0;
	for(i = 0; i < len - 1; i++)
	{
		sum += dst[i];
	}
	dst[len - 1] = CHECK_CODE - sum;  //产生数据包校验
}
