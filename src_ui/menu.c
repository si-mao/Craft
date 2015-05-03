/*
 * menu.c
 * 菜单
 * 创建者：死猫
 * 创建时间：2015.03.04
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.20
 */

#include "system_define.h"

#include "menu.h"
#include "menu_redir.h"


#define SCREEN_LEFT   0
#define SCREEN_TOP    0

#define SCREEN_ITEM_WIDTH   8
#define SCREEN_VALUE_WIDTH  8
#define SCREEN_HEIGHT       3

#define SCREEN_VALUE_LEFT   ((SCREEN_LEFT) + (SCREEN_ITEM_WIDTH))
#define SCREEN_WIDTH  ((SCREEN_ITEM_WIDTH) + (SCREEN_VALUE_WIDTH))

#define SCREEN_RIGHT  ((SCREEN_LEFT) + (SCREEN_LIST_WIDTH - 1) + (SCREEN_VALUE_WIDTH - 1))
#define SCREEN_DOWN   ((SCREEN_TOP) + (SCREEN_HEIGHT - 1))

extern menu_node menu_data[];


/*
menu_node menu_data[] = {
							MENU_NODE_INIT_DATA(1,  0, 0, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a1"),
							MENU_NODE_INIT_DATA(2,  0, 1, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a2"),
							MENU_NODE_INIT_DATA(3,  0, 2, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a3"),
							MENU_NODE_INIT_DATA(4,  0, 3, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a4"),
							MENU_NODE_INIT_DATA(5,  1, 0, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a1b1"),
							MENU_NODE_INIT_DATA(6,  1, 1, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a1b2"),
							MENU_NODE_INIT_DATA(7,  1, 2, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a1b3"),
							MENU_NODE_INIT_DATA(8,  2, 0, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a2b1"),
							MENU_NODE_INIT_DATA(9,  2, 1, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a2b2"),
							MENU_NODE_INIT_DATA(10, 2, 2, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a2b3"),
							MENU_NODE_INIT_DATA(11, 2, 3, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a2b4"),
							MENU_NODE_INIT_DATA(12, 3, 0, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a3b1"),
							MENU_NODE_INIT_DATA(13, 3, 1, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a3b2"),
							MENU_NODE_INIT_DATA(14, 4, 0, MENU_ACT_NULL, 0, MENU_TYPE_NULL, "a4b1"),

							MENU_NODE_INIT_DATA(-1, 0, 0, NULL, 0, MENU_TYPE_NULL, NULL)  //End of the data.
						};
*/

#define M_NODE(x)            menu_data[x]
#define M_NODE_ID(x)         M_NODE(x).id
#define M_NODE_FATHER_ID(x)  M_NODE(x).father_id
#define M_NODE_RANK(x)       M_NODE(x).rank
#define M_NODE_UP(x)         M_NODE(x).node_up
#define M_NODE_DOWN(x)       M_NODE(x).node_down
#define M_NODE_FORWARD(x)    M_NODE(x).node_forward
#define M_NODE_BACK(x)       M_NODE(x).node_back

menu_inf menu_inf_data;

