/*
 * user_config.h
 * 用户配置
 * 创建者：死猫
 * 创建时间：2015.02.13
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.06
 */

#ifndef USER_CONFIG_H_
#define USER_CONFIG_H_

//packge配置
// #define PACKAGE_UART_USE
#define PACKAGE_RF_USE

//file配置
// #define FILE_UART_USE
// #define FILE_SCREEN_USE
// 	#define FILE_SCREEN_LCD12864_USE
// #endif

//error配置
#define ERROR_LED_ALARM   { LED1 = 1; }
#define ERROR_LED_DEALARM { LED1 = 0; }

//LED配置
#define LED_BREATH LED2
#endif
