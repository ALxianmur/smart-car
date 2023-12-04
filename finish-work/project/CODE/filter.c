/*
 * filter.c
 *
 *  Created on: 2021��2��16��
 *      Author: sundm
 */

#include "headfile.h"
#include "filter.h"

const float filter_weight[WIN_SIZE] = {10, 15, 20, 25, 30, 40, 50, 60, 70, 80};

float SlidingFilter(float array[WIN_SIZE], float value) // �����˲�
{
    int i;
    float result, weight_sum, max, argmax, min, argmin;
    // ���ڻ���
    for (i = 0; i < WIN_SIZE - 1; i++)
    {
        array[i] = array[i + 1];
    }
    array[WIN_SIZE - 1] = value;

    // ȥ�����С
    max = -50000.0f;
    argmax = 0;
    min = 50000.0f;
    argmin = 0;
    for (i = 0; i < WIN_SIZE; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
            argmax = i;
        }
        if (array[i] < min)
        {
            min = array[i];
            argmin = i;
        }
    }

    // ��Ȩ
    result = 0.0f;
    weight_sum = 0.0f;
    for (i = 0; i < WIN_SIZE; i++)
    {
        /*if(i == argmin || i == argmax)
            continue;*/
        result += filter_weight[i] * array[i];
        weight_sum += filter_weight[i];
    }
    if (weight_sum != 0)
    {
        result = result / weight_sum;
    }
    return result;
}

const float filter_weight_2[WIN_SIZE] = {1, 2, 3, 4, 5, 10, 15, 60, 80, 100};

float LowPassFilter(float array[WIN_SIZE], float value) // ��ͨ�˲�
{
    int i;
    float result, weight_sum;
    // ���ڻ���
    for (i = 0; i < WIN_SIZE - 1; i++)
    {
        array[i] = array[i + 1];
    }
    array[WIN_SIZE - 1] = value;

    // ��Ȩ
    result = 0.0f;
    weight_sum = 0.0f;
    for (i = 0; i < WIN_SIZE; i++)
    {
        result += filter_weight_2[i] * array[i];
        weight_sum += filter_weight_2[i];
    }
    if (weight_sum != 0)
    {
        result = result / weight_sum;
    }
    return result;
}

float first_order_lpf(float *lpf, float val, float cut_off_freq, float sample_freq)
{
    *lpf += (1 / (1 + 1 / (2 * 3.1415 * cut_off_freq * (1 / sample_freq)))) * (val - *lpf);
    return *lpf;
}

/**************************************************************************/
float K1 = 0.02;
float angle_dot;
float angle;
float Q_angle = 0.001; // ����������Э����
float Q_gyro = 0.003;  // 0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
float R_angle = 0.5;   // ����������Э���� �Ȳ���ƫ��
float dt = 0.005;      //
char C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] = {0, 0, 0, 0};
float PP[2][2] = {{1, 0}, {0, 1}};

/**************************************************************************
�������ܣ����׿������˲�
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
float Kalman_Filter(float Accel, float Gyro)
{
    angle += (Gyro - Q_bias) * dt;           // �������
    Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

    Pdot[1] = -PP[1][1];
    Pdot[2] = -PP[1][1];
    Pdot[3] = Q_gyro;
    PP[0][0] += Pdot[0] * dt; // Pk-����������Э����΢�ֵĻ���
    PP[0][1] += Pdot[1] * dt; // =����������Э����
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;

    Angle_err = Accel - angle; // zk-�������

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0; // ����������Э����
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;

    angle += K_0 * Angle_err;  // �������
    Q_bias += K_1 * Angle_err; // �������
    angle_dot = Gyro - Q_bias; // ���ֵ(�������)��΢��=���ٶ�
    return angle;
}

/**************************************************************************
�������ܣ�һ�׻����˲�
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
void Yijielvbo(float angle_m, float gyro_m)
{
    angle = K1 * angle_m + (1 - K1) * (angle + gyro_m * 0.005);
}