uint8_t menu_node_init(void)
{
	uint8_t error_code;
	uint8_t i;  //当前初始化的id
	uint8_t s_i;  //正在遍历的id

	error_code = 0;

	i = 0;
	while(M_NODE_ID(i) != MENU_ID_END)  //没有结束
	{
		M_NODE_UP(i) = MENU_NODE_ERR;
		M_NODE_DOWN(i) = MENU_NODE_ERR;
		M_NODE_FORWARD(i) = MENU_NODE_ERR;
		M_NODE_BACK(i) = MENU_NODE_ERR;
		i++;
	}

	i = 0;
	while(M_NODE_ID(i) != MENU_ID_END)  //没有结束
	{
		s_i = 0;
		while(M_NODE_ID(s_i) != MENU_ID_END)
		{
			if(M_NODE_FORWARD(i) == MENU_NODE_ERR)  //下一层未找到
			{
				if(M_NODE_ID(i) == M_NODE_FATHER_ID(s_i) && M_NODE_RANK(s_i) == 0)
				{
					M_NODE_FORWARD(i) = &M_NODE(s_i);
					M_NODE_BACK(s_i) = &M_NODE(i);
				}
			}
			if(M_NODE_BACK(i) == MENU_NODE_ERR)  //上一层未找到
			{
				if(M_NODE_FATHER_ID(i) == 0)  //第一层
				{
					M_NODE_BACK(i) = MENU_NODE_NULL;
				}
				else if(M_NODE_FATHER_ID(i) == M_NODE_ID(s_i))
				{
					M_NODE_BACK(i) = &M_NODE(s_i);
				}
			}
			if(M_NODE_UP(i) == MENU_NODE_ERR)  //上一个未找到
			{
				if(M_NODE_RANK(i) == 0)  //菜单的第一项
				{
					M_NODE_UP(i) = MENU_NODE_NULL;
				}
				else if(M_NODE_FATHER_ID(i) == M_NODE_FATHER_ID(s_i) && M_NODE_RANK(i) == (M_NODE_RANK(s_i) + 1))
				{
					M_NODE_UP(i) = &M_NODE(s_i);
					M_NODE_DOWN(s_i) = &M_NODE(i);
				}
			}
			if(M_NODE_DOWN(i) == MENU_NODE_ERR)  //下一个未找到
			{
				if(M_NODE_FATHER_ID(i) == M_NODE_FATHER_ID(s_i) && (M_NODE_RANK(i) + 1) == M_NODE_RANK(s_i))
				{
					M_NODE_DOWN(i) = &M_NODE(s_i);
					M_NODE_UP(s_i) = &M_NODE(i);
				}
			}

			//所有项目均寻找完毕
			if(M_NODE_UP(i) != MENU_NODE_ERR && M_NODE_DOWN(i) != MENU_NODE_ERR && M_NODE_FORWARD(i) != MENU_NODE_ERR && M_NODE_BACK(i) != MENU_NODE_ERR)
			{
				break;
			}
			s_i++;
		}
		//补充结束节点指针
		if(M_NODE_DOWN(i) == MENU_NODE_ERR) M_NODE_DOWN(i) = MENU_NODE_NULL;
		if(M_NODE_FORWARD(i) == MENU_NODE_ERR) M_NODE_FORWARD(i) = MENU_NODE_NULL;

		//判断是否出错
		if(M_NODE_UP(i) == MENU_NODE_ERR || M_NODE_BACK(i) == MENU_NODE_ERR)
		{
			error_code = 1;
		}
		i++;
	}
	return error_code;
}

uint8_t menu_init(void)
{
	uint8_t error_code;
	//变量初始化
	menu_inf_data.node_top = &M_NODE(0);
	menu_inf_data.node_select = &M_NODE(0);
	menu_inf_data.node_screen = &M_NODE(0);
	//菜单数据初始化
	error_code = menu_node_init();
	
	menu_inf_data.val_or_act_select = 0;
	return error_code;
}

void menu_refresh_item(void)
{
	menu_node* p_menu_node_tmp;
	uint8_t pos_tmp[2];
	//依次显示行
	pos_tmp[0] = SCREEN_TOP;
	pos_tmp[1] = SCREEN_LEFT;
	p_menu_node_tmp = menu_inf_data.node_screen;
	while(pos_tmp[0] <= SCREEN_DOWN)
	{
		if(p_menu_node_tmp != MENU_NODE_NULL)
		{
			//显示item
			menu_put_str(pos_tmp, SCREEN_ITEM_WIDTH, p_menu_node_tmp -> text);
			p_menu_node_tmp = p_menu_node_tmp -> node_down;
		}
		else
		{
			menu_put_str(pos_tmp, SCREEN_WIDTH, "");
			//menu_put_str(pos_tmp, SCREEN_ITEM_WIDTH, "");
		}
		pos_tmp[0]++;
	}
}

void menu_refresh_value(void)
{
	menu_node* p_menu_node_tmp;
	uint8_t pos_tmp[2];
	//依次显示行
	pos_tmp[0] = SCREEN_TOP;
	pos_tmp[1] = SCREEN_VALUE_LEFT;
	p_menu_node_tmp = menu_inf_data.node_screen;
	while(pos_tmp[0] <= SCREEN_DOWN)
	{
		if(p_menu_node_tmp != MENU_NODE_NULL)
		{
			//显示value
			if(p_menu_node_tmp -> value_type != TYPE_NULL && p_menu_node_tmp -> value != MENU_VALUE_NULL)
			{
				if(p_menu_node_tmp -> value_type == TYPE_BOOL)
				{
					menu_put_str(pos_tmp, SCREEN_VALUE_WIDTH, (const uint8_t *)(*((uint8_t *)(p_menu_node_tmp -> value)) == 0 ? "false" : "true"));
				}
				else
				{
					menu_put_num(pos_tmp, SCREEN_VALUE_WIDTH, p_menu_node_tmp -> value, p_menu_node_tmp -> value_type);
				}
			}
			else
			{
				menu_put_str(pos_tmp, SCREEN_VALUE_WIDTH, "");
			}
			p_menu_node_tmp = p_menu_node_tmp -> node_down;
		}
		else
		{
			//menu_put_str(pos_tmp, SCREEN_VALUE_WIDTH, "");
		}
		pos_tmp[0]++;
	}
}

