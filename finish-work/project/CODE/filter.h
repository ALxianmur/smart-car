/*
 * filter.h
 *
 *  Created on: 2021��2��16��
 *      Author: sundm
 */

#ifndef USER_FILTER_H_
#define USER_FILTER_H_

#define WIN_SIZE 10

float SlidingFilter(float array[WIN_SIZE], float value);
float LowPassFilter(float array[WIN_SIZE], float value);
float first_order_lpf(float *lpf, float val, float cut_off_freq, float sample_freq);

float Kalman_Filter(float Accel, float Gyro);
void Yijielvbo(float angle_m, float gyro_m);

#endif /* USER_FILTER_H_ */