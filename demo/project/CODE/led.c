/*
 * led.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "led.h"

#define LED_PIN    	P52

void led_init(void)
{
    LED_PIN = 1;
}

void led_set( uint8 dat)
{
    LED_PIN = dat;
}

void led_toggle(void)
{
    LED_PIN = !P52;
}



