/*
 * steer.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "steer.h"

#define STEER PWMB_CH1_P74 // ����������

void steer_init(void)
{
    pwm_init(STEER, 50, STEER_MIDDLE);
}

// �����Ӧ�� 0-180 ��Ƕȶ�Ӧ ��������� 1ms-2ms �ߵ�ƽ
// STEER_MAX/(1000/freq)*(1+Angle/180) �� 50hz ʱ���� STEER_MAX/(1000/50)*(1+Angle/180)
// ��ô 100hz �� 90�ȵĴ�� ���ߵ�ƽʱ��1.5ms ��������Ϊ
// PWM_DUTY_MAX/(1000/100)*(1+90/180) = PWM_DUTY_MAX/10*1.5
void steer_duty(uint32 duty)
{
    if (duty > STEER_MAX)
        duty = STEER_MAX;
    if (duty < STEER_MIN)
        duty = STEER_MIN;
    pwm_duty(STEER, duty);
}

void steer_angle(float angle) /*degree < 0  -->  turn right*/
{
    if (angle > STEER_MAX_ANGLE)
        angle = STEER_MAX_ANGLE;
    if (angle < STEER_MIN_ANGLE)
        angle = STEER_MIN_ANGLE;
    pwm_duty(STEER, (uint32)(STEER_MIDDLE + angle * (STEER_MAX - STEER_MIN) / (2 * STEER_MAX_ANGLE)));
}
