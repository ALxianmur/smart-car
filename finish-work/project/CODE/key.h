/*
 * key.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef _KEY_H
#define _KEY_H

#include "headfile.h"

typedef enum KEY
{
    KEY_NC = 0,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
} Key_State;

void key_Handle();
int get_sw_value(void);
int get_key_value(void);
int get_hall_value(void);

#endif
