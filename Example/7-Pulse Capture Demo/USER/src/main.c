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
 * ϵͳƵ�ʣ��ɲ鿴board.h�е� FOSC �궨���޸ġ�
 * board.h�ļ���FOSC��ֵ����Ϊ0,������Զ�����ϵͳƵ��Ϊ33.1776MHZ
 * ��board_init��,�Ѿ���P54��������Ϊ��λ
 * �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */

#define DIR P35
int16 dat = 0;

void main()
{
	board_init();			// ��ʼ���Ĵ���,��ɾ���˾���롣
	
	// �˴���д�û�����(���磺�����ʼ�������)
	
	ctimer_count_init(CTIM0_P34);

    while(1)
	{
        if(DIR == 1)
		{
			dat = ctimer_count_read(CTIM0_P34);
		}
		else
		{
			dat = ctimer_count_read(CTIM0_P34) * -1;
		}
		ctimer_count_clean(CTIM0_P34);
		printf("dat = %d\r\n", dat);
		delay_ms(100);
    }
}


