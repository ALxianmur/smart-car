/*
 * key.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "key.h"

#define HALL_PIN P26
#define KEY1_PIN P70
#define KEY2_PIN P71
#define KEY3_PIN P72
#define KEY4_PIN P73

#define SW1_PIN P75
#define SW2_PIN P76

uint8 sw1_status;
uint8 sw2_status;
uint8 sw_value;
uint8 hall_value;

uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
// uint8 key4_status = 1;  //key4 ���˫�� ���� ���� ���ﲻ����д����
uint8 key_value = 0;

uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
// uint8 key4_last_status;

uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
// uint8 key4_flag;
uint8 is_editing_flag = 0;

void key_Handle()
{
	hall_value = HALL_PIN;
	sw1_status = !SW1_PIN;
	sw2_status = !SW2_PIN;
	sw_value = (sw2_status << 1) | sw1_status;

	key1_last_status = key1_status;
	key2_last_status = key2_status;
	key3_last_status = key3_status;
	//    key4_last_status = key4_status;

	key1_status = KEY1_PIN;
	key2_status = KEY2_PIN;
	key3_status = KEY3_PIN;
	//    key4_status = KEY4_PIN;

	if (key1_status && !key1_last_status)
		key1_flag = 1;
	if (key2_status && !key2_last_status)
		key2_flag = 1;
	if (key3_status && !key3_last_status)
		key3_flag = 1;
	//    if(key4_status && !key4_last_status)    key4_flag = 1;

	if (key1_flag)
	{
		key_value = 1;
		key1_flag = 0;
	}

	if (key2_flag)
	{
		key_value = 2;
		key2_flag = 0;
	}

	if (key3_flag)
	{
		key_value = 3;
		key3_flag = 0;
	}

	/*
		if(key4_flag)
		{
			key_value = KEY_DOWN;
			key4_flag = 0;
		}
		*/
}

int get_key_value(void) // ���ʹ�ð���ֵ 5-�л��༭״̬ 1-���� 2-����
{
	int res;
	if (5 == key_value) // ����key4 ����˫��key4
	{
		is_editing_flag = !is_editing_flag;
	}
	else if ((0 < key_value) && (key_value < 3)) // ����key1 key2
	{
		if (!is_editing_flag)
		{
			key_value = key_value + 2;
		}
	}
	else if (3 == key_value) // ����key3 ����
	{
		key_value = 0;
		beep(1, 300, 50);
	}

	res = key_value;
	key_value = 0;
	return res;
}

void set_key_value(uint8 val)
{
	key_value = val;
}

uint8 get_is_editing_flag(void)
{
	return is_editing_flag;
}

int get_sw_value(void)
{
	return sw_value;
}

int get_hall_value(void)
{
	return hall_value;
}

/****************************************************************************************************
 *                             key4 ������������˫������
 * �ο���https://blog.csdn.net/qq_44078824/article/details/123757354
 * �����¼����κδ��� KEY_LONG_PRESS_TIME
 * �����¼�������ʱ�䲻���� KEY_LONG_PRESS_TIME �� �ͷź� KEY_WAIT_DOUBLE_TIME �����ٴΰ��µĲ���
 * ˫���¼������ζ̰�ʱ����С��KEY_WAIT_DOUBLE_TIME�����ζ̰������ϲ�Ϊһ��˫���¼���
 * ����˵����
 *          1.�̰��ͳ���ʱ����С�� KEY_WAIT_DOUBLE_TIME����Ӧһ�ε����ͳ����¼�������Ӧ˫���¼�
 *          2.����2n�ζ̰�����ʱ����С�� KEY_WAIT_DOUBLE_TIME����ӦΪn��˫��
 *          3.����2n+1�ζ̰�����ʱ����С�� KEY_WAIT_DOUBLE_TIME�������һ��KEY_WAIT_DOUBLE_TIME���޲�����
 *				��ӦΪn��˫�� �� һ�ε����¼�
 ****************************************************************************************************/
#define KEY_LONG_PRESS_TIME 50	// 20ms*50 = 1s
#define KEY_WAIT_DOUBLE_TIME 25 // 20ms*25 = 500ms

#define KEY_PRESSED_LEVEL 0 // ���������ǵ�ƽΪ��
/****************************************************************************************************
 *                             �ֲ���������
 ****************************************************************************************************/
static KEY_PinLevel_TypeDef KEY_ReadPin(void);	// ������ȡ�����ĵ�ƽ����
static void KEY_GetAction_PressOrRelease(void); // ��ȡ�����ǰ��»����ͷţ����浽�ṹ��

/****************************************************************************************************
 *                             ȫ�ֱ���
 ****************************************************************************************************/
KEY_Configure_TypeDef KeyCfg = {
	0,					// ������������
	KEY_Action_Release, // ���⵱ǰIO��ƽ������1��̧��0
	KEY_Status_Idle,	// ����״̬
	KEY_Event_Null,		// �����¼�
	KEY_ReadPin			// ��IO��ƽ����
};
/****************************************************************************************************
 *                             ��������
 ****************************************************************************************************/
// ������ȡ�����ĵ�ƽ����
static KEY_PinLevel_TypeDef KEY_ReadPin(void) // ������ȡ����
{
	return (KEY_PinLevel_TypeDef)KEY4_PIN;
}

