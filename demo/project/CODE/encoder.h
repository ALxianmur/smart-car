/*
 * encoder.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef _ENCODER_H
#define _ENCODER_H
#include "headfile.h"

#define ENCODER_SPEED_K 8.6612060365841234f
#define ENCODER_DISTANCE_K 0.086612060365841234f

void encoder_init(void);
void update_speed_and_distance(void);

int get_right_encoder(void);
int get_left_encoder(void);
float get_left_speed(void);
float get_right_speed(void);
float get_distance(void);
float get_speed(void);

#endif
