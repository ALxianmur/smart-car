/*
 * control.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef _CONTROL_H
#define _CONTROL_H
#include "headfile.h"

void steer_control(void);
void motor_control(void);
int get_left_u(void);
int get_right_u(void);

#endif