// ��ȡ�����ǰ��»����ͷţ����浽�ṹ��
static void KEY_GetAction_PressOrRelease(void) // ����ʵ�ʰ��°�ť�ĵ�ƽȥ�������������Ľ��
{
	if (KeyCfg.KEY_ReadPin_Fcn() == KEY_PRESSED_LEVEL)
	{
		KeyCfg.KEY_Action = KEY_Action_Press;
	}
	else
	{
		KeyCfg.KEY_Action = KEY_Action_Release;
	}
}

// ������ʼ������
void KEY_Init(void) // IO��ʼ��
{
}

/****************************************************************************************************
 *                             ��ȡ����״̬��
 ****************************************************************************************************/
void KEY_ReadStateMachine(void)
{
	KEY_GetAction_PressOrRelease();

	switch (KeyCfg.KEY_Status)
	{
	// ״̬��û�а�������
	case KEY_Status_Idle:
		if (KeyCfg.KEY_Action == KEY_Action_Press)
		{
			KeyCfg.KEY_Status = KEY_Status_Debounce;
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		else
		{
			KeyCfg.KEY_Status = KEY_Status_Idle;
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		break;

	// ״̬������
	case KEY_Status_Debounce:
		if (KeyCfg.KEY_Action == KEY_Action_Press)
		{
			KeyCfg.KEY_Status = KEY_Status_ConfirmPress;
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		else
		{
			KeyCfg.KEY_Status = KEY_Status_Idle;
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		break;

	// ״̬����������
	case KEY_Status_ConfirmPress:
		if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count >= KEY_LONG_PRESS_TIME))
		{
			KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong;
			KeyCfg.KEY_Event = KEY_Event_Null;
			KeyCfg.KEY_Count = 0;
		}
		else if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_LONG_PRESS_TIME))
		{
			KeyCfg.KEY_Count++;
			KeyCfg.KEY_Status = KEY_Status_ConfirmPress;
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		else
		{
			KeyCfg.KEY_Count = 0;
			KeyCfg.KEY_Status = KEY_Status_WaiteAgain; // �����˺��ͷ�
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		break;

	// ״̬��һֱ������
	case KEY_Status_ConfirmPressLong:
		if (KeyCfg.KEY_Action == KEY_Action_Press)
		{ // һֱ�ȴ���ſ�
			KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong;
			KeyCfg.KEY_Event = KEY_Event_Null;
			KeyCfg.KEY_Count = 0;
		}
		else
		{
			KeyCfg.KEY_Status = KEY_Status_Idle;
			KeyCfg.KEY_Event = KEY_Event_LongPress;
			KeyCfg.KEY_Count = 0;
		}
		break;

	// ״̬���ȴ��Ƿ��ٴΰ���
	case KEY_Status_WaiteAgain:
		if ((KeyCfg.KEY_Action != KEY_Action_Press) && (KeyCfg.KEY_Count >= KEY_WAIT_DOUBLE_TIME))
		{ // ��һ�ζ̰�,���ͷ�ʱ�����KEY_WAIT_DOUBLE_TIME
			KeyCfg.KEY_Count = 0;
			KeyCfg.KEY_Status = KEY_Status_Idle;
			KeyCfg.KEY_Event = KEY_Event_SingleClick; // ��Ӧ����
		}
		else if ((KeyCfg.KEY_Action != KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_WAIT_DOUBLE_TIME))
		{ // ��һ�ζ̰�,���ͷ�ʱ�仹û��KEY_WAIT_DOUBLE_TIME
			KeyCfg.KEY_Count++;
			KeyCfg.KEY_Status = KEY_Status_WaiteAgain; // �����ȴ�
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		else // ��һ�ζ̰�,�һ�û��KEY_WAIT_DOUBLE_TIME �ڶ��α�����
		{
			KeyCfg.KEY_Count = 0;
			KeyCfg.KEY_Status = KEY_Status_SecondPress; // �ڶ��ΰ���
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		break;
	case KEY_Status_SecondPress:
		if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count >= KEY_LONG_PRESS_TIME))
		{
			KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong; // �ڶ��ΰ���ʱ����� KEY_LONG_PRESS_TIME
			KeyCfg.KEY_Event = KEY_Event_SingleClick;		 // ����Ӧ����
			KeyCfg.KEY_Count = 0;
		}
		else if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_LONG_PRESS_TIME))
		{
			KeyCfg.KEY_Count++;
			KeyCfg.KEY_Status = KEY_Status_SecondPress;
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		else
		{ // �ڶ��ΰ��º��� KEY_LONG_PRESS_TIME���ͷ�
			KeyCfg.KEY_Count = 0;
			KeyCfg.KEY_Status = KEY_Status_Idle;
			KeyCfg.KEY_Event = KEY_Event_DoubleClick; // ��Ӧ˫��
		}
		break;
	default:
		break;
	}
}

// ������ʱ�жϷ��� 20ms����һ���ж�
void key_timer_Handler(void)
{
	KEY_ReadStateMachine(); // ����״̬��

	if (KeyCfg.KEY_Event == KEY_Event_SingleClick)
	{
		key_value = 5;
	}
	else if (KeyCfg.KEY_Event == KEY_Event_LongPress)
	{
		motor_start_flag = 1; // �������������
		key_value = 9;
	}
	else if (KeyCfg.KEY_Event == KEY_Event_DoubleClick)
	{
		key_value = 5;
	}
}