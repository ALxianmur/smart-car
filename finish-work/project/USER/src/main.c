#include "headfile.h"

static void TestMotor(void);
static void TestSteer(void);
static void TestAD(void);
static void TestEncoder(void);
// static void TestMPU6050(void);
// static void Test20602(void);

char read_buf[10];
uint32 dat_len = 0;

/*外设初始化*/
static void peripheral_init(void)
{
	ips114_init();
	ips114_clear(WHITE);

	steer_init();
	led_init();
	encoder_init();
	beep_init();
	beep(2, 500, 50); // 响2声 周期500ms 响声占50%
	wireless_uart_init();
	motor_init();
	ad_init();
	pit_timer_ms(TIM_4, 1);

	ips114_showstr(0, 0, "Init OK!");
}
uint8 senddata[256] = {0};
void main()
{
	int alarm_time;
	float ls, rs;
	uint8 ad1, ad2, ad3, ad4;
	WTST = 0;
	DisableGlobalIRQ();
	sys_clk = 35000000;

	board_init();
	peripheral_init();

	EnableGlobalIRQ(); // 开启总中断

	printf("Init OK! \r\n");
	SMARTCAR_Init();
	//	iap_init();
	//	e2prom_init_data(&fd, 0, &smartcar_param, sizeof(SMARTCAR_PARAM));

	wireless_uart_send_buff((uint8 *)"\r\nSEEKFREE wireless test.", 25);
	wireless_uart_send_buff((uint8 *)"\r\n", 2); // 换行

	while (1)
	{
		display_Handle();

		//		TestSteer();
		//		TestMotor();
		//		TestAD();
		//		TestEncoder();
		sprintf(senddata, "diangab:%D,%D,%d,%d,%d,%d\n", ad1 = P10, ad2 = P11, (int)inductor_value_list[0], (int)inductor_value_list[1], (int)inductor_value_list[3], (int)inductor_value_list[4]);
		wireless_uart_send_buff((uint8 *)senddata, sizeof(senddata));
		// 读取fifo中的内容
		dat_len = wireless_uart_read_buff(read_buf, 10);
		// 如果读取到数据
		//        if(dat_len != 0)
		//        {
		//            // 将读取到的fifo发送出去
		//            wireless_uart_send_buff(read_buf, (uint16)dat_len);
		//					  delay_ms(1);
		//					  wireless_uart_send_buff((uint8 *)"\r\n", 2);        // 换行
		//					  if(read_buf[0] == 'a')
		//						{
		//						    alarm_time = read_buf[2] - 0x30;
		//							beep(alarm_time, 200,10);
		//						}
		//						else if(read_buf[0] == 'e')
		//						{
		//								test_endian();
		//						}
		//						else if(read_buf[0] == 'p')
		//						{
		//							printf("this is test \r\n");
		//						}
		//						else if(read_buf[0] == 's')
		//						{
		//							  e2prom_save_data(&fd);
		//								printf("save data \r\n");
		//						}
		//						else if(read_buf[0] == 'l')
		//						{
		//							  e2prom_load_data(&fd);
		//								printf("load data \r\n");
		//						}
		//						else if(read_buf[0] == 'r')
		//						{
		//							  e2prom_reset_data();
		//								printf("reset data \r\n");
		//						}

		//        }
	}
}

void TestMotor(void)
{
	static uint8 motor_dir = 1;
	static int16 motor_duty = 0;
	if (motor_dir)
	{
		motor_duty += 100;
	}
	else
	{
		motor_duty -= 100;
	}

	if (PWM_MAX <= motor_duty)
		motor_dir = 0;
	else if (-PWM_MAX >= motor_duty)
		motor_dir = 1;

	left_motor_duty(motor_duty);
	right_motor_duty(motor_duty);
	ips114_showstr(12 * 8, 1, "motor:");
	ips114_showint32(18 * 8, 1, motor_duty, 5);
}

void TestSteer()
{
	static uint16 dir = 1;
	static uint16 duty = STEER_MIDDLE;
	static float angle = 0;

	//	if(dir) duty ++;
	//	else duty--;
	//	if(duty > STEER_MAX) dir = 0; //反向
	//	else if(duty < STEER_MIN) dir = 1;
	//	steer_duty(duty);
	//	ips114_showstr(0,1,"steer:"); ips114_showint32(56,1,duty,5);

	if (dir)
		angle++;
	else
		angle--;
	if (angle > STEER_MAX_ANGLE)
		dir = 0; // 反向
	else if (angle < STEER_MIN_ANGLE)
		dir = 1;
	steer_angle(angle);
	ips114_showstr(0, 1, "angle:");
	ips114_showfloat(56, 1, angle, 3, 1);
}

void TestAD(void)
{
	uint16 data0, data1, data2, data3, data4;
	data0 = get_adc(0);
	data1 = get_adc(1);
	data2 = get_adc(2);
	data3 = get_adc(3);
	data4 = get_adc(4);
	ips114_showstr(0, 2, "AD:");
	ips114_showuint16(6 * 8, 2, data0);
	ips114_showuint16(0, 3, data1);
	ips114_showuint16(6 * 8, 3, data2);
	ips114_showuint16(12 * 8, 3, data3);
	ips114_showuint16(18 * 8, 3, data4);
}

void TestEncoder(void)
{
	ips114_showstr(0, 4, "encode:");
	ips114_showint16(8 * 7, 4, get_left_encoder());
	ips114_showint16(8 * 14, 4, get_right_encoder());
	ips114_showfloat(8 * 20, 4, get_distance(), 5, 1);
}