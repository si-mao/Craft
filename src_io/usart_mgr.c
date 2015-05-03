/*
 * usart_mgr.c
 * usart内存管理
 * 创建者：死猫
 * 创建时间：2015.04.24
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.04.25
 */

#include "system_define.h"
#include "error.h"
#include "queue.h"

#include "io_data.h"
#include "strlib.h"

#include "package.h"

#define USART_FIFO_SIZE 128
#define USART_BUF_SIZE  64
#define USART_DMA_SIZE  128

uint8_t usart_rdy_r;
Queue_Type queue_usart_fifo_r;
uint8_t usart_buf_queue_r[USART_FIFO_SIZE];
uint8_t usart_buf_r_swap[USART_BUF_SIZE];
uint8_t usart_buf_r[USART_BUF_SIZE];

uint8_t usart_dev_free_w;
uint8_t usart_free_w;
Queue_Type queue_usart_fifo_w;
uint8_t usart_buf_queue_w[USART_FIFO_SIZE];
uint8_t usart_buf_w[USART_DMA_SIZE];

uint8_t usart_dma_buf_w[USART_DMA_SIZE];

void usart_mgr_pack_init(void);
void usart_mgr_pack_find(Queue_Type* queue_uart_recv, uint8_t *pack, uint32_t *error);

void usart_mgr_init(void)
{
	queue_init(&queue_usart_fifo_w, usart_buf_queue_w, USART_FIFO_SIZE);
	queue_init(&queue_usart_fifo_r, usart_buf_queue_r, USART_FIFO_SIZE);

	usart_dev_free_w = 1;
	usart_free_w = 1;

	usart_rdy_r = 0;
	
	usart_mgr_pack_init();
}

void usart_mgr_read(uint8_t *pack, uint32_t *error)
{
	if(usart_rdy_r > 0)
	{
		strcpy(pack, usart_buf_r_swap, usart_buf_r_swap[0]);
		usart_rdy_r = 0;
		*error = 0;
	}
	else  //无数据可读
	{
		*error = 1;
	}
}

void usart_mgr_fifo_write(uint8_t ch)
{
	queue_enter(&queue_usart_fifo_w, ch);
}

void usart_mgr_dma_write(const uint8_t *pack, uint8_t len, uint32_t *error)
{
	if(usart_dev_free_w > 0)
	{
		strcpy(usart_dma_buf_w, pack, len);
		usart_dev_free_w = 0;

		DMA1_Channel4->CMAR = (uint32_t)usart_dma_buf_w;
		DMA1_Channel4->CNDTR = len;
		DMA_Cmd(DMA1_Channel4, ENABLE);

		*error = 0;
	}
	else
	{
		*error = 1;
	}
}

void usart_mgr_loop(void)
{
	uint32_t err;
	uint8_t len = 0;
	//读
	usart_mgr_pack_find(&queue_usart_fifo_r, usart_buf_r_swap, &err);
	if(err == 1)  //收到一个数据包
	{
		usart_rdy_r++;
	}
	//写
	if(queue_data_num(&queue_usart_fifo_w) > 0)  //fifo有数据
	{
		if(usart_dev_free_w > 0)  //外设可写
		{
			//将fifo内数据全部取出
			usart_buf_w[len] = queue_delete(&queue_usart_fifo_w, &err);
			while(err == 0 && (++len) < USART_DMA_SIZE)
			{
				usart_buf_w[len] = queue_delete(&queue_usart_fifo_w, &err);
			}
			
			//发送数据
			if(len > 0) usart_mgr_dma_write(usart_buf_w, len, &err);
			//设置标志
			//uasrt_dev_free_w = 0;  //已在usart_mgr_dma_write()设置
		}
		else  //外设不可写，无操作以等待外设可写
		{
		}
	}
}

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4))
	{
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		DMA_Cmd(DMA1_Channel4, DISABLE);

		usart_dev_free_w++;
	}
}

void USART1_IRQHandler(void)
{
	uint8_t c;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		c = USART1->DR;
		queue_enter(&queue_usart_fifo_r, c);
	}
}

