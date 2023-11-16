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
#define M_I 0 //  ��ط�ѹ
#define R_M_I 3
#define R_I 4

#define LEFT_INDUC 0
#define LEFT_MID_INDUC 1
#define MID_INDUC 2 //  ��ط�ѹ
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
    adc_init(ADC_P15, 0); //  ��ط�ѹ
    adc_init(ADC_P05, 0);
    adc_init(ADC_P06, 0);
}

uint16 get_adc(uint8 num)
{
    uint16 adc_value;
    switch (num)
    {
    case 1:
        adc_value = adc_once(ADC_P00, ADC_12BIT); // alxian 12λ 0-4096
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
        adc_value = adc_once(ADC_P15, ADC_12BIT); //  ��ط�ѹ
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
    unsigned short int m_i[10]; // ��ص�ѹ
    unsigned short int r_m_i[10];
    unsigned short int r_i[10];
    int i;
    unsigned short int max, min, sum;

    old_inductor_value_list[0] = inductor_value_list[0];
    old_inductor_value_list[1] = inductor_value_list[1];
    old_inductor_value_list[2] = inductor_value_list[2]; //  ��ط�ѹ
    old_inductor_value_list[3] = inductor_value_list[3];
    old_inductor_value_list[4] = inductor_value_list[4];

    for (i = 0; i < 10; i++)
    {
        l_i[i] = get_adc(L_I);
        l_m_i[i] = get_adc(L_M_I);
        m_i[i] = get_adc(M_I); //  ��ط�ѹ
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
    inductor_value_list_raw[MID_INDUC] = (sum - max - min) / 8; //  ��ط�ѹ
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
    numerator = (float)(inductor_value_list[0] * (-RATIO * conf_level0) + inductor_value_list[1] * (-RATIO * conf_level1) + inductor_value_list[2] * (RATIO * conf_level2) * 0 + inductor_value_list[3] * (RATIO * conf_level3) + inductor_value_list[4] * (RATIO * conf_level4)); // ���м��?
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
 * Ԫ���ж� ���ݣ�
 * 1) ������ƫ�� ���ֵ�����С��midֵ < 0, ������֣���� angle > 0
 * 2) ������ƫ�� ���ֵ��С�Ҵ�midֵ > 0, ���Ҵ��֣���� angle < 0
 * 3) ���һ�����, ���Ҵ��֣� ͬ 2) ƫ�����ӣ����ֵ��С�Ҵ�
 * 4) ��������, ������֣� ͬ 1) ƫ�����ӣ����ֵ�����С
 *
 * ���²����ж�Ԫ��
 * horizon_diff ���в�ֵ
 * slope_diff  б��в�ֵ
 * horizon_sum  ���к�ֵ
 * slope_sum  б��к�ֵ
 * horizon_left_error ֱƫ  ��ߺ������ƫ�� = ���-�����+�Һᣩ/2
 * horizon_right_error  �ұߺ������ƫ�� = �Һ�-�����+�Һᣩ/2
 * slope_left_error  бƫ ���б������ƫ�� = ��б-����б+��б��/2
 * slope_right_error  �ұ�б������ƫ�� = ��б-����б+��б��/2
 * ����Ѱ��  ��  11000 7500 4700 0 0(�� ֱ�� б�� бƫ ֱƫ)  ����ƫ��10cm: -1400 -1400(бƫ ֱƫ)
 * �жϽ����� �� б��к�ֵ>5000 ���� ���Ҳ�� бƫ-1000 ֱƫ-600 ��б<100 ;������ бƫ1000 ֱƫ-500 ��б<100
 * �жϽ�Բ�� �� ��к�ֵ>14000  ���룺бƫ=232 ֱƫ = 1  �󻷳�����б<500
 * �������   :  ��к�ֵ<1000
 */
float inductor_sum;        // ��к�ֵ
float horizon_diff;        // ���в�ֵ
float slope_diff;          // б��в�ֵ
float horizon_sum;         // ���к�ֵ
float slope_sum;           // б��к�ֵ
float horizon_left_error;  // ֱƫ ��ߺ������ƫ�� = ���-�����+�Һᣩ/2
float horizon_right_error; // �ұߺ������ƫ�� = �Һ�-�����+�Һᣩ/2
float slope_left_error;    // бƫ���б������ƫ�� = ��б-����б+��б��/2
float slope_right_error;   // �ұ�б������ƫ�� = ��б-����б+��б��/2

static void updata_element_value(void)
{
    inductor_sum = get_inductor_sum(); // ��к�ֵ

    slope_diff = inductor_value_list[1] - inductor_value_list[3];   // б��в�ֵ
    horizon_diff = inductor_value_list[0] - inductor_value_list[4]; // ���в�ֵ
    slope_sum = inductor_value_list[1] + inductor_value_list[3];    // б��к�ֵ
    horizon_sum = inductor_value_list[0] + inductor_value_list[4];  // ���к�ֵ

    horizon_left_error = inductor_value_list[0] - horizon_sum / 2;  // ֱƫ ��ߺ������ƫ�� = ���-�����+�Һᣩ/2
    horizon_right_error = inductor_value_list[4] - horizon_sum / 2; // �ұߺ������ƫ�� = �Һ�-�����+�Һᣩ/2
    slope_left_error = inductor_value_list[1] - slope_sum / 2;      // бƫ ���б������ƫ�� = ��б-����б+��б��/2
    slope_right_error = inductor_value_list[3] - slope_sum / 2;     // �ұ�б������ƫ�� = ��б-����б+��б��/2
}
