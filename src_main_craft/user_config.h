/*
 * user_config.h
 * �û�����
 * �����ߣ���è
 * ����ʱ�䣺2015.02.13
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.04.06
 */

#ifndef USER_CONFIG_H_
#define USER_CONFIG_H_

//packge����
// #define PACKAGE_UART_USE
#define PACKAGE_RF_USE

//file����
// #define FILE_UART_USE
// #define FILE_SCREEN_USE
// 	#define FILE_SCREEN_LCD12864_USE
// #endif

//error����
#define ERROR_LED_ALARM   { LED1 = 1; }
#define ERROR_LED_DEALARM { LED1 = 0; }

//LED����
#define LED_BREATH LED2
#endif
