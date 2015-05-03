/*
 * package_master.h
 * Master打包解包
 * 创建者：死猫
 * 创建时间：2015.01.24
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.23
 */

#include "error.h"
#include "file.h"
#include "io_data.h"
#include "rf_mgr.h"
#include "usart_mgr.h"

#include "package.h"
#include "code.h"

#include "craft_data.h"

void package_uart_recv(const uint8_t *pack)
{
	package_master_decode(pack);
}

void package_cc1101_recv(const uint8_t *pack)
{
	package_master_encode(pack);
}

void package_master_decode(const uint8_t *pack)
{
	int8_t i;
	int8_t pack_len;
	uint32_t err;
	pack_len = pack[0];
	for(i = 0; i < pack_len; i++)
	{
		package_cc1101_send_buff[i] = pack[i];
	}
	rf_mgr_write(package_cc1101_send_buff, pack_len, &err);
}

void package_master_encode(const uint8_t *pack)
{
	int8_t i;
	int8_t pack_len;
	uint32_t err;
	const uint8_t start[] = {"Starb"};
	pack_len = pack[0];
	for(i = 0; i < 5; i++)
	{
		package_uart_send_buff[i] = start[i];
	}
	for(i = 0; i < pack_len; i++)
	{
		package_uart_send_buff[i + 5] = pack[i];
	}

	//DMA发送
	usart_mgr_dma_write(package_uart_send_buff, pack_len + 5, &err);
	
	//FIFO发送
// 	for(i = 0; i < pack_len + 5; i++)
// 	{
// 		usart_mgr_fifo_write(package_uart_send_buff[i]);
// 	}
}

void package_master_encode_cmd(uint8_t *pack)
{
	pack[0] = SIZE_CMD;
	pack[1] = 0;
	pack[2] = MODE_CMD;
	pack[3] = flags.power;
	pack[4] = flags.run;
	pack[5] = flags.echo;
	pack[6] = flags.algori;
	pack[7] = 0;
	package_en_check(pack);
}

void package_master_encode_pid(uint8_t *pack)
{
	pack[0] = SIZE_CTRL_SET_PID;
	pack[1] = 0;
	pack[2] = MODE_CTRL;
	pack[3] = CTRL_SET_PID;
	pack[4] = (1 << PID_ROLL_ID) | (1 << PID_ROLL_ID) | (1 << PID_ROLL_ID);
	code_encode_32bits((uint32_t *)&conf.PID[0], pack + 5, 3);
	package_en_check(pack);
}

void package_master_encode_rc(uint8_t *pack)
{
	pack[0] = SIZE_RC;
	pack[1] = 0;
	pack[2] = MODE_RC;
	code_encode_16bits((uint16_t *)rcData, pack + 3, RC_CHANS);
	pack[SIZE_RC - 1] = 0;
	package_en_check(pack);
}
