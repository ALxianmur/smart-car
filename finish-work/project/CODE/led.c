/*
 * led.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "led.h"

void led_init(void)
{
    P52 = 1;
}

void led_set(uint8 dat)
{
    P52 = dat;
}

void led_toggle(void)
{
    P52 = !P52;
}