void menu_refresh_white(void)
{
	menu_node* p_menu_node_tmp;
	uint8_t pos_tmp[2];
	//清除反白
	pos_tmp[0] = SCREEN_TOP;
	pos_tmp[1] = SCREEN_LEFT;
	while(pos_tmp[0] <= SCREEN_DOWN)
	{
		menu_clear_white(pos_tmp, SCREEN_WIDTH);
		pos_tmp[0]++;
	}
	
	//设置反白
	pos_tmp[0] = SCREEN_TOP;
	p_menu_node_tmp = menu_inf_data.node_screen;
	while(pos_tmp[0] <= SCREEN_DOWN)
	{
		if(p_menu_node_tmp != MENU_NODE_NULL)
		{
			if(p_menu_node_tmp == menu_inf_data.node_select)
			{
				if(menu_inf_data.val_or_act_select == 0)
				{
					pos_tmp[1] = SCREEN_LEFT;
					menu_set_white(pos_tmp, SCREEN_ITEM_WIDTH);
				}
				else
				{
					pos_tmp[1] = SCREEN_VALUE_LEFT;
					menu_set_white(pos_tmp, SCREEN_VALUE_WIDTH);
				}
			}
			p_menu_node_tmp = p_menu_node_tmp -> node_down;
		}
		pos_tmp[0]++;
	}
}

void menu_refresh(void)
{
	menu_refresh_item();
	//menu_refresh_value();
	menu_refresh_white();
}

__weak void menu_press_enter_callback(menu_inf* menu)
{
}

__weak void menu_press_back_callback(menu_inf* menu)
{
}

void menu_press_enter(void)
{
	if(menu_inf_data.node_select -> node_forward != MENU_NODE_NULL)
	{
		menu_inf_data.node_top = menu_inf_data.node_select -> node_forward;
		menu_inf_data.node_select = menu_inf_data.node_top;
		menu_inf_data.node_screen = menu_inf_data.node_top;
	}
	else
	{
		
// 		if((menu_inf_data.node_select -> action == MENU_ACT_RW) ||
// 					(menu_inf_data.node_select -> action == MENU_ACT_RO)
// 			)
// 		//只要value_type不为空就可进入value_select模式
// 		if(menu_inf_data.node_select -> value_type != TYPE_NULL)

		//只要action不为空
		if(menu_inf_data.node_select -> action != MENU_ACT_NULL)
		{
			menu_inf_data.val_or_act_select = 1;
		}
	}
	menu_refresh();
	menu_press_enter_callback(&menu_inf_data);
}

void menu_press_back(void)
{
	uint8_t i;
	if(menu_inf_data.val_or_act_select == 1)
	{
		menu_inf_data.val_or_act_select = 0;
	}
	else
	{
		if(menu_inf_data.node_select -> node_back != MENU_NODE_NULL)
		{
			menu_inf_data.node_select = menu_inf_data.node_top -> node_back;
			menu_inf_data.node_top = menu_inf_data.node_select;
			while(menu_inf_data.node_top -> node_up != MENU_NODE_NULL)
				menu_inf_data.node_top = menu_inf_data.node_top -> node_up;
		}
		if(menu_inf_data.node_select -> rank <= SCREEN_HEIGHT - 1)
		{
			menu_inf_data.node_screen = menu_inf_data.node_top;
		}
		else
		{
			menu_inf_data.node_screen = menu_inf_data.node_select;
			for(i = 0; i < SCREEN_HEIGHT - 1; i++)
			{
				menu_inf_data.node_screen = menu_inf_data.node_screen -> node_up;
			}
		}
	}
	menu_refresh();
	menu_press_back_callback(&menu_inf_data);
}

void menu_press_up(void)
{
	if(menu_inf_data.val_or_act_select == 0)
	{
		if(menu_inf_data.node_select -> node_up != MENU_NODE_NULL)
		{
			menu_inf_data.node_select = menu_inf_data.node_select -> node_up;
		}
		if(menu_inf_data.node_select -> rank < menu_inf_data.node_screen -> rank)
		{
			menu_inf_data.node_screen = menu_inf_data.node_select;
		}
	}
	menu_refresh();
}

void menu_press_down(void)
{
	uint8_t i;
	if(menu_inf_data.val_or_act_select == 0)
	{
		if(menu_inf_data.node_select -> node_down != MENU_NODE_NULL)
		{
			menu_inf_data.node_select = menu_inf_data.node_select -> node_down;
		}
		if(menu_inf_data.node_select -> rank > menu_inf_data.node_screen -> rank + SCREEN_HEIGHT - 1)
		{
			menu_inf_data.node_screen = menu_inf_data.node_select;
			for(i = 0; i < SCREEN_HEIGHT - 1; i++)
			{
				menu_inf_data.node_screen = menu_inf_data.node_screen -> node_up;
			}
		}
	}
	menu_refresh();
}
