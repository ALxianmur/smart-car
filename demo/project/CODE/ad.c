/*
 * ad.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "ad.h"
#define RATIO 60

#define L_I 1
#define L_M_I 2
#define M_I 0 //  电池分压
#define R_M_I 3
#define R_I 4

#define LEFT_INDUC 0
#define LEFT_MID_INDUC 1
#define MID_INDUC 2 //  电池分压
#define RIGHT_MID_INDUC 3
#define RIGHT_INDUC 4

float inductor_value_list_raw[INDUCTOR_NUM];
float inductor_value_list[INDUCTOR_NUM];
float old_inductor_value_list[INDUCTOR_NUM];

float inductor_error = 0.0;
float raw_inductor_error = 0.0;

static void updata_element_value(void);

void ad_init(void)
{
    adc_init(ADC_P00, 0);
    adc_init(ADC_P01, 0);
    adc_init(ADC_P15, 0); //  电池分压
    adc_init(ADC_P05, 0);
    adc_init(ADC_P06, 0);
}

uint16 get_adc(uint8 num)
{
    uint16 adc_value;
    switch (num)
    {
    case 1:
        adc_value = adc_once(ADC_P00, ADC_12BIT); // alxian 12位 0-4096
        break;
    case 2:
        adc_value = adc_once(ADC_P01, ADC_12BIT);
        break;
    case 3:
        adc_value = adc_once(ADC_P05, ADC_12BIT);
        break;
    case 4:
        adc_value = adc_once(ADC_P06, ADC_12BIT);
        break;
    case 0:
        adc_value = adc_once(ADC_P15, ADC_12BIT); //  电池分压
        break;
    default:
        adc_value = 0;
        break;
    }
    return adc_value;
}

void update_inductor_value(void)
{
#define INDUC_FILTER_CUTT_OFF_HZ 100
    unsigned short int l_i[10];
    unsigned short int l_m_i[10];
    unsigned short int m_i[10]; // 电池电压
    unsigned short int r_m_i[10];
    unsigned short int r_i[10];
    int i;
    unsigned short int max, min, sum;

    old_inductor_value_list[0] = inductor_value_list[0];
    old_inductor_value_list[1] = inductor_value_list[1];
    old_inductor_value_list[2] = inductor_value_list[2]; //  电池分压
    old_inductor_value_list[3] = inductor_value_list[3];
    old_inductor_value_list[4] = inductor_value_list[4];

    for (i = 0; i < 10; i++)
    {
        l_i[i] = get_adc(L_I);
        l_m_i[i] = get_adc(L_M_I);
        m_i[i] = get_adc(M_I); //  电池分压
        r_m_i[i] = get_adc(R_M_I);
        r_i[i] = get_adc(R_I);
    }

    max = 0, min = 4096, sum = 0;
    for (i = 0; i < 10; i++)
    {
        max = MYMAX(max, l_i[i]);
        min = MYMIN(min, l_i[i]);
        sum += l_i[i];
    }
    inductor_value_list_raw[LEFT_INDUC] = (sum - max - min) / 8;
    first_order_lpf(&inductor_value_list[LEFT_INDUC], (sum - max - min) / 8.0, INDUC_FILTER_CUTT_OFF_HZ, 1000);

    max = 0, min = 4096, sum = 0;
    for (i = 0; i < 10; i++)
    {
        max = MYMAX(max, l_m_i[i]);
        min = MYMIN(min, l_m_i[i]);
        sum += l_m_i[i];
    }
    inductor_value_list_raw[LEFT_MID_INDUC] = (sum - max - min) / 8;
    first_order_lpf(&inductor_value_list[LEFT_MID_INDUC], (sum - max - min) / 8.0, INDUC_FILTER_CUTT_OFF_HZ, 1000);

    max = 0, min = 4096, sum = 0;
    for (i = 0; i < 10; i++)
    {
        max = MYMAX(max, m_i[i]);
        min = MYMIN(min, m_i[i]);
        sum += m_i[i];
    }
    inductor_value_list_raw[MID_INDUC] = (sum - max - min) / 8; //  电池分压
    first_order_lpf(&inductor_value_list[MID_INDUC], (sum - max - min) / 8.0, INDUC_FILTER_CUTT_OFF_HZ, 1000);

    max = 0, min = 4096, sum = 0;
    for (i = 0; i < 10; i++)
    {
        max = MYMAX(max, r_m_i[i]);
        min = MYMIN(min, r_m_i[i]);
        sum += r_m_i[i];
    }
    inductor_value_list_raw[RIGHT_MID_INDUC] = (sum - max - min) / 8;
    first_order_lpf(&inductor_value_list[RIGHT_MID_INDUC], (sum - max - min) / 8.0, INDUC_FILTER_CUTT_OFF_HZ, 1000);

    max = 0, min = 4096, sum = 0;
    for (i = 0; i < 10; i++)
    {
        max = MYMAX(max, r_i[i]);
        min = MYMIN(min, r_i[i]);
        sum += r_i[i];
    }
    inductor_value_list_raw[RIGHT_INDUC] = (sum - max - min) / 8;
    first_order_lpf(&inductor_value_list[RIGHT_INDUC], (sum - max - min) / 8.0, INDUC_FILTER_CUTT_OFF_HZ, 1000);
}

void get_inductor_error(float conf_level0, float conf_level1, float conf_level2, float conf_level3, float conf_level4, int using_cubic_fun, int using_5_induc)
{
    float numerator;
    float denominator;
    float ratio;
    static float raw_err;
    float err;
    static float last_protect_err = 0;
    numerator = (float)(inductor_value_list[0] * (-RATIO * conf_level0) + inductor_value_list[1] * (-RATIO * conf_level1) + inductor_value_list[2] * (RATIO * conf_level2) * 0 + inductor_value_list[3] * (RATIO * conf_level3) + inductor_value_list[4] * (RATIO * conf_level4)); // 锟斤拷锟叫硷拷锟?
    if (using_5_induc)
        denominator = (float)(inductor_value_list[0] + inductor_value_list[1] + inductor_value_list[2] + inductor_value_list[3] + inductor_value_list[4]);
    else
        denominator = (float)(inductor_value_list[0] + inductor_value_list[1] + inductor_value_list[3] + inductor_value_list[4]);

    if (denominator != 0)
        ratio = (numerator / denominator);
    else
        ratio = 0;

    if (denominator > 1100)
    {
        raw_err = ratio;
    }
    else
    {
        if (raw_err < 0)
            raw_err = -32;
        else if (raw_err > 0)
            raw_err = 32;
    }

    if (using_cubic_fun)
        ;
    else
        err = raw_err;
    inductor_error = err;

    updata_element_value();
}
float get_inductor_err(void)
{
    return inductor_error;
}

float get_inductor_raw_err(void)
{
    return raw_inductor_error;
}

unsigned short int get_inductor_sum(void)
{
    return (unsigned short int)(inductor_value_list[0] + inductor_value_list[1] + inductor_value_list[3] + inductor_value_list[4]);
}

void show_induc(void)
{
    printf(" ad:%5d,%5d,%5d,%5d,%5d,%5d\r\n", (int)inductor_value_list[0], (int)inductor_value_list[1],
           (int)inductor_value_list[2], (int)inductor_value_list[3], (int)inductor_value_list[4], (int)get_inductor_sum());
    printf(" adraw:%.0f,%.0f,%.0f,%.0f,%.0f\r\n", inductor_value_list_raw[0], inductor_value_list_raw[1],
           inductor_value_list_raw[2], inductor_value_list_raw[3], inductor_value_list_raw[4]);
}

/*
 * 元素判断 依据：
 * 1) 车辆右偏， 电感值左大右小，mid值 < 0, 向左打轮，输出 angle > 0
 * 2) 车辆左偏， 电感值左小右大，mid值 > 0, 向右打轮，输出 angle < 0
 * 3) 进右环内切, 向右打轮， 同 2) 偏右增加，电感值左小右大
 * 4) 进左环内切, 向左打轮， 同 1) 偏左增加，电感值左大右小
 *
 * 以下参数判断元素
 * horizon_diff 横电感差值
 * slope_diff  斜电感差值
 * horizon_sum  横电感和值
 * slope_sum  斜电感和值
 * horizon_left_error 直偏  左边横电感相对偏差 = 左横-（左横+右横）/2
 * horizon_right_error  右边横电感相对偏差 = 右横-（左横+右横）/2
 * slope_left_error  斜偏 左边斜电感相对偏差 = 左斜-（左斜+右斜）/2
 * slope_right_error  右边斜电感相对偏差 = 右斜-（左斜+右斜）/2
 * 正常寻迹  ：  11000 7500 4700 0 0(和 直和 斜和 斜偏 直偏)  车辆偏左10cm: -1400 -1400(斜偏 直偏)
 * 判断进三叉 ： 斜电感和值>5000 出： 从右叉出 斜偏-1000 直偏-600 左斜<100 ;从左叉出 斜偏1000 直偏-500 右斜<100
 * 判断进圆环 ： 电感和值>14000  左环入：斜偏=232 直偏 = 1  左环出：右斜<500
 * 冲出赛道   :  电感和值<1000
 */
