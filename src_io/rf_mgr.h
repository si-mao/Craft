/*
 * rf_mgr.h
 * ���߹���
 * �����ߣ���è
 * ����ʱ�䣺2015.04.24
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.04.24
 */
 
#ifndef RF_MGR_H_
#define RF_MGR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "cc1101_mgr.h"

#define rf_mgr_init       cc1101_mgr_init
#define rf_mgr_recv_start cc1101_mgr_recv_start
#define rf_mgr_loop       cc1101_mgr_loop

#define rf_mgr_read       cc1101_mgr_read
#define rf_mgr_write      cc1101_mgr_write

#ifdef __cplusplus
};
#endif

#endif
