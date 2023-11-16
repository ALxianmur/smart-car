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
 *  移植长按、单击、双击 模块
 *  长按、单击、双击定义
 * 长按事件：任何大于 KEY_LONG_PRESS_TIME
 * 单击事件：按下时间不超过 KEY_LONG_PRESS_TIME 且 释放后 KEY_WAIT_DOUBLE_TIME 内无再次按下的操作
 * 双击事件：俩次短按时间间隔小于KEY_WAIT_DOUBLE_TIME，俩次短按操作合并为一次双击事件。
 * 特殊说明：
 *          1.短按和长按时间间隔小于 KEY_WAIT_DOUBLE_TIME，响应一次单击和长按事件，不响应双击事件
 *          2.连续2n次短按，且时间间隔小于 KEY_WAIT_DOUBLE_TIME，响应为n次双击
 *          3.连续2n+1次短按，且时间间隔小于 KEY_WAIT_DOUBLE_TIME，且最后一次KEY_WAIT_DOUBLE_TIME内无操作，
 */

// 按键状态
typedef enum _KEY_StatusList_TypeDef
{
	KEY_Status_Idle = 0,		 // 空闲
	KEY_Status_Debounce,		 // 消抖
	KEY_Status_ConfirmPress,	 // 确认按下
	KEY_Status_ConfirmPressLong, // 确认长按着
	KEY_Status_WaiteAgain,		 // 等待再次按下
	KEY_Status_SecondPress,		 // 第二次按下
} KEY_StatusList_TypeDef;

// 按键事件
typedef enum _KEY_EventList_TypeDef
{
	KEY_Event_Null = 0x00,		  // 无事件
	KEY_Event_SingleClick = 0x01, // 单击
	KEY_Event_DoubleClick = 0x02, // 双击
	KEY_Event_LongPress = 0x04	  // 长按
} KEY_EventList_TypeDef;

// 按键动作，按下、释放
typedef enum
{
	KEY_Action_Press = 0,
	KEY_Action_Release
} KEY_Action_TypeDef;

// 按键引脚的电平
typedef enum
{
	KKEY_PinLevel_Low = 0,
	KEY_PinLevel_High
} KEY_PinLevel_TypeDef;

// 按键配置结构体
typedef struct _KEY_Configure_TypeDef
{
	uint16 KEY_Count;							   // 按键长按计数
	KEY_Action_TypeDef KEY_Action;				   // 按键动作，按下1，抬起0
	KEY_StatusList_TypeDef KEY_Status;			   // 按键状态
	KEY_EventList_TypeDef KEY_Event;			   // 按键事件
	KEY_PinLevel_TypeDef (*KEY_ReadPin_Fcn)(void); // 读IO电平函数
} KEY_Configure_TypeDef;

// 按键初始化函数
void KEY_Init(void);			 // IO初始化
void KEY_ReadStateMachine(void); // 读取按键状态机
void key_timer_Handler(void);

#endif