float inductor_sum;        // 电感和值
float horizon_diff;        // 横电感差值
float slope_diff;          // 斜电感差值
float horizon_sum;         // 横电感和值
float slope_sum;           // 斜电感和值
float horizon_left_error;  // 直偏 左边横电感相对偏差 = 左横-（左横+右横）/2
float horizon_right_error; // 右边横电感相对偏差 = 右横-（左横+右横）/2
float slope_left_error;    // 斜偏左边斜电感相对偏差 = 左斜-（左斜+右斜）/2
float slope_right_error;   // 右边斜电感相对偏差 = 右斜-（左斜+右斜）/2

static void updata_element_value(void)
{
    inductor_sum = get_inductor_sum(); // 电感和值

    slope_diff = inductor_value_list[1] - inductor_value_list[3];   // 斜电感差值
    horizon_diff = inductor_value_list[0] - inductor_value_list[4]; // 横电感差值
    slope_sum = inductor_value_list[1] + inductor_value_list[3];    // 斜电感和值
    horizon_sum = inductor_value_list[0] + inductor_value_list[4];  // 横电感和值

    horizon_left_error = inductor_value_list[0] - horizon_sum / 2;  // 直偏 左边横电感相对偏差 = 左横-（左横+右横）/2
    horizon_right_error = inductor_value_list[4] - horizon_sum / 2; // 右边横电感相对偏差 = 右横-（左横+右横）/2
    slope_left_error = inductor_value_list[1] - slope_sum / 2;      // 斜偏 左边斜电感相对偏差 = 左斜-（左斜+右斜）/2
    slope_right_error = inductor_value_list[3] - slope_sum / 2;     // 右边斜电感相对偏差 = 右斜-（左斜+右斜）/2
}
