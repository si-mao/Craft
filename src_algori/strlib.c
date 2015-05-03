/*
 * strlib.h
 * 字符串处理
 * 创建者：死猫
 * 创建时间：2015.03.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.15
 */

#include "system_define.h"

void strcpy(uint8_t* dst, const uint8_t* src, uint8_t len)
{
	uint8_t i;
	for(i = 0; i < len; i++)
	{
		dst[i] = src[i];
	}
}

//param 0 : 原始字符串
//param 1 : 欲寻找的字符串
//return  : 返回字符串第一次发现位置, 若未发现返回NULL

uint8_t* strfind(const uint8_t* src, uint8_t src_len, const uint8_t* sub, uint8_t sub_len)
{
	uint8_t *src_tmp;
	uint8_t *src_rear, *sub_rear;
	uint8_t *bp, *sp;
	
	if(src_len < sub_len) return NULL;
	if(sub_len == 0)      return (uint8_t *)src;
	
	src_rear = (uint8_t *)src + src_len;
	sub_rear = (uint8_t *)sub + sub_len;
	src_tmp = (uint8_t *)src;
	
	while(src_tmp < src_rear)
	{
		bp = src_tmp;
		sp = (uint8_t *)sub;
		while(*bp==*sp)
		{
			sp++;
			bp++;
			if(sp == sub_rear) return src_tmp;
		}
		src_tmp++;
	}

	return NULL;
}
