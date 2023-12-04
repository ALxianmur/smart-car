/*
 * ui.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef __UI_H__
#define __UI_H__

#include "headfile.h"

typedef uint8 Index;

typedef struct MenuItems
{
    float step_level;
    float max_value;
    float min_value;
    uint8 *title;
} MenuItem;

extern MenuItem items[];

void display_Handle(void);

#endif
