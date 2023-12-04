/*
 * control.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "control.h"
#include "motor.h"

void steer_control(void)
{
    if (smartcar_status.steer_on)
        steer_angle(smartcar_status.goal_angle);
    else
        steer_angle(0);
}

void quick_slow(int *u, float s)
{
    if (s > 5)
    {
        *u = -PWM_MAX;
    }
    else
    {
        *u = 0;
    }
}
int left_motor_u = 0;
int right_motor_u = 0;

int left_positional_pid_realize(float actual_val)
{
    static int err, err_next, integral;
    int ret;
    err = smartcar_status.left_goal_speed - actual_val;
    integral += err;
    ret = left_motor_pid.Kp * err + left_motor_pid.Ki * integral +
          left_motor_pid.Kd * (err - err_next);
    err_next = err;
    return ret;
}
int right_positional_pid_realize(float actual_val)
{
    static int err, err_next, integral;
    int ret;
    err = smartcar_status.right_goal_speed - actual_val;
    integral += err;
    ret = right_motor_pid.Kp * err + right_motor_pid.Ki * integral +
          right_motor_pid.Kd * (err - err_next);
    err_next = err;
    return ret;
}

void motor_control(void)
{
    static int motor_delay_start = 0;
    float left_speed, right_speed;

    PID_param_init();

    if (motor_start_flag == 0)
    {
        motor_delay_start = 200;
        left_motor_duty(0);
        right_motor_duty(0);
        return;
    }
    if (motor_start_flag == 1)
    {
        motor_delay_start--;
        if (motor_delay_start > 0)
            return;
    }
    if (1 == smartcar_status.motor_close_circle)
    {
        set_pid_target(&left_motor_pid, smartcar_status.left_goal_speed);
        set_pid_target(&right_motor_pid, smartcar_status.right_goal_speed);
        left_speed = get_left_speed();
        right_speed = get_right_speed();

        // 增量式
        //		left_motor_u = (int)incremental_pid_realize(&left_motor_pid, get_left_speed());
        //		right_motor_u = (int)incremental_pid_realize(&right_motor_pid, get_right_speed());
        // 位置式
        //        left_motor_u = (int)(positional_pid_realize(&left_motor_pid, left_speed));
        //        right_motor_u = (int)(positional_pid_realize(&right_motor_pid, right_speed));
        left_motor_u = left_positional_pid_realize(left_speed);
        right_motor_u = right_positional_pid_realize(right_speed);

        if (left_motor_u > PWM_MAX)
            left_motor_u = PWM_MAX;
        else if (left_motor_u < -PWM_MAX)
            left_motor_u = -PWM_MAX;

        if (right_motor_u > PWM_MAX)
            right_motor_u = PWM_MAX;
        else if (right_motor_u < -PWM_MAX)
            right_motor_u = -PWM_MAX;
    }
    else
    {
        left_motor_u = smartcar_status.left_duty_u;
        right_motor_u = smartcar_status.right_duty_u;
    }

    if (smartcar_status.motor_on)
    {
        left_motor_duty(left_motor_u);
        right_motor_duty(right_motor_u);
    }
    else
    {
        left_motor_duty(0);
        right_motor_duty(0);
    }
}

int get_left_u(void)
{
    return left_motor_u;
}
int get_right_u(void)
{
    return right_motor_u;
}
