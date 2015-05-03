/*
 * menu.h
 * �˵�
 * �����ߣ���è
 * ����ʱ�䣺2015.03.04
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.20
 */

#ifndef MENU_H_
#define MENU_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#define MENU_ID_TYPE uint16_t
#define MENU_ID_END ((MENU_ID_TYPE)(-1))

#define MENU_ACT_NULL 0  //����ζ�������Ĳ˵�ѡ���޲˵����ݻ�˵�����
#define MENU_ACT_RO   (0x0100 | FILE_RO)
#define MENU_ACT_WO   (0x0100 | FILE_WO)
#define MENU_ACT_RW   (0x0100 | FILE_RW)
#define MENU_ACT_CRAFT_SEND_CMD       (0x0200 | 0x0010)
#define MENU_ACT_CRAFT_SEND_CMD_START (0x0200 | 0x0011)
#define MENU_ACT_CRAFT_SEND_CMD_END   (0x0200 | 0x0012)
#define MENU_ACT_CRAFT_SEND_SET_FILED (0x0200 | 0x0013)
#define MENU_ACT_CRAFT_RC             (0x0200 | 0x0020)
#define MENU_ACT_CRAFT_SEND_PID       (0x0200 | 0x0030)
	
#define MENU_NODE_NULL  NULL
#define MENU_NODE_ERR  ((menu_node *)0x01)

#define MENU_VALUE_NULL NULL

typedef struct _menu_node
{
	MENU_ID_TYPE id;                        //Ψһ, ����Ϊ0, �ɼ��
	MENU_ID_TYPE father_id;                 //��id
	MENU_ID_TYPE rank;                      //˳��, ͬ��˵���0��n-1, Ψһ�Ҳ����
	struct _menu_node* node_up;       //ͬ����һ���˵���(��һ��, rank==0)
	struct _menu_node* node_down;     //ͬ����һ���˵���
	struct _menu_node* node_forward;  //�Ӳ˵���
	struct _menu_node* node_back;     //���˵���
	uint32_t action;
	void *value;
	uint8_t value_type;
	uint8_t *text;
}menu_node;

typedef struct _menu_inf
{
	menu_node* node_top;
	menu_node* node_select;
	menu_node* node_screen;
	uint8_t val_or_act_select;
}menu_inf;

#define MENU_NODE_INIT_DATA(id, father_id, rank, action, value, value_type, text) \
					{(id), (father_id), (rank), NULL, NULL, NULL, NULL, (action), (value), (value_type), (text)}

extern uint8_t menu_init(void);
extern void menu_refresh(void);
extern void menu_refresh_value(void);
__INLINE void menu_loop(void) { menu_refresh_value(); }


//extern menu_inf menu_inf_data;


extern void menu_press_enter(void);
extern void menu_press_back(void);
extern void menu_press_up(void);
extern void menu_press_down(void);
//extern uint8_t menu_set_select(uint8_t sel);

#ifdef __cplusplus
};
#endif

#endif
