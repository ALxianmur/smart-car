/*
 * modor.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef _MOTOR_H
#define _MOTOR_H
#include "headfile.h"

#define PWM_MAX 10000

extern int left_duty;
extern int right_duty;

void motor_init(void);
void right_motor_duty(int duty);
void left_motor_duty(int duty);
void motor_duty(int l, int r);

int get_left_duty(void);
int get_right_duty(void);

#endif
