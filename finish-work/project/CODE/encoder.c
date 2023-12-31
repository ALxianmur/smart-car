/*
 * encoder.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "encoder.h"

// 定义脉冲引脚
#define SPEEDL_PLUSE CTIM0_P34
#define SPEEDR_PLUSE CTIM3_P04
// 定义方向引脚
#define SPEEDL_DIR P35
#define SPEEDR_DIR P53

int16 left_encoder = 0;
int16 right_encoder = 0;

float left_speed;   // cm/s
float right_speed;  // cm/s
float distance = 0; // cm

void encoder_init(void)
{
    ctimer_count_init(SPEEDL_PLUSE);
    ctimer_count_init(SPEEDR_PLUSE);
}

void pit_handler(void)
{
    left_encoder = ctimer_count_read(SPEEDL_PLUSE);
    right_encoder = ctimer_count_read(SPEEDR_PLUSE);

    ctimer_count_clean(SPEEDL_PLUSE);
    ctimer_count_clean(SPEEDR_PLUSE);

    if (1 == SPEEDL_DIR)
    {
    }
    else
    {
        left_encoder = -left_encoder;
    }
    if (1 == SPEEDR_DIR)
    {
        right_encoder = -right_encoder;
    }
    else
    {
    }
}
// C D 车模数 0.4; 主动轮齿数 ： 16 ;  从动轮齿数 ：68  编码器齿数：30; 68/30 = 2.266667
// 车轮直径65mm ; 1周-(512*2.266667) = 1,160.53357脉冲  ;1个脉冲 65*pi/1,160.53357 mm = 0.17595661978688 mm; 车速最快3000mm /s ;
// n个脉冲/10ms  换算到 cm/s ,测量周期10ms : n*100*0.17595661978688f mm/s
void update_speed_and_distance(void)
{
    static float win_l[WIN_SIZE] = {0};
    static float win_r[WIN_SIZE] = {0};
    static float left_s_lpf = 0, right_s_lpf = 0;
    pit_handler();
    left_speed = LowPassFilter(win_l, left_encoder * ENCODER_SPEED_K);
    right_speed = LowPassFilter(win_r, right_encoder * ENCODER_SPEED_K);
    //		left_speed = left_encoder * ENCODER_SPEED_K;
    //    right_speed =  right_encoder * ENCODER_SPEED_K;
    distance += (left_encoder + right_encoder) / 2 * ENCODER_DISTANCE_K;
}

int get_right_encoder(void)
{
    return right_encoder;
}

int get_left_encoder(void)
{
    return left_encoder;
}

float get_left_speed(void)
{
    return left_speed;
}

float get_right_speed(void)
{
    return right_speed;
}

float get_distance(void)
{
    return distance / 10;
}

float clear_distance(void)
{
    distance = 0;
}

float get_speed(void)
{
    return (left_speed + right_speed) / 2;
}
