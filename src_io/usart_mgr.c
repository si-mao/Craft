/*
 * usart_mgr.c
 * usart�ڴ����
 * �����ߣ���è
 * ����ʱ�䣺2015.04.24
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.04.25
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
	else  //�����ݿɶ�
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
	//��
	usart_mgr_pack_find(&queue_usart_fifo_r, usart_buf_r_swap, &err);
	if(err == 1)  //�յ�һ�����ݰ�
	{
		usart_rdy_r++;
	}
	//д
	if(queue_data_num(&queue_usart_fifo_w) > 0)  //fifo������
	{
		if(usart_dev_free_w > 0)  //�����д
		{
			//��fifo������ȫ��ȡ��
			usart_buf_w[len] = queue_delete(&queue_usart_fifo_w, &err);
			while(err == 0 && (++len) < USART_DMA_SIZE)
			{
				usart_buf_w[len] = queue_delete(&queue_usart_fifo_w, &err);
			}
			
			//��������
			if(len > 0) usart_mgr_dma_write(usart_buf_w, len, &err);
			//���ñ�־
			//uasrt_dev_free_w = 0;  //����usart_mgr_dma_write()����
		}
		else  //���費��д���޲����Եȴ������д
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

//################### �ֽ�����Ѱ�����ݰ� ########################//
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

//error: 0-> δ���յ�, 1->���յ�
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
				case 0:  //����ǰ��
				{
					//���������ݴﵽ�򳬹�"ǰ������"
					//    Ѱ��"ǰ��"
					//    ��Ѱ�ҵ�ǰ��
					//        {ǰ�����ճɹ�}
					//    ��δѰ�ҵ�ǰ��
					//        �ж������ַ��Ƿ񳬹���ֵ
					//            ������, ����"ǰ������-1"�ֽڵ�buffer��ǰ��
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
				case 1:  //�������ݰ����ȼ�����У��
				{
					//���������ݴﵽ�򳬹�"ǰ������+2"
					//    У�����ݰ�����
					//    ��У��ɹ�
					//        {���ݰ����Ƚ��ճɹ�}
					//    ��У��ʧ��
					//        ��ɾ��1���ַ�, ��stateתΪ"ǰ������ģʽ"
					if(uart_recv_length >= UART_PACK_START_LEN + 2)
					{
						if((uart_pack_pointer[0] + uart_pack_pointer[1]) == CHECK_CODE)  //У�鳤�ȳɹ�
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
				case 2:  //�������ݰ���У��λ
				{
					//���������ݴﵽ�򳬹�"ǰ������+���ݰ�����"
					//    У�����ݰ�
					//    ��У��ɹ�
					//        {У��ɹ�}
					//    ��У��ʧ��
					//        ��ɾ��1���ַ�, ��stateתΪ"ǰ������ģʽ"
					if(uart_recv_length >= UART_PACK_START_LEN + uart_start_pointer[UART_PACK_START_LEN + 0])
					{
						if(0 == package_check(uart_pack_pointer))  //У��ɹ�
						{
							strcpy(pack, uart_pack_pointer, uart_pack_pointer[0]);

							uart_state = 0;
							uart_start_pointer = usart_buf_r;
							uart_recv_length = 0;

							//�ڴ�����ѭ��, ʹʣ������ݰ����´ν��н��
							*error = 1;
							break;
						}
						else  //У��ʧ��
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
