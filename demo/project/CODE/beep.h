/*
 * beep.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef CODE_BEEP_H_
#define CODE_BEEP_H_

#include "headfile.h"

enum Beep_State
{
    BEEP_NONE = 0,
    BEEP_START = 1,
    BEEP_ON = 2,
    BEEP_OFF = 3,
    BEEP_END = 4
};

struct beep_struct
{
    uint32 nums;
    uint32 pulse;
    uint32 npulse;
};

void beep_init(void);
void beep_handle(void);
void beep(uint32 nums, uint32 period, uint32 percent);

#endif /* CODE_BEEP_H_ */
