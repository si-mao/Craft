/*
 * inter_inter.c
 * 定时器中断处理程序
 * 创建者：死猫
 * 创建时间：2015.01.20
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.23
 */


#include "system_define.h"
#include "hardware.h"		//io端口宏定义
#include "stm32_systick.h"

//STM32
#include "stm32_gpio.h"
#include "stm32_key.h"

#include "file.h"

#include "adc_roll.h"
#include "menu.h"
#include "my_math.h"

#include "package.h"
#include "craft_data.h"
#include "io_data.h"

extern volatile uint8_t uart_recv_count;  //位于inter_usart.c

void TIM2_IRQHandler()  //10ms
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 		//判断中断标志位是否为1；为1则中断响应啦，0则没有
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);       	//清除中断挂起位
		TSET_1 = 1;
		
		adc_roll_loop();
		TSET_1 = 0;
	}
}

#define STATE_MENU_ITEM        0
#define STATE_MENU_VALUE       1
#define STATE_CRAFT_SEND_CMD       2
#define STATE_CRAFT_SEND_CMD_START 3
#define STATE_CRAFT_SEND_CMD_END   4
#define STATE_CRAFT_SEND_SET_FILED 5
#define STATE_CRAFT_SEND_PID       6
#define STATE_CRAFT_RC             7

void *value_point;
int32_t value_input_int;
float32_t value_input_float;
uint8_t value_type;
uint8_t val_or_act_select;
uint32_t value_action;
uint8_t state;

void inter_time_data_init(void)
{
	value_point = NULL;
	value_input_int = 0;
	value_input_float = 0;
	value_type = TYPE_INT32;
	val_or_act_select = 0;
	value_action = 0;
	state = STATE_MENU_ITEM;
}

void menu_press_enter_callback(menu_inf* menu)
{
	val_or_act_select = menu -> val_or_act_select;
	value_action = menu -> node_select -> action;
	value_point = menu -> node_select -> value;
	value_type = menu -> node_select -> value_type;
	
	value_input_int = 0;
	value_input_float = 0;
	adc_roll_clear_data(0);
	adc_roll_clear_data(1);

	switch(value_action)
	{
		case MENU_ACT_RO: break;
		case MENU_ACT_WO:
		case MENU_ACT_RW:
		{
			if(val_or_act_select != 0)  //如果进入value_select模式
			{
				switch(value_type)
				{
					case TYPE_BOOL:    value_input_int = (int32_t)(*((uint8_t   *)value_point)); break;
					case TYPE_UINT8:   value_input_int = (int32_t)(*((uint8_t   *)value_point)); break;
					case TYPE_INT8:    value_input_int = (int32_t)(*((int8_t    *)value_point)); break;
					case TYPE_UINT16:  value_input_int = (int32_t)(*((uint16_t  *)value_point)); break;
					case TYPE_INT16:   value_input_int = (int32_t)(*((int16_t   *)value_point)); break;
					case TYPE_UINT32:  value_input_int = (int32_t)(*((uint16_t  *)value_point)); break;
					case TYPE_INT32:   value_input_int = (int32_t)(*((int32_t   *)value_point)); break;
					case TYPE_FLOAT32: value_input_float = (float32_t)(*((float32_t *)value_point)); break;
					default: break;
				}
				state = STATE_MENU_VALUE;
			}
		}
		break;
		case MENU_ACT_CRAFT_SEND_CMD:
		{
			if(val_or_act_select != 0)  //如果进入val_or_act_select模式
			{
				state = STATE_CRAFT_SEND_CMD;
			}
		}
		break;
		case MENU_ACT_CRAFT_SEND_CMD_START:
		{
			if(val_or_act_select != 0)  //如果进入val_or_act_select模式
			{
				state = STATE_CRAFT_SEND_CMD_START;
			}
		}
		break;
		case MENU_ACT_CRAFT_SEND_CMD_END:
		{
			if(val_or_act_select != 0)  //如果进入val_or_act_select模式
			{
				state = STATE_CRAFT_SEND_CMD_END;
			}
		}
		break;
		case MENU_ACT_CRAFT_SEND_SET_FILED:
		{
			if(val_or_act_select != 0)  //如果进入val_or_act_select模式
			{
				state = STATE_CRAFT_SEND_SET_FILED;
			}
		}
		break;
		case MENU_ACT_CRAFT_SEND_PID:
		{
			if(val_or_act_select != 0)  //如果进入val_or_act_select模式
			{
				state = STATE_CRAFT_SEND_PID;
			}
		}
		break;
		case MENU_ACT_CRAFT_RC:
		{
			if(val_or_act_select != 0)  //如果进入val_or_act_select模式
			{
				value_input_int = *((uint16_t *)value_point);
				state = STATE_CRAFT_RC;
			}
		}
		break;
		default: state = STATE_MENU_ITEM; break;
	}
}

