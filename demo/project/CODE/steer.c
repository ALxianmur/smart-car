/*
 * steer.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "steer.h"

#define STEER           PWMB_CH1_P74     //定义舵机引脚



void steer_init(void)
{
    pwm_init(STEER, 50, STEER_MIDDLE);
}

void steer_duty(uint32 duty)
{
    if(duty > STEER_MAX)   duty = STEER_MAX;
    if(duty < STEER_MIN)   duty = STEER_MIN;
    pwm_duty(STEER, duty);
}
