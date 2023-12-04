/*
 * steer.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */
#ifndef _STEER_H
#define _STEER_H
#include "headfile.h"

#define STEER_MIDDLE 1.5 * 10000 / 20 + 35
#define STEER_MIN (STEER_MIDDLE - 95) // ��ƫ
#define STEER_MAX (STEER_MIDDLE + 95) // ��ƫ

#define STEER_MAX_ANGLE 60.0  // ����       right
#define STEER_MIN_ANGLE -60.0 // ��С       left

#define STEER_DIR 0

void steer_init(void);
void steer_duty(uint32 duty);
void steer_angle(float angle); /*degree > 0  -->  turn right*/

#endif
