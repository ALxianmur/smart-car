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
uint8 key4_status = 1;
Key_State key_value;

uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

void key_Handle()
{
    hall_value = HALL_PIN;
    sw1_status = !SW1_PIN;
    sw2_status = !SW2_PIN;
    sw_value = (sw2_status << 1) | sw1_status;

    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;

    key1_status = KEY1_PIN;
    key2_status = KEY2_PIN;
    key3_status = KEY3_PIN;
    key4_status = KEY4_PIN;

    if (key1_status && !key1_last_status)
        key1_flag = 1;
    if (key2_status && !key2_last_status)
        key2_flag = 1;
    if (key3_status && !key3_last_status)
        key3_flag = 1;
    if (key4_status && !key4_last_status)
        key4_flag = 1;

    if (key1_flag)
    {
        key_value = KEY_LEFT;
        key1_flag = 0;
    }

    if (key2_flag)
    {
        key_value = KEY_RIGHT;
        key2_flag = 0;
    }

    if (key3_flag)
    {
        key_value = KEY_UP;
        key3_flag = 0;
    }

    if (key4_flag)
    {
        key_value = KEY_DOWN;
        key4_flag = 0;
    }
}

int get_key_value(void)
{
    int res;
    res = key_value;
    key_value = KEY_NC;
    return res;
}

int get_sw_value(void)
{
    return sw_value;
}

int get_hall_value(void)
{
    return hall_value;
}