void menu_press_back_callback(menu_inf* menu)
{
	inter_time_data_init();
	val_or_act_select = menu -> val_or_act_select;
	state = STATE_MENU_ITEM;
	adc_roll_clear_data(0);
	adc_roll_clear_data(1);
}

#define POW10_MIN  (-2)
#define POW10_MAX  (5)
#define POW10_SIZE ((POW10_MAX) - (POW10_MIN) + 1)
const float32_t pow10[POW10_SIZE] = {0.01, 0.1, 1, 10, 100, 1000, 10000, 100000};

void TIM3_IRQHandler()  //100ms
{
	int8_t i;
	int16_t roll[2];
	uint8_t pos[2];
	static int32_t scale = 0;
	int16_t roll_value;

	uint8_t key1_press_flag;
	uint8_t key2_press_flag;
	static uint8_t KEY1_prev = 1;
	static uint8_t KEY2_prev = 1;

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 		//判断中断标志位是否为1；为1则中断响应啦，0则没有
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);       	//清除中断挂起位
		
		key1_press_flag = 0;
		if(KEY1_prev == 1 && KEY1 == 0)
		{
			key1_press_flag = 1;
		}
		KEY1_prev = KEY1;
		
		key2_press_flag = 0;
		if(KEY2_prev == 1 && KEY2 == 0)
		{
			key2_press_flag = 1;
		}
		KEY2_prev = KEY2;
		
		
		roll[0] = adc_roll_get_data(0);
		roll[1] = adc_roll_get_data(1);
		
		switch(state)
		{
			case STATE_MENU_ITEM:
			{
				if(roll[0] > 0)
					menu_press_up();
				else if(roll[0] < 0)
					menu_press_down();
				adc_roll_clear_data(0);
				
				if(0 != key1_press_flag)  //Back
				{
					menu_press_back();
				}
				else if(0 != key2_press_flag)  //Enter
				{
					menu_press_enter();
				}
			}
			break;
			case STATE_MENU_VALUE:
			{
				if(roll[0] > 0)
					scale++;
				else if(roll[0] < 0)
					scale--;
				adc_roll_clear_data(0);
				
				if(roll[1] > 0)
					roll_value = 1;
				else if(roll[1] < 0)
					roll_value = -1;
				else
					roll_value = 0;
				adc_roll_clear_data(1);

				MY_CONSTRAIN(scale, POW10_MIN, POW10_MAX);

				if(IS_TYPE_INT(value_type) != 0)
				{
					value_input_int += (int32_t)pow10[scale - POW10_MIN] * (int32_t)roll_value;
					//限幅
					if(IS_TYPE_UNSIGNED(value_type) != 0)
					{
						MY_CONSTRAIN_DOWN(value_input_int, 0);
						switch(value_type)
						{
							case TYPE_BOOL:    MY_CONSTRAIN(value_input_int, 0,      1);      break;
							case TYPE_UINT8:   MY_CONSTRAIN(value_input_int, 0,      255);    break;
							case TYPE_INT8:    MY_CONSTRAIN(value_input_int, -128,   127);    break;
							case TYPE_UINT16:  MY_CONSTRAIN(value_input_int, 0,      65535);  break;
							case TYPE_INT16:   MY_CONSTRAIN(value_input_int, -32768, 32767);  break;
							case TYPE_UINT32:  break;
							case TYPE_INT32:   break;
							case TYPE_FLOAT32: break;
							default: break;
						}
						if(value_type == TYPE_BOOL)
						{
							MY_CONSTRAIN_UP(value_input_int, 1);
						}
					}
				}
				else
				{
					value_input_float += (float32_t)pow10[scale - POW10_MIN] * (float32_t)roll_value;
				}

				//Press Enter or Back
				if(0 != key1_press_flag)  //Back
				{
					menu_press_back();
				}
				else if(0 != key2_press_flag)  //Enter
				{
					//保存值
					switch(value_type)
					{
						case TYPE_BOOL:    *((uint8_t   *)value_point) = (uint8_t  )value_input_int; break;
						case TYPE_UINT8:   *((uint8_t   *)value_point) = (uint8_t  )value_input_int; break;
						case TYPE_INT8:    *((int8_t    *)value_point) = (int8_t   )value_input_int; break;
						case TYPE_UINT16:  *((uint16_t  *)value_point) = (uint16_t )value_input_int; break;
						case TYPE_INT16:   *((int16_t   *)value_point) = (int16_t  )value_input_int; break;
						case TYPE_UINT32:  *((uint16_t  *)value_point) = (uint16_t )value_input_int; break;
						case TYPE_INT32:   *((int32_t   *)value_point) = (int32_t  )value_input_int; break;
						case TYPE_FLOAT32: *((float32_t *)value_point) = (float32_t)value_input_float; break;
						default: break;
					}
					menu_press_enter();
				}
			}
			break;
			case STATE_CRAFT_SEND_CMD:
			{
				package_master_encode_cmd(package_uart_recv_buff);
				package_master_decode(package_uart_recv_buff);
				state = STATE_MENU_ITEM;
				menu_press_back();  //退出value_select模式
			}
			break;
			case STATE_CRAFT_SEND_CMD_START:
			{
				flags.power = POWER_ON;
				flags.run = RUN_MOV;
				flags.echo = ECHO_NULL;
				flags.algori = ALGORI_DEFAULT;
				package_master_encode_cmd(package_uart_recv_buff);
				package_master_decode(package_uart_recv_buff);
				state = STATE_MENU_ITEM;
				menu_press_back();  //退出value_select模式
			}
			break;
			case STATE_CRAFT_SEND_CMD_END:
			{
				flags.power = POWER_OFF;
				flags.run = RUN_IDLE;
				flags.echo = ECHO_NULL;
				flags.algori = ALGORI_DEFAULT;
				package_master_encode_cmd(package_uart_recv_buff);
				package_master_decode(package_uart_recv_buff);
				state = STATE_MENU_ITEM;
				menu_press_back();  //退出value_select模式
			}
			break;
			case STATE_CRAFT_SEND_SET_FILED:
			{
				package_uart_recv_buff[0] = 5;
				package_uart_recv_buff[1] = 0;
				package_uart_recv_buff[2] = MODE_CTRL;
				package_uart_recv_buff[3] = CTRL_SET_FIELD;
				package_uart_recv_buff[4] = 0;
				package_en_check(package_uart_recv_buff);
				//package_master_encode_cmd(uart_recv_buff);
				package_master_decode(package_uart_recv_buff);
				state = STATE_MENU_ITEM;
				menu_press_back();  //退出value_select模式
			}
			break;
			case STATE_CRAFT_SEND_PID:
			{
				package_master_encode_pid(package_uart_recv_buff);
				package_master_decode(package_uart_recv_buff);
				state = STATE_MENU_ITEM;
				menu_press_back();  //退出value_select模式
			}
			break;
			case STATE_CRAFT_RC:
			{
// 				if(roll[0] > 0)
// 					scale++;
// 				else if(roll[0] < 0)
// 					scale--;
// 				adc_roll_clear_data(0);
// 				
// 				if(roll[1] > 0)
// 					roll_value = 1;
// 				else if(roll[1] < 0)
// 					roll_value = -1;
// 				else
// 					roll_value = 0;
// 				adc_roll_clear_data(1);
// 				
// 				MY_CONSTRAIN(scale, POW10_MIN, POW10_MAX);
// 				
// 				MY_CONSTRAIN(scale, 0, 1);
// 				
// 				value_input_int += (float32_t)pow10[scale - POW10_MIN] * (float32_t)roll_value;

				//输入值为当前值与初始值的差值
				value_input_int = ((int32_t)adc_get_data(1) - (int32_t)adc_get_data_after_clear(1)) / (ADC_FULL_RANGE / 1000);
				if(value_input_int < 10)  //输入过小则不输出
				{
					if(value_input_int < 0)  //若当前值小于初始值, 则调整初始值为当前值
					{
						adc_roll_clear_data(1);
					}
					value_input_int = 0;
				}
				MY_CONSTRAIN(value_input_int, 0, 800);

				//Press Back
				if(0 != key1_press_flag)  //Back
				{
					rcData[THROTTLE] = 0;
				}
				else
				{
					rcData[THROTTLE] = value_input_int;
				}
				
				//Send package
				package_master_encode_rc(package_uart_recv_buff);
				package_master_decode(package_uart_recv_buff);
				
				//Press Back
				if(0 != key1_press_flag)  //Back
				{
					menu_press_back();
				}
			}
			break;
			default: break;
		}

		menu_loop();
		
		pos[0] = 3;
		pos[1] = 0;
		file_set_pos(FILE_SCREEN_WO, pos);
		
		pos[1] += file_write(FILE_SCREEN_WO, "x10^");
		pos[1] += file_write_int(FILE_SCREEN_WO, scale);
		for(i = 0; i <  8 - pos[1]; i++)
		{
			file_write_char(FILE_SCREEN_WO, ' ');
		}
		
		pos[1] = 8;
		file_set_pos(FILE_SCREEN_WO, pos);
		
		if(IS_TYPE_FLOAT(value_type) == 0)  //整数
		{
			if(value_type == TYPE_BOOL)     //bool
			{
				pos[1] += file_write(FILE_SCREEN_WO, (const uint8_t *)(value_input_int == 0 ? "false" : "true"));
			}
			else                            //整数
			{
				pos[1] += file_write_int(FILE_SCREEN_WO, value_input_int);
			}
		}
		else                             //浮点数
		{
			pos[1] += file_write_float32(FILE_SCREEN_WO, value_input_float, 2);
		}
		for(i = 0; i <  16 - pos[1]; i++)
		{
			file_write_char(FILE_SCREEN_WO, ' ');
		}
	}
}
