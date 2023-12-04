/*
 * ad.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef _AD_H
#define _AD_H
#include "headfile.h"
#define INDUCTOR_NUM 5

extern float inductor_value_list[INDUCTOR_NUM];

void ad_init(void);
uint16 get_adc(uint8 num);

void update_inductor_value(void);
float get_inductor_error(float conf_level0, float conf_level1, float conf_level2, float conf_level3, float conf_level4, int using_cubic_fun, int using_5_induc);
void show_induc(void);
float get_inductor_err(void);

/* ���²����� smart_car.c �� analyze_road�е��� */
extern float inductor_sum;        // ��к�ֵ
extern float horizon_diff;        // ���в�ֵ
extern float slope_diff;          // б��в�ֵ
extern float horizon_sum;         // ���к�ֵ
extern float slope_sum;           // б��к�ֵ
extern float horizon_left_error;  // ֱƫ ��ߺ������ƫ�� = ���-�����+�Һᣩ/2
extern float horizon_right_error; // �ұߺ������ƫ�� = �Һ�-�����+�Һᣩ/2
extern float slope_left_error;    // бƫ���б������ƫ�� = ��б-����б+��б��/2
extern float slope_right_error;   // �ұ�б������ƫ�� = ��б-����б+��б��/2

#endif