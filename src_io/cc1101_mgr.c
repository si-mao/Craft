/*
 * cc1101_mgr.c
 * cc1101����
 * �����ߣ���è
 * ����ʱ�䣺2015.04.24
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.04.24
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
	else  //�����ݿɶ�
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
	else  //��һ������δ�������
	{
		*error = 1;
	}
}

void cc1101_mgr_loop(void)
{
	uint32_t err;
	uint8_t cc1101_enter_recv_flag = 0;  //�Ƿ���������ģʽ

	//��
	//����������ת�浽buf_r��
	if(cc1101_dev_rdy_r > 0)  //���յ�����
	{
		cc1101_enter_recv_flag = 1;

		if(cc1101_rdy_r > 0)  //��������δ��ȡ��
		{
		}

		//��ȡ����
		cc1101_recv_get_data(cc1101_buf_r, CC1101_BUF_SIZE, &err);
		cc1101_dev_rdy_r = 0;

		if(0 == err)  //���ճɹ�
		{
			if(0 == package_check(cc1101_buf_r))  //У��ɹ�
			{
				strcpy(cc1101_buf_r_swap, cc1101_buf_r, CC1101_BUF_SIZE);
				cc1101_rdy_r++;
			}
			else  //У��ʧ��
			{
			}
		}
	}

	//д
	//��buf_w_swap����д���豸
	if(cc1101_free_w == 0)  //�����ݴ��ڻ�����
	{
		if(cc1101_dev_free_w > 0)  //�����д
		{
			cc1101_enter_recv_flag = 0;

			//���뷢��ģʽ
			cc1101_dev_free_w = 0;
			cc1101_mgr_send_start(cc1101_buf_w_swap, cc1101_buf_w_swap[0]);
			cc1101_free_w++;
		}
		else if(SR_Mode == SR_MODE_SEND)  //���費��д����ǰ���ڷ���״̬, �޲����Եȴ������д
		{
			cc1101_enter_recv_flag = 0;
		}
		else  //���費��д, ��ǰ���ڽ���״̬, ��״̬����, ǿ�ƽ��뷢��ģʽ
		{
			cc1101_enter_recv_flag = 0;

			//���뷢��ģʽ
			cc1101_dev_free_w = 0;
			cc1101_mgr_send_start(cc1101_buf_w_swap, cc1101_buf_w_swap[0]);
			cc1101_free_w++;
		}
	}

	if(cc1101_enter_recv_flag != 0)
	{
		//�������ģʽ
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
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)  //�ж��ж��Ƿ����
	{
		if(SR_Mode != SR_MODE_SEND)  //(SR_Mode == SR_MODE_RECV or SR_MODE_NULL)
		{
			cc1101_dev_rdy_r++;
		}
		else  //(SR_Mode == SR_MODE_SEND)
		{
			cc1101_dev_free_w++;
			//�������ģʽ
			cc1101_mgr_recv_start();
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //����ж�	
}
