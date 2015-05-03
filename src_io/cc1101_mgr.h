/*
 * cc1101_mgr.h
 * cc1101�ڴ����
 * �����ߣ���è
 * ����ʱ�䣺2015.04.24
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.04.24
 */
 
#ifndef CC1101_MGR_H_
#define CC1101_MGR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"



extern void cc1101_mgr_init(void);
extern void cc1101_mgr_recv_start(void);
extern void cc1101_mgr_send_start(const uint8_t *pack, uint8_t len);
extern void cc1101_mgr_loop(void);

extern void cc1101_mgr_read(uint8_t *pack, uint32_t *error);
extern void cc1101_mgr_write(const uint8_t *pack, uint8_t len, uint32_t *error);




#ifdef __cplusplus
};
#endif

#endif
