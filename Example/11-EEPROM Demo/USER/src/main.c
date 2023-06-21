/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"



/*
 * 系统频率，可查看board.h中的 FOSC 宏定义修改。
 * board.h文件中FOSC的值设置为0,则程序自动设置系统频率为33.1776MHZ
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */


uint8 write_buff[] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38};
uint8 read_buff[8];


void main()
{
    board_init();			// 初始化寄存器,勿删除此句代码。

	// 使用之前务必查看 ..\Example\11-EEPROM Demo\MDK文件夹下面的【必看】STC-ISP设置.png
	// 使用之前务必查看 ..\Example\11-EEPROM Demo\MDK文件夹下面的【必看】STC-ISP设置.png
	// 使用之前务必查看 ..\Example\11-EEPROM Demo\MDK文件夹下面的【必看】STC-ISP设置.png
    iap_init();				// 初始化EEPROM
    
	// 如果要写入的地址里面有数据。就需要先擦除再写入。
	// 擦除地址0所在的扇区数据，一共512个字节
	iap_erase_page(0);		
	
	// 将write_buff这个数组写入到EEPROM中，地址(0x00-0x07)，共8个数据
	iap_write_bytes(0x00, write_buff, 8);
	
	// 将EEPROM中的内容读取到read_str字符串中，地址(0x00-0x07)，共8个字符
	iap_read_bytes(0x00, read_buff, 8);

    while(1)
	{
        // 串口发送读取到的数据
		// 使用文本模式，串口助手显示的数据为:12345678
		// 使用HEX模式，串口助手显示的数据为:0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 
        uart_putbuff(UART_1, read_buff, 8);
        delay_ms(500);
    }
}



