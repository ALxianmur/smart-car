#ifndef __BSP_PID_H
#define __BSP_PID_H
#include "headfile.h"

typedef struct
{
    float target_val;          // Ŀ��ֵ
    float actual_val;          // ʵ��ֵ
    float err;                 // ���嵱ǰƫ��ֵ
    float err_next;            // ������һ��ƫ��ֵ
    float err_last;            // �������һ��ƫ��ֵ
    float Kp, Ki, Kd;          // ������������֡�΢��ϵ��
    float integral;            // �������ֵ
    float integral_up_limit;   // ����PID�޷�
    float integral_down_limit; // ����PID�޷�
} _pid;

extern _pid left_motor_pid, right_motor_pid;
extern _pid steer_pid;

extern _pid pid_speed_erect_angle;
extern _pid pid_angle;
extern _pid pid_erect_angle_speed;
extern _pid differential_pid;

void PID_param_init(void);
void set_pid_target(_pid *pid, float temp_val);
float get_pid_target(_pid *pid);
void set_p_i_d(_pid *pid, float p, float i, float d);
float positional_pid_realize(_pid *pid, float actual_val);
float erect_angle_positional_pid_realize(_pid *pid, float actual_val);
float incremental_pid_realize(_pid *pid, float actual_val);
void range_protect(void *value, float upper_limit, float lower_limit, uint8 type);

#endif