//################### 字节流中寻找数据包 ########################//
uint8_t  uart_state;
uint8_t *uart_start_pointer;
uint8_t  uart_recv_length;
uint8_t *uart_pack_pointer;

#define UART_PACK_START_LEN 5         //UART_PACK_START_LEN = strlen(pack_start)
#define UART_PACK_START "Starb"

#define UART_PACK_MAX           64
#define usart_buf_r_LEN         (2 * UART_PACK_MAX)
#define usart_buf_r_RENEW_SIZE  (UART_PACK_MAX)

void usart_mgr_pack_init(void)
{
	uart_state = 0;
	uart_start_pointer = usart_buf_r;
	uart_recv_length = 0;
	uart_pack_pointer = NULL;
}

//error: 0-> 未接收到, 1->接收到
void usart_mgr_pack_find(Queue_Type* queue_uart_recv, uint8_t *pack, uint32_t *error)
{
	uint32_t errcode;
	uint8_t *pos;
	uint8_t c;
	
	*error = 0;
	do
	{
		c = queue_delete(queue_uart_recv, &errcode);
		if(0 == errcode)
		{
			uart_start_pointer[uart_recv_length] = c;
			uart_recv_length++;
			switch(uart_state)
			{
				case 0:  //接收前导
				{
					//若接收数据达到或超过"前导长度"
					//    寻找"前导"
					//    若寻找到前导
					//        {前导接收成功}
					//    若未寻找到前导
					//        判断垃圾字符是否超过阈值
					//            若超过, 则复制"前导长度-1"字节到buffer最前端
					if(uart_recv_length >= UART_PACK_START_LEN)
					{
						pos = strfind(uart_start_pointer, uart_recv_length, UART_PACK_START, UART_PACK_START_LEN);
						if(pos != NULL)
						{
							uart_state++;
							uart_recv_length -= pos - uart_start_pointer;
							uart_start_pointer = pos;

							uart_pack_pointer = uart_start_pointer + UART_PACK_START_LEN;
						}
						else
						{
							if(uart_recv_length >= usart_buf_r_RENEW_SIZE)
							{
								strcpy(usart_buf_r, uart_start_pointer + uart_recv_length - (UART_PACK_START_LEN - 1), UART_PACK_START_LEN - 1);

								uart_start_pointer = usart_buf_r;
								uart_recv_length = UART_PACK_START_LEN - 1;
							}
						}
					}
				}
				break;
				case 1:  //接收数据包长度及长度校验
				{
					//若接收数据达到或超过"前导长度+2"
					//    校验数据包长度
					//    若校验成功
					//        {数据包长度接收成功}
					//    若校验失败
					//        则删除1个字符, 且state转为"前导接收模式"
					if(uart_recv_length >= UART_PACK_START_LEN + 2)
					{
						if((uart_pack_pointer[0] + uart_pack_pointer[1]) == CHECK_CODE)  //校验长度成功
						{
							uart_state++;
						}
						else
						{
							uart_state = 0;
							uart_start_pointer++;
							uart_recv_length--;
						}
					}
				}
				break;
				case 2:  //接收数据包和校验位
				{
					//若接收数据达到或超过"前导长度+数据包长度"
					//    校验数据包
					//    若校验成功
					//        {校验成功}
					//    若校验失败
					//        则删除1个字符, 且state转为"前导接收模式"
					if(uart_recv_length >= UART_PACK_START_LEN + uart_start_pointer[UART_PACK_START_LEN + 0])
					{
						if(0 == package_check(uart_pack_pointer))  //校验成功
						{
							strcpy(pack, uart_pack_pointer, uart_pack_pointer[0]);

							uart_state = 0;
							uart_start_pointer = usart_buf_r;
							uart_recv_length = 0;

							//在此跳出循环, 使剩余的数据包在下次进行解包
							*error = 1;
							break;
						}
						else  //校验失败
						{
							uart_state = 0;
							uart_start_pointer++;
							uart_recv_length--;
						}
					}
				}
				break;
				default:
				{
					error_alarm(ERROR_CODE_EX_UART | ERROR_CODE_SOFTFAULT);
				}
				break;
			}
		}
	}while(0 == errcode);
}
