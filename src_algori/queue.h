/*
 * queue.h
 * ���в���
 * �����ߣ���è
 * ����ʱ�䣺2015.01.18
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.04.24
 */
 
#ifndef QUEUE_H_
#define QUEUE_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _Queue_Type
{
	uint8_t *Queue;
	uint16_t Size;
	uint16_t Front;
	uint16_t Rear;
}Queue_Type;

extern void queue_init(Queue_Type* Queuex, uint8_t *buffer, uint16_t buffer_size);
extern void queue_reset(Queue_Type* Queuex);
extern void queue_enter(Queue_Type* Queuex, uint8_t dat);
extern uint8_t queue_delete(Queue_Type* Queuex, uint32_t *err);
extern uint16_t queue_data_num(Queue_Type* Queuex);

#ifdef __cplusplus
};
#endif

#endif
