/*
 * queue.c
 * 队列操作
 * 创建者：死猫
 * 创建时间：2015.01.18
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.24
 */
 
 /*
 * Front位置无数据
 * 所以实际能容纳的数据为内存长度-1
 */

#include "system_define.h"

#include "queue.h"

#define QUEUE_MAX 8

#define _QUEUE (Queuex->Queue)
#define _SIZE  (Queuex->Size)
#define _FRONT (Queuex->Front)
#define _REAR  (Queuex->Rear)

void queue_init(Queue_Type* Queuex, uint8_t *buffer, uint16_t buffer_size)
{
	_QUEUE = buffer;
	_SIZE = buffer_size;
	_FRONT = 0;
	_REAR = 0;
}

void queue_reset(Queue_Type* Queuex)
{
	_FRONT = 0;
	_REAR = 0;
}

void queue_enter(Queue_Type* Queuex, uint8_t dat)
{
	_QUEUE[_REAR] = dat;
	_REAR = (_REAR + 1) % _SIZE;
	if(_REAR == _FRONT) _FRONT = (_FRONT + 1) % _SIZE;
}

//uint8_t *err : 空返回1，否则返回0
uint8_t queue_delete(Queue_Type* Queuex, uint32_t *err)
{
	uint8_t dat = 0;
	if(_REAR == _FRONT)
	{
		*err = 1;
		return 0;
	}
	dat = _QUEUE[_FRONT];
	_FRONT = (_FRONT + 1) % _SIZE;
	*err = 0;
	return dat;
}

uint16_t queue_data_num(Queue_Type* Queuex)
{
	if(_REAR >= _FRONT)
	{
		return (_REAR - _FRONT);
	}
	else
	{
		return (_SIZE + _REAR - _FRONT);
	}
}
