#include "headfile.h"

static void TestMotor(void);
static void TestSteer(void);
static void TestAD(void);
static void TestEncoder(void);
static void TestKey(void);

char read_buf[10];
uint32 dat_len = 0;

/*�����ʼ��*/
static void peripheral_init(void)
{
	ips114_init();
	ips114_clear(WHITE);

	steer_init();
	led_init();
	encoder_init();
	beep_init();
	beep(2, 500, 50); // ��2�� ����500ms ����ռ50%
	wireless_uart_init();
	motor_init();
	ad_init();
	pit_timer_ms(TIM_4, 1);

	ips114_showstr(0, 0, "Init OK!");
}

void main()
{
	WTST = 0;//���ý�cpu��ִ���ٶȵ������
	DisableGlobalIRQ();//�ر����ж�
	sys_clk = 35000000;//����ϵͳƵ�ʣ���Ҫ��stc-isp�е�Ƶ��һ��
	board_init();//��ʼ���Ĵ���
	peripheral_init();//�����ʼ��
	EnableGlobalIRQ(); // �������ж�

	printf("Init OK! \r\n");
	SMARTCAR_Init();//���ܳ�������ʼ��

	while (1)
	{
		TestMotor();
		TestAD();
		TestEncoder();
		TestSteer();
		TestKey();
		printf("123456789/n");
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

	if (PWM_MAX / 2 <= motor_duty)
		motor_dir = 0;
	else if (-PWM_MAX / 2 >= motor_duty)
		motor_dir = 1;

	left_motor_duty(motor_duty);
	right_motor_duty(motor_duty);
	ips114_showstr(14 * 8, 1, "motor:");
	ips114_showint32(20 * 8, 1, motor_duty, 5);
}

void TestSteer(void)
{
	float *steer_mid = &smartcar_param.steer_middle_value;
	steer_duty(steer_mid);
	ips114_showstr(0, 1, "steer:");
	ips114_showfloat(7 * 8, 1, *steer_mid, 3, 0);
}

void TestKey(void)
{
	float *steer_mid = &smartcar_param.steer_middle_value;
	int key_value = 0;
	key_value = get_key_value();
	switch (key_value)
	{
	case 1: // ֵ + step
		*steer_mid = *steer_mid + 10;
		break;
	case 2: // ֵ - step
		*steer_mid = *steer_mid - 10;
		break;
	case 3: // ֵ + step
		*steer_mid = *steer_mid + 1;
		break;
	case 4: // ֵ - step
		*steer_mid = *steer_mid - 1;
		break;
	default:
		break;
	}
	ips114_showstr(0, 5, "key:");
	ips114_showint32(16 * 8, 5, key_value, 1);
	ips114_showint32(6 * 8, 5, get_is_editing_flag(), 1);
	ips114_showstr(0, 6, "sw_value:");
	ips114_showint32(16 * 8, 6, get_sw_value(), 1);
}

void TestAD(void)
{
	unsigned char str[32];
	sprintf(str, "vbat:%.1f  mid :%d ", (float)inductor_value_list[2] * 5 * 4 / 4096, (int)get_inductor_err());
	ips114_showstr(0, 2, str);
	sprintf(str, " %d %d %d %d ", (int)(inductor_value_list[0]), (int)(inductor_value_list[1]), (int)(inductor_value_list[3]), (int)(inductor_value_list[4]));
	ips114_showstr(0, 3, str);
}

void TestEncoder(void)
{
	ips114_showstr(0, 4, "encode:");
	ips114_showint16(8 * 7, 4, get_left_encoder());
	ips114_showint16(8 * 14, 4, get_right_encoder());
	ips114_showfloat(8 * 20, 4, get_distance(), 5, 1);
}