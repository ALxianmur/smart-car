/*
 * buzzer.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "beep.h"

static struct beep_struct beep_data = {0};
static uint32 ms_time_counter = 0;
static int beep_state = BEEP_NONE;

#define BEEP P77

void beep_init(void)
{
    gpio_mode(BEEP, GPO_PP);
    BEEP = 0;
}

void beep_on(void)
{
    BEEP = 1;
}

void beep_off(void)
{
    BEEP = 0;
}

/***************************************************************************************************
 * @fn      beep
 *
 * @brief   蜂鸣器闪鸣，在1ms中断中周期调用 beep_handle
 *
 * @param   nums        - 闪鸣次数
 *          period      - 蜂鸣器闪鸣的周期，以毫秒为单位，[10-100000]
 *          percent     - 蜂鸣器闪鸣的脉冲宽度百分比，[1-100]
 *
 * @return  None
 ***************************************************************************************************/
void beep(uint32 nums, uint32 period, uint32 percent)
{
    if (nums)
    {
        beep_data.nums = nums;
        beep_data.pulse = period * percent / 100;
        beep_data.npulse = period - beep_data.pulse;
        beep_state = BEEP_START;
    }
}

void beep_handle(void)
{
    switch (beep_state)
    {
    case BEEP_NONE:
        break;
    case BEEP_START:
        if (beep_data.nums--)
        {
            ms_time_counter = beep_data.pulse;
            beep_state = BEEP_ON;
        }
        else
        {
            beep_state = BEEP_END;
            ms_time_counter = 0;
        }
        break;
    case BEEP_ON:
        if (ms_time_counter--)
            beep_on();
        else
        {
            beep_state = BEEP_OFF;
            ms_time_counter = beep_data.npulse;
        }
        break;
    case BEEP_OFF:
        if (ms_time_counter--)
            beep_off();
        else
        {
            beep_state = BEEP_START;
        }
        break;
    case BEEP_END:
        beep_state = BEEP_NONE;
        break;
    default:
        break;
    }
}
