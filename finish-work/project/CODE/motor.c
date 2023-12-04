#include "motor.h"

int left_duty = 0;
int right_duty = 0;

#define MOTOR_DEAD_VAL 100 // the max of duty 10000

static uint8 dir = 0;
static int16 duty = 0;

#define DIR_1 P64
#define DIR_2 P60
#define PWM_1 PWMA_CH4P_P66
#define PWM_2 PWMA_CH2P_P62

#define HIP4082 // ���ʹ��DRV8701��ע�ʹ˾�

void motor_init(void)
{
#ifdef HIP4082
    /* HIP4082���� */
    pwm_init(PWMA_CH1P_P60, 17000, 0);
    pwm_init(PWMA_CH2P_P62, 17000, 0);

    pwm_init(PWMA_CH3P_P64, 17000, 0);
    pwm_init(PWMA_CH4P_P66, 17000, 0);
#else
    /* DRV8701 ���� */
    pwm_init(PWM_1, 17000, 0); // ��ʼ��PWM1  ʹ��P60����  ��ʼ��Ƶ��Ϊ17Khz
    pwm_init(PWM_2, 17000, 0); // ��ʼ��PWM2  ʹ��P62����  ��ʼ��Ƶ��Ϊ17Khz

    gpio_mode(P6_4, GPO_PP);
    gpio_mode(P6_0, GPO_PP);
#endif
}
void left_motor_duty(int duty)
{
    left_duty = duty;
    if (left_duty > 0)
        left_duty += MOTOR_DEAD_VAL + 50;
    else
        left_duty -= MOTOR_DEAD_VAL - 50;

    if (left_duty > PWM_MAX)
        left_duty = PWM_MAX;
    else if (left_duty < -PWM_MAX)
        left_duty = -PWM_MAX;

    if (left_duty >= 0) // ��ת
    {
#ifdef HIP4082
        /* HIP4082���� */
        pwm_duty(PWMA_CH3P_P64, 0);
        pwm_duty(PWMA_CH4P_P66, left_duty);
#else
        /*DRV8701����*/
        DIR_1 = 0;
        pwm_duty(PWM_1, left_duty);
#endif
    }
    else // ��ת
    {
#ifdef HIP4082
        /* HIP4082����*/
        pwm_duty(PWMA_CH3P_P64, -left_duty);
        pwm_duty(PWMA_CH4P_P66, 0);
#else
        /*DRV8701����*/
        DIR_1 = 1;
        pwm_duty(PWM_1, -left_duty);
#endif
    }
}

void right_motor_duty(int duty)
{
    right_duty = duty;
    if (right_duty > 0)
        right_duty += MOTOR_DEAD_VAL;
    else
        right_duty -= MOTOR_DEAD_VAL;

    if (right_duty > PWM_MAX)
        right_duty = PWM_MAX;
    else if (right_duty < -PWM_MAX)
        right_duty = -PWM_MAX;

    if (right_duty >= 0) // ��ת
    {
#ifdef HIP4082
        /* HIP4082���� */
        pwm_duty(PWMA_CH1P_P60, 0);
        pwm_duty(PWMA_CH2P_P62, right_duty);
#else
        /*DRV8701����*/
        DIR_2 = 0;
        pwm_duty(PWM_2, right_duty);
#endif
    }
    else // ��ת
    {
#ifdef HIP4082
        /* HIP4082���� */
        pwm_duty(PWMA_CH1P_P60, -right_duty);
        pwm_duty(PWMA_CH2P_P62, 0);
#else
        /*DRV8701����*/
        DIR_2 = 1;
        pwm_duty(PWM_2, -right_duty);
#endif
    }
}

void motor_duty(int l, int r)
{
    left_motor_duty(l);
    right_motor_duty(r);
}

int get_left_duty(void)
{
    return left_duty;
}
int get_right_duty(void)
{
    return right_duty;
}
