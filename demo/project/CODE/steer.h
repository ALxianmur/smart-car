/*
 * steer.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */
#ifndef _STEER_H
#define _STEER_H
#include "headfile.h"

#define STEER_MIDDLE smartcar_param.steer_middle_value
#define STEER_MIN (STEER_MIDDLE-76)  // ��ƫ
#define STEER_MAX (STEER_MIDDLE+76)  // ��ƫ

#define STEER_MAX_ANGLE 40.0         //����       right
#define STEER_MIN_ANGLE -40.0        //��С       left

#define STEER_DIR 0

void steer_init(void);
void steer_duty(uint32 duty);

#endif
