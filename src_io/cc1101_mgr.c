/*
 * cc1101_mgr.c
 * cc1101管理
 * 创建者：死猫
 * 创建时间：2015.04.24
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.04.24
 */

#include "system_define.h"

#include "cc1101.h"
#include "io_data.h"
#include "strlib.h"

#include "package.h"

#include "cc1101_mgr.h"

#define CC1101_BUF_SIZE  64

#define SR_MODE_NULL 0
#define SR_MODE_SEND 1
#define SR_MODE_RECV 2

uint8_t cc1101_dev_rdy_r;
uint8_t cc1101_rdy_r;

uint8_t cc1101_buf_r[CC1101_BUF_SIZE];
uint8_t cc1101_buf_r_swap[CC1101_BUF_SIZE];

uint8_t cc1101_dev_free_w;
uint8_t cc1101_free_w;

uint8_t cc1101_buf_w_swap[CC1101_BUF_SIZE];

uint8_t SR_Mode;

void cc1101_mgr_init(void)
{
	SR_Mode = SR_MODE_NULL;
	cc1101_dev_rdy_r = 0;
	cc1101_rdy_r = 0;
	cc1101_dev_free_w = 1;
	cc1101_free_w = 1;
}

void cc1101_mgr_read(uint8_t *pack, uint32_t *error)
{
	if(cc1101_rdy_r > 0)
	{
		strcpy(pack, cc1101_buf_r_swap, cc1101_buf_r_swap[0]);
		cc1101_rdy_r = 0;
		*error = 0;
	}
	else  //无数据可读
	{
		*error = 1;
	}
}

void cc1101_mgr_write(const uint8_t *pack, uint8_t len, uint32_t *error)
{
	if(cc1101_free_w > 0)
	{
		strcpy(cc1101_buf_w_swap, pack, len);
		cc1101_free_w = 0;
		*error = 0;
	}
	else  //上一个数据未发送完毕
	{
		*error = 1;
	}
}

void cc1101_mgr_loop(void)
{
	uint32_t err;
	uint8_t cc1101_enter_recv_flag = 0;  //是否启动接收模式

	//读
	//将外设数据转存到buf_r中
	if(cc1101_dev_rdy_r > 0)  //接收到数据
	{
		cc1101_enter_recv_flag = 1;

		if(cc1101_rdy_r > 0)  //缓存数据未被取出
		{
		}

		//读取数据
		cc1101_recv_get_data(cc1101_buf_r, CC1101_BUF_SIZE, &err);
		cc1101_dev_rdy_r = 0;

		if(0 == err)  //接收成功
		{
			if(0 == package_check(cc1101_buf_r))  //校验成功
			{
				strcpy(cc1101_buf_r_swap, cc1101_buf_r, CC1101_BUF_SIZE);
				cc1101_rdy_r++;
			}
			else  //校验失败
			{
			}
		}
	}

	//写
	//将buf_w_swap数据写入设备
	if(cc1101_free_w == 0)  //有数据存于缓存中
	{
		if(cc1101_dev_free_w > 0)  //外设可写
		{
			cc1101_enter_recv_flag = 0;

			//进入发送模式
			cc1101_dev_free_w = 0;
			cc1101_mgr_send_start(cc1101_buf_w_swap, cc1101_buf_w_swap[0]);
			cc1101_free_w++;
		}
		else if(SR_Mode == SR_MODE_SEND)  //外设不可写，当前处于发送状态, 无操作以等待外设可写
		{
			cc1101_enter_recv_flag = 0;
		}
		else  //外设不可写, 当前处于接收状态, 则状态出错, 强制进入发送模式
		{
			cc1101_enter_recv_flag = 0;

			//进入发送模式
			cc1101_dev_free_w = 0;
			cc1101_mgr_send_start(cc1101_buf_w_swap, cc1101_buf_w_swap[0]);
			cc1101_free_w++;
		}
	}

	if(cc1101_enter_recv_flag != 0)
	{
		//进入接收模式
		cc1101_mgr_recv_start();
	}
}

void cc1101_mgr_recv_start(void)
{
	cc1101_enter_idle_mode();
	cc1101_clear_recv_buff();
	SR_Mode = SR_MODE_RECV;
	cc1101_enter_recv_mode();
}

void cc1101_mgr_send_start(const uint8_t *pack, uint8_t len)
{
	cc1101_enter_idle_mode();
	cc1101_clear_send_buff();
	SR_Mode = SR_MODE_SEND;
	cc1101_inter_send(pack, len);
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)  //判断中断是否产生
	{
		if(SR_Mode != SR_MODE_SEND)  //(SR_Mode == SR_MODE_RECV or SR_MODE_NULL)
		{
			cc1101_dev_rdy_r++;
		}
		else  //(SR_Mode == SR_MODE_SEND)
		{
			cc1101_dev_free_w++;
			//进入接收模式
			cc1101_mgr_recv_start();
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除中断	
}
