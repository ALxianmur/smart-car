/*
 * key.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef _KEY_H
#define _KEY_H

#include "headfile.h"

void key_Handle();
int get_sw_value(void);
int get_key_value(void);
int get_hall_value(void);
uint8 get_is_editing_flag(void);
void set_key_value(uint8 val);

/*
 *  ��ֲ������������˫�� ģ��
 *  ������������˫������
 * �����¼����κδ��� KEY_LONG_PRESS_TIME
 * �����¼�������ʱ�䲻���� KEY_LONG_PRESS_TIME �� �ͷź� KEY_WAIT_DOUBLE_TIME �����ٴΰ��µĲ���
 * ˫���¼������ζ̰�ʱ����С��KEY_WAIT_DOUBLE_TIME�����ζ̰������ϲ�Ϊһ��˫���¼���
 * ����˵����
 *          1.�̰��ͳ���ʱ����С�� KEY_WAIT_DOUBLE_TIME����Ӧһ�ε����ͳ����¼�������Ӧ˫���¼�
 *          2.����2n�ζ̰�����ʱ����С�� KEY_WAIT_DOUBLE_TIME����ӦΪn��˫��
 *          3.����2n+1�ζ̰�����ʱ����С�� KEY_WAIT_DOUBLE_TIME�������һ��KEY_WAIT_DOUBLE_TIME���޲�����
 */

// ����״̬
typedef enum _KEY_StatusList_TypeDef
{
	KEY_Status_Idle = 0,		 // ����
	KEY_Status_Debounce,		 // ����
	KEY_Status_ConfirmPress,	 // ȷ�ϰ���
	KEY_Status_ConfirmPressLong, // ȷ�ϳ�����
	KEY_Status_WaiteAgain,		 // �ȴ��ٴΰ���
	KEY_Status_SecondPress,		 // �ڶ��ΰ���
} KEY_StatusList_TypeDef;

// �����¼�
typedef enum _KEY_EventList_TypeDef
{
	KEY_Event_Null = 0x00,		  // ���¼�
	KEY_Event_SingleClick = 0x01, // ����
	KEY_Event_DoubleClick = 0x02, // ˫��
	KEY_Event_LongPress = 0x04	  // ����
} KEY_EventList_TypeDef;

// �������������¡��ͷ�
typedef enum
{
	KEY_Action_Press = 0,
	KEY_Action_Release
} KEY_Action_TypeDef;

// �������ŵĵ�ƽ
typedef enum
{
	KKEY_PinLevel_Low = 0,
	KEY_PinLevel_High
} KEY_PinLevel_TypeDef;

// �������ýṹ��
typedef struct _KEY_Configure_TypeDef
{
	uint16 KEY_Count;							   // ������������
	KEY_Action_TypeDef KEY_Action;				   // ��������������1��̧��0
	KEY_StatusList_TypeDef KEY_Status;			   // ����״̬
	KEY_EventList_TypeDef KEY_Event;			   // �����¼�
	KEY_PinLevel_TypeDef (*KEY_ReadPin_Fcn)(void); // ��IO��ƽ����
} KEY_Configure_TypeDef;

// ������ʼ������
void KEY_Init(void);			 // IO��ʼ��
void KEY_ReadStateMachine(void); // ��ȡ����״̬��
void key_timer_Handler(void);

#endif
