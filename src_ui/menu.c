/*
 * menu.c
 * �˵�
 * �����ߣ���è
 * ����ʱ�䣺2015.03.04
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.20
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
	uint8_t i;  //��ǰ��ʼ����id
	uint8_t s_i;  //���ڱ�����id

	error_code = 0;

	i = 0;
	while(M_NODE_ID(i) != MENU_ID_END)  //û�н���
	{
		M_NODE_UP(i) = MENU_NODE_ERR;
		M_NODE_DOWN(i) = MENU_NODE_ERR;
		M_NODE_FORWARD(i) = MENU_NODE_ERR;
		M_NODE_BACK(i) = MENU_NODE_ERR;
		i++;
	}

	i = 0;
	while(M_NODE_ID(i) != MENU_ID_END)  //û�н���
	{
		s_i = 0;
		while(M_NODE_ID(s_i) != MENU_ID_END)
		{
			if(M_NODE_FORWARD(i) == MENU_NODE_ERR)  //��һ��δ�ҵ�
			{
				if(M_NODE_ID(i) == M_NODE_FATHER_ID(s_i) && M_NODE_RANK(s_i) == 0)
				{
					M_NODE_FORWARD(i) = &M_NODE(s_i);
					M_NODE_BACK(s_i) = &M_NODE(i);
				}
			}
			if(M_NODE_BACK(i) == MENU_NODE_ERR)  //��һ��δ�ҵ�
			{
				if(M_NODE_FATHER_ID(i) == 0)  //��һ��
				{
					M_NODE_BACK(i) = MENU_NODE_NULL;
				}
				else if(M_NODE_FATHER_ID(i) == M_NODE_ID(s_i))
				{
					M_NODE_BACK(i) = &M_NODE(s_i);
				}
			}
			if(M_NODE_UP(i) == MENU_NODE_ERR)  //��һ��δ�ҵ�
			{
				if(M_NODE_RANK(i) == 0)  //�˵��ĵ�һ��
				{
					M_NODE_UP(i) = MENU_NODE_NULL;
				}
				else if(M_NODE_FATHER_ID(i) == M_NODE_FATHER_ID(s_i) && M_NODE_RANK(i) == (M_NODE_RANK(s_i) + 1))
				{
					M_NODE_UP(i) = &M_NODE(s_i);
					M_NODE_DOWN(s_i) = &M_NODE(i);
				}
			}
			if(M_NODE_DOWN(i) == MENU_NODE_ERR)  //��һ��δ�ҵ�
			{
				if(M_NODE_FATHER_ID(i) == M_NODE_FATHER_ID(s_i) && (M_NODE_RANK(i) + 1) == M_NODE_RANK(s_i))
				{
					M_NODE_DOWN(i) = &M_NODE(s_i);
					M_NODE_UP(s_i) = &M_NODE(i);
				}
			}

			//������Ŀ��Ѱ�����
			if(M_NODE_UP(i) != MENU_NODE_ERR && M_NODE_DOWN(i) != MENU_NODE_ERR && M_NODE_FORWARD(i) != MENU_NODE_ERR && M_NODE_BACK(i) != MENU_NODE_ERR)
			{
				break;
			}
			s_i++;
		}
		//��������ڵ�ָ��
		if(M_NODE_DOWN(i) == MENU_NODE_ERR) M_NODE_DOWN(i) = MENU_NODE_NULL;
		if(M_NODE_FORWARD(i) == MENU_NODE_ERR) M_NODE_FORWARD(i) = MENU_NODE_NULL;

		//�ж��Ƿ����
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
	//������ʼ��
	menu_inf_data.node_top = &M_NODE(0);
	menu_inf_data.node_select = &M_NODE(0);
	menu_inf_data.node_screen = &M_NODE(0);
	//�˵����ݳ�ʼ��
	error_code = menu_node_init();
	
	menu_inf_data.val_or_act_select = 0;
	return error_code;
}

void menu_refresh_item(void)
{
	menu_node* p_menu_node_tmp;
	uint8_t pos_tmp[2];
	//������ʾ��
	pos_tmp[0] = SCREEN_TOP;
	pos_tmp[1] = SCREEN_LEFT;
	p_menu_node_tmp = menu_inf_data.node_screen;
	while(pos_tmp[0] <= SCREEN_DOWN)
	{
		if(p_menu_node_tmp != MENU_NODE_NULL)
		{
			//��ʾitem
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
	//������ʾ��
	pos_tmp[0] = SCREEN_TOP;
	pos_tmp[1] = SCREEN_VALUE_LEFT;
	p_menu_node_tmp = menu_inf_data.node_screen;
	while(pos_tmp[0] <= SCREEN_DOWN)
	{
		if(p_menu_node_tmp != MENU_NODE_NULL)
		{
			//��ʾvalue
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
	//�������
	pos_tmp[0] = SCREEN_TOP;
	pos_tmp[1] = SCREEN_LEFT;
	while(pos_tmp[0] <= SCREEN_DOWN)
	{
		menu_clear_white(pos_tmp, SCREEN_WIDTH);
		pos_tmp[0]++;
	}
	
	//���÷���
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
// 		//ֻҪvalue_type��Ϊ�վͿɽ���value_selectģʽ
// 		if(menu_inf_data.node_select -> value_type != TYPE_NULL)

		//ֻҪaction��Ϊ��
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
