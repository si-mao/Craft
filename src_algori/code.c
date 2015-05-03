/*
 * code.h
 * 编码解码
 * 创建者：死猫
 * 创建时间：2015.02.13
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.02.13
 */

#include "system_define.h"

void code_encode_16bits(const uint16_t *src, uint8_t *dst, uint8_t len)
{
	uint8_t i;
	uint8_t *dst_tem;
	for(i = 0; i < len; i++)
	{
		dst_tem = dst + 2 * i;
		dst_tem[0] = (src[i] & 0xFF00) >> 8;
		dst_tem[1] = (src[i] & 0x00FF);
	}
}

void code_encode_32bits(const uint32_t *src, uint8_t *dst, uint8_t len)
{
	uint8_t i;
	uint8_t *dst_tem;
	for(i = 0; i < len; i++)
	{
		dst_tem = dst + 4 * i;
		dst_tem[0] = (src[i] & 0xFF000000) >> 24;
		dst_tem[1] = (src[i] & 0x00FF0000) >> 16;
		dst_tem[2] = (src[i] & 0x0000FF00) >> 8;
		dst_tem[3] = (src[i] & 0x000000FF);
	}
}

void code_decode_16bits(const uint8_t *src, uint16_t *dst, uint8_t len)
{
	uint8_t i;
	const uint8_t *src_tem;
	for(i = 0; i < len; i++)
	{
		src_tem = src + 2 * i;
		dst[i] = (((uint16_t)src_tem[0]) << 8) | ((uint16_t)src_tem[1]);
	}
}

void code_decode_32bits(const uint8_t *src, uint32_t *dst, uint8_t len)
{
	uint8_t i;
	const uint8_t *src_tem;
	for(i = 0; i < len; i++)
	{
		src_tem = src + 4 * i;
		dst[i] = (((uint32_t)src_tem[0]) << 24)
					| (((uint32_t)src_tem[1]) << 16)
					| (((uint32_t)src_tem[2]) << 8)
					| ((uint32_t)src_tem[3]);
	}
}
