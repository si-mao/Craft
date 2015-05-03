/*
 * menu_data.c
 * 菜单数据
 * 创建者：死猫
 * 创建时间：2015.03.04
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.22
 */

#include "menu.h"
#include "file.h"
#include "craft_data.h"
#include "adc_roll.h"

uint32_t zero = 0;

menu_node menu_data[] = {
			MENU_NODE_INIT_DATA(1, 0, 0, MENU_ACT_NULL, MENU_VALUE_NULL, TYPE_NULL, "ADC->"),
			MENU_NODE_INIT_DATA(2, 0, 1, MENU_ACT_NULL, MENU_VALUE_NULL, TYPE_NULL, "Craft->"),

				MENU_NODE_INIT_DATA(10, 1, 0, MENU_ACT_RO, &ADC_Value[0], TYPE_UINT16, "[ADC0]"),
				MENU_NODE_INIT_DATA(11, 1, 1, MENU_ACT_RO, &ADC_Value[1], TYPE_UINT16, "[ADC1]"),

				MENU_NODE_INIT_DATA(20, 2, 0, MENU_ACT_CRAFT_SEND_CMD_START, MENU_VALUE_NULL,  TYPE_NULL,    "< 启动 >"),
				MENU_NODE_INIT_DATA(21, 2, 1, MENU_ACT_CRAFT_SEND_CMD_END,   MENU_VALUE_NULL,  TYPE_NULL,    "< 停止 >"),
				MENU_NODE_INIT_DATA(25, 2, 2, MENU_ACT_CRAFT_SEND_SET_FILED, MENU_VALUE_NULL, TYPE_NULL, "< 初姿 >"),
				MENU_NODE_INIT_DATA(22, 2, 3, MENU_ACT_CRAFT_RC,        &rcData[THROTTLE], TYPE_UINT16, "< 高度 >"),
				MENU_NODE_INIT_DATA(23, 2, 4, MENU_ACT_NULL,            MENU_VALUE_NULL,  TYPE_NULL,    "PID->"),
				MENU_NODE_INIT_DATA(24, 2, 5, MENU_ACT_NULL,            MENU_VALUE_NULL,  TYPE_NULL,    "状态->"),

					MENU_NODE_INIT_DATA(230, 23, 0, MENU_ACT_CRAFT_SEND_PID, MENU_VALUE_NULL,       TYPE_NULL,  "<Send>"),
					MENU_NODE_INIT_DATA(231, 23, 1, MENU_ACT_RW,        &conf.PID[0].kp, TYPE_FLOAT32, "[P]"),
					MENU_NODE_INIT_DATA(232, 23, 2, MENU_ACT_RW,        &conf.PID[0].ki, TYPE_FLOAT32, "[I]"),
					MENU_NODE_INIT_DATA(233, 23, 3, MENU_ACT_RW,        &conf.PID[0].kd, TYPE_FLOAT32, "[D]"),

					MENU_NODE_INIT_DATA(240, 24, 0, MENU_ACT_CRAFT_SEND_CMD, MENU_VALUE_NULL, TYPE_NULL,  "<Send>"),
					MENU_NODE_INIT_DATA(241, 24, 1, MENU_ACT_RW,        &flags.power,    TYPE_BOOL,  "[Power]"),
					MENU_NODE_INIT_DATA(242, 24, 2, MENU_ACT_RW,        &flags.run,      TYPE_UINT8, "[Run]"),
					MENU_NODE_INIT_DATA(243, 24, 3, MENU_ACT_RW,        &flags.echo,     TYPE_UINT8, "[Echo]"),
					MENU_NODE_INIT_DATA(244, 24, 4, MENU_ACT_RW,        &flags.algori,   TYPE_UINT8, "[Algori]"),

			MENU_NODE_INIT_DATA(MENU_ID_END, 0, 0, MENU_ACT_NULL, MENU_VALUE_NULL, TYPE_NULL, NULL)  //End of the data.
		};
