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
// uint8 key4_status = 1;  //key4 检测双击 长按 单击 这里不用再写代码
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

int get_key_value(void) // 检测使用按键值 5-切换编辑状态 1-增加 2-减少
{
	int res;
	if (5 == key_value) // 单击key4 或者双击key4
	{
		is_editing_flag = !is_editing_flag;
	}
	else if ((0 < key_value) && (key_value < 3)) // 单击key1 key2
	{
		if (!is_editing_flag)
		{
			key_value = key_value + 2;
		}
	}
	else if (3 == key_value) // 单击key3 屏蔽
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
 *                             key4 长按、单击、双击定义
 * 参考：https://blog.csdn.net/qq_44078824/article/details/123757354
 * 长按事件：任何大于 KEY_LONG_PRESS_TIME
 * 单击事件：按下时间不超过 KEY_LONG_PRESS_TIME 且 释放后 KEY_WAIT_DOUBLE_TIME 内无再次按下的操作
 * 双击事件：俩次短按时间间隔小于KEY_WAIT_DOUBLE_TIME，俩次短按操作合并为一次双击事件。
 * 特殊说明：
 *          1.短按和长按时间间隔小于 KEY_WAIT_DOUBLE_TIME，响应一次单击和长按事件，不响应双击事件
 *          2.连续2n次短按，且时间间隔小于 KEY_WAIT_DOUBLE_TIME，响应为n次双击
 *          3.连续2n+1次短按，且时间间隔小于 KEY_WAIT_DOUBLE_TIME，且最后一次KEY_WAIT_DOUBLE_TIME内无操作，
 *				响应为n次双击 和 一次单击事件
 ****************************************************************************************************/
#define KEY_LONG_PRESS_TIME 50	// 20ms*50 = 1s
#define KEY_WAIT_DOUBLE_TIME 25 // 20ms*25 = 500ms

#define KEY_PRESSED_LEVEL 0 // 按键按下是电平为低
/****************************************************************************************************
 *                             局部函数定义
 ****************************************************************************************************/
static KEY_PinLevel_TypeDef KEY_ReadPin(void);	// 按键读取按键的电平函数
static void KEY_GetAction_PressOrRelease(void); // 获取按键是按下还是释放，保存到结构体

/****************************************************************************************************
 *                             全局变量
 ****************************************************************************************************/
KEY_Configure_TypeDef KeyCfg = {
	0,					// 按键长按计数
	KEY_Action_Release, // 虚拟当前IO电平，按下1，抬起0
	KEY_Status_Idle,	// 按键状态
	KEY_Event_Null,		// 按键事件
	KEY_ReadPin			// 读IO电平函数
};
/****************************************************************************************************
 *                             函数定义
 ****************************************************************************************************/
// 按键读取按键的电平函数
static KEY_PinLevel_TypeDef KEY_ReadPin(void) // 按键读取函数
{
	return (KEY_PinLevel_TypeDef)KEY4_PIN;
}

// 获取按键是按下还是释放，保存到结构体
static void KEY_GetAction_PressOrRelease(void) // 根据实际按下按钮的电平去把它换算成虚拟的结果
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

// 按键初始化函数
void KEY_Init(void) // IO初始化
{
}

/****************************************************************************************************
 *                             读取按键状态机
 ****************************************************************************************************/
void KEY_ReadStateMachine(void)
{
	KEY_GetAction_PressOrRelease();

	switch (KeyCfg.KEY_Status)
	{
	// 状态：没有按键按下
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

	// 状态：消抖
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

	// 状态：继续按下
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
			KeyCfg.KEY_Status = KEY_Status_WaiteAgain; // 按短了后释放
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		break;

	// 状态：一直长按着
	case KEY_Status_ConfirmPressLong:
		if (KeyCfg.KEY_Action == KEY_Action_Press)
		{ // 一直等待其放开
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

	// 状态：等待是否再次按下
	case KEY_Status_WaiteAgain:
		if ((KeyCfg.KEY_Action != KEY_Action_Press) && (KeyCfg.KEY_Count >= KEY_WAIT_DOUBLE_TIME))
		{ // 第一次短按,且释放时间大于KEY_WAIT_DOUBLE_TIME
			KeyCfg.KEY_Count = 0;
			KeyCfg.KEY_Status = KEY_Status_Idle;
			KeyCfg.KEY_Event = KEY_Event_SingleClick; // 响应单击
		}
		else if ((KeyCfg.KEY_Action != KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_WAIT_DOUBLE_TIME))
		{ // 第一次短按,且释放时间还没到KEY_WAIT_DOUBLE_TIME
			KeyCfg.KEY_Count++;
			KeyCfg.KEY_Status = KEY_Status_WaiteAgain; // 继续等待
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		else // 第一次短按,且还没到KEY_WAIT_DOUBLE_TIME 第二次被按下
		{
			KeyCfg.KEY_Count = 0;
			KeyCfg.KEY_Status = KEY_Status_SecondPress; // 第二次按下
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		break;
	case KEY_Status_SecondPress:
		if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count >= KEY_LONG_PRESS_TIME))
		{
			KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong; // 第二次按的时间大于 KEY_LONG_PRESS_TIME
			KeyCfg.KEY_Event = KEY_Event_SingleClick;		 // 先响应单击
			KeyCfg.KEY_Count = 0;
		}
		else if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_LONG_PRESS_TIME))
		{
			KeyCfg.KEY_Count++;
			KeyCfg.KEY_Status = KEY_Status_SecondPress;
			KeyCfg.KEY_Event = KEY_Event_Null;
		}
		else
		{ // 第二次按下后在 KEY_LONG_PRESS_TIME内释放
			KeyCfg.KEY_Count = 0;
			KeyCfg.KEY_Status = KEY_Status_Idle;
			KeyCfg.KEY_Event = KEY_Event_DoubleClick; // 响应双击
		}
		break;
	default:
		break;
	}
}

// 按键定时中断服务 20ms产生一个中断
void key_timer_Handler(void)
{
	KEY_ReadStateMachine(); // 调用状态机

	if (KeyCfg.KEY_Event == KEY_Event_SingleClick)
	{
		key_value = 5;
	}
	else if (KeyCfg.KEY_Event == KEY_Event_LongPress)
	{
		motor_start_flag = 1; // 长按后启动电机
		key_value = 9;
	}
	else if (KeyCfg.KEY_Event == KEY_Event_DoubleClick)
	{
		key_value = 5;
	}
}