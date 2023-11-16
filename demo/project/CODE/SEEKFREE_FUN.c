/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		FUN
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/



#include "SEEKFREE_FUN.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ֵ����
//  @param      dat				��Ҫ�����ֵ����
//  @return     int				���ؾ���ֵ
//  Sample usage:				dat = myabs(dat);//��dat�������
//-------------------------------------------------------------------------------------------------------------------
int  myabs(int dat)
{
    if(dat>=0)  return dat;
    else        return -dat;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ʱ
//  @param      t				��ʱʱ��
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void my_delay(long t)
{
    while(t--);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �޷�
//  @param      x				���޷�������
//  @param      y				�޷���Χ(���ݻᱻ������-y��+y֮��)
//  @return     float			�޷�֮�������
//  Sample usage:				float dat = limit(500,300);//���ݱ�������-300��+300֮��  ��˷��صĽ����300
//-------------------------------------------------------------------------------------------------------------------
float limit(float x, int32 y)
{
    if(x>y)             return y;
    else if(x<-y)       return -y;
    else                return x;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ˫���޷�
//  @param      x				���޷�������
//  @param      a				�޷���Χ��߽�
//  @param      b				�޷���Χ�ұ߽�
//  @return     float			�޷�֮�������
//  Sample usage:				int16 dat = limit_ab(500,-300��400);//���ݱ�������-300��+400֮��  ��˷��صĽ����400
//-------------------------------------------------------------------------------------------------------------------
int16 limit_ab(int16 x, int16 a, int16 b)
{
    if(x<a) x = a;
    if(x>b) x = b;
    return x;
}
