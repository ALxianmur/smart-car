/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 *��board_init��,�Ѿ���P54��������Ϊ��λ
 *�����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */


uint16 adc_data[3];
void main()
{
    WTST = 0;               //���ó������ȴ���������ֵΪ0�ɽ�CPUִ�г�����ٶ�����Ϊ���
    
	DisableGlobalIRQ();		//�ر����ж�
	
    //sys_clk��ѡֵ:35000000��30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600��
    //����ϵͳƵ�ʣ���Ƶ����Ҫ��STC-ISP�����е� <�����û��������е�IRCƵ��>ѡ���Ƶ��һ�¡�
    //���Ƶ�����ò��ԣ����ᵼ�´��ڵ����ݲ�����,PWM�Ĺ����������ȵȡ�
    sys_clk = 35000000;     //����ϵͳƵ��Ϊ35MHz
    
	board_init();			//��ʼ���Ĵ���
	//�˴���д�û�����(���磺�����ʼ�������)

    adc_init(ADC_P10, ADC_SYSclk_DIV_2);	//��ʼ��ADC,P1.0ͨ�� ��ADCʱ��Ƶ�ʣ�SYSclk/2
    adc_init(ADC_P11, ADC_SYSclk_DIV_2);	//��ʼ��ADC,P1.1ͨ�� ��ADCʱ��Ƶ�ʣ�SYSclk/2
    adc_init(ADC_P13, ADC_SYSclk_DIV_2);	//��ʼ��ADC,P1.2ͨ�� ��ADCʱ��Ƶ�ʣ�SYSclk/2

	//���ж������
	EnableGlobalIRQ();		//�������ж�
    while(1)
	{
		//ʹ�����ߵ��ԣ��鿴adc_data�������ֵ�����Եõ�AD���ݡ�
		adc_data[0] = adc_once(ADC_P10, ADC_12BIT);	//�ɼ�һ��ADC������10λ
		adc_data[1] = adc_once(ADC_P11, ADC_10BIT);	//�ɼ�һ��ADC������9λ
		adc_data[2] = adc_once(ADC_P13, ADC_8BIT);	//�ɼ�һ��ADC������8λ
		printf("adc_data[0] = %d\r\n", adc_data[0]);
		printf("adc_data[1] = %d\r\n", adc_data[1]);
		printf("adc_data[2] = %d\r\n", adc_data[2]);
		
		delay_ms(100);
    }
}


