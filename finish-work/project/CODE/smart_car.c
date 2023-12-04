/*
 * smart_cat.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "smart_car.h"

#define IMAGE_WIDTH 80

/*���� index*/
int motor_start_flag = 1;

SMARTCAR_PARAM smartcar_param;
SMARTCAR_STATUS smartcar_status;

sbit Hall = P2 ^ 6;

/*=========================================�ж�·����Ϣ����================================================*/
int ele_lock = _BEND;
int road_type = BEND;
int det_cir_flag = 0;
int stop_flag = 0;
int stop_dec = 0;
int dec_time = 500;
int dec_out = 0;
static int lenth_init = 150;		//��ʼ���루�����㵽Բ���е�ľ��룩

static float start_dis = 0;
static int circle_flag = 0;
static float circle_dis_rec = 0;
static float circle_dis = 0;
static int circle_clear = 0;

static int falg = 0;
static int circle_dir = 0; // 1 Ϊ �� �� 2 Ϊ �һ� �� 0 Ϊ�޻�
int fork_dir = 0;          // ż��Ϊ��� ����Ϊ�Ҳ�

/**
 * @brief  ���ܳ�������ʼ��
 * @note
 * @param  None
 * @retval None
 */
 
 
 //Բ���ڵ�pidҪ����������200��
 //��ʼ���루�����㵽Բ���е�ľ��룩������28�� 
 //--20231124--�Ժ���
void SMARTCAR_Init(void)
{
    float *p_param;
    int i;
    p_param = (float *)&smartcar_param;
    for (i = 0; i < sizeof(SMARTCAR_PARAM) / 4; i++)
    {
        *p_param++ = 0;
    }
    smartcar_param.bend_steer_kp = 2.0;
    smartcar_param.bend_steer_ki = 0.0;
    smartcar_param.bend_steer_kd = 6.2;
    smartcar_param.motor_kp = 15;
    smartcar_param.motor_ki = 0;
    smartcar_param.motor_kd = 0.4;
    smartcar_param.circle_pid_ratio = 1.0;
    smartcar_param.circle_in_angle = -30;
    smartcar_param.circle_in_dis = 50;//�뻷���һС��ֱ�߾���
    smartcar_param.circle_len = 180;	//������Բ���ľ��루���������ߵľ��룩
    smartcar_param.circle_exit_dis = 1;
    smartcar_param.circle_exit_angle = 1;
    smartcar_param.cir_L_1 = 1;
    smartcar_param.cir_L_2 = 99999999;
    smartcar_param.cir_L_3 = 3009999;
    smartcar_param.global_speed = 1000.0;//ȫ���ٶ�
    smartcar_param.start_dis = 50.0;
    smartcar_param.start_angle = 10.0;
    smartcar_param.barn_stop_angle = 30.0;
    smartcar_param.barn_stop_distance = 30.0;
    smartcar_param.ramp_distance = 30.0;
    smartcar_param.fork_distance1 = 30.0;
    smartcar_param.fork_distance2 = 30.0;
    smartcar_param.fork_turn_distance = 30.0;
    smartcar_param.fork_in_angle = 1.0;
    smartcar_param.fork_out_angle = 2.0;
    smartcar_param.fork_L1 = 5000.0;
    smartcar_param.fork_L2 = 1000.0;
    smartcar_param.length_total = 1.0;
    smartcar_param.length_fork = 1.0;
    smartcar_param.length_ramp = 1.0;
    smartcar_param.length_circle1 = 1.0;
    smartcar_param.length_barn = 1.0;
    smartcar_param.run_direction = 1.0;
}

/**
 * @brief  ���ܳ�������λ���ָ�Ĭ�����ã�
 * @note
 * @param  None
 * @retval None
 */
void SMARTCAR_ResetConfig(void)
{
    SMARTCAR_Init();
}
/*=========================================�ж�·����Ϣ================================================*/
int get_lock(void);
void set_lock(int lock);
void clear_lock(void);
int detect_start(void);
int detect_out_bound(void);
int detect_ramp(void);
int detect_barn(void);
int detect_fork(void);
int detect_circle(void);

int get_lock(void)
{
    return ele_lock;
}
void set_lock(int lock)
{
    ele_lock = lock;
    beep(1, 1000, 90);
}
void clear_lock(void)
{
    ele_lock = _BEND;
    beep(3, 300, 50);
}

/*�ж�·����Ϣ*/
/* ----------------------------------------�������---------------------------------------------- */
int detect_start(void)
{
    static int start_flag = 0;
    static int start_end_flag = 0;

    if (start_end_flag == 1)
        return 0;

    if (motor_start_flag == 0)
    {
        start_flag = 0;
    }
    else
    {
        if (start_flag == 0)
        {
            start_flag = 1;
            start_dis = get_distance();
            set_lock(_START);
        }
        else if (start_flag == 1 && get_distance() - start_dis > smartcar_param.start_dis && start_end_flag == 0)
        {
            start_flag = 0;
            start_end_flag = 1;
            clear_lock();
        }
    }
    return start_flag;
}

/* ----------------------------------------���Բ��---------------------------------------------- */
int detect_circle(void)
{

    static uint8 circle_times;

    // ��������κ�������bend  �򷵻�
    if (get_lock() != _BEND && get_lock() != _CIRCLE)
        return 0;
    switch (circle_flag)//׼���뻷
    {
    case 0:
        if (((get_distance() - circle_dis_rec) > lenth_init) && det_cir_flag == 0 && circle_clear == 0) // �뻷 cir_L_1 15000
        {
            //            if(slope_left_error > smartcar_param.cir_L_2) //cir_L_2 0
            //                circle_dir = 1; // ��
            //            else if(slope_left_error <= -smartcar_param.cir_L_2)
            circle_dir = 2; // �һ�

            circle_flag = CIRCLE_ENTRY;
            beep(1, 1000, 90);
            set_lock(_CIRCLE);
            circle_clear = 1;
            circle_dis_rec = get_distance();
            circle_dis = get_distance();
            det_cir_flag = 1;
            smartcar_param.global_speed--;
            if (smartcar_param.global_speed < 350)
						{            
							smartcar_param.global_speed = 350;
							smartcar_param.motor_kp = 21.5;
							smartcar_param.motor_ki = 0;
							smartcar_param.motor_kd = 0;
						} 
					}
        break;
    case CIRCLE_ENTRY://�뻷ֱ��
        if ((get_distance() - circle_dis_rec) > smartcar_param.circle_in_dis) //smartcar_param.circle_in_dis �뻷����
        {
            circle_flag = CIRCLE_IN;
            circle_dis_rec = get_distance();
        }
        break;
    case CIRCLE_IN://����ѭ��
        if (circle_dir == 1) // �������
        {
            if ((get_distance() - circle_dis_rec > smartcar_param.circle_len) )// smartcar_param.circle_lenԲ������
//                || (inductor_value_list[4] < smartcar_param.cir_L_3))         // ������б < 300  cir_L_3 300
            {
                circle_flag = CIRCLE_EXIT;
                circle_dis_rec = get_distance();
                smartcar_param.global_speed--;
                if (smartcar_param.global_speed < 350)
                    smartcar_param.global_speed = 350;
            }
        }
        else if(circle_dir == 2) //������һ�
        {
        		if(((get_distance() - circle_dis_rec > smartcar_param.circle_len ))//Բ������
//                   ||(inductor_value_list[0] < smartcar_param.cir_L_3)) //������б < 300
           {
                  circle_flag = CIRCLE_EXIT;
            circle_dis_rec = get_distance();
     				smartcar_param.global_speed--;
        				if( smartcar_param.global_speed < 350)
       				smartcar_param.global_speed = 350;
           }

           }
        break;
    case CIRCLE_EXIT:
        if (get_distance() - circle_dis_rec > smartcar_param.circle_exit_dis) // ��������
        {
            circle_flag = 0;
            clear_lock();
            circle_dir = 0; // �޻�
            circle_clear = 0;
            //					motor_start_flag = 0;
            smartcar_param.global_speed++;
            if (smartcar_param.global_speed > 1200)
                smartcar_param.global_speed = 1200;
            smartcar_param.motor_kp = 15;
            smartcar_param.motor_ki = 0;
            smartcar_param.motor_kd = 0.4;
        }
        break;
    default:
        break;
    }
    return circle_flag;
}

/* ----------------------------------------���ֲ�---------------------------------------------- */

int detect_fork(void)
{
    static int fork_flag = 0;
    static float fork_dis_rec = 50;
    static uint8 fork_cnt = 0;
    static float fork_dis = 0;
    static int fork_clear = 0;

    if (get_lock() != _BEND && get_lock() != _FORK)
        return 0;

    switch (fork_flag)
    {
    case 0:
        if (slope_sum >= smartcar_param.fork_L1                            // fork_L1 5000
            && fork_flag <= 0 && fork_clear == 0 && motor_start_flag == 1) // ������
        {
            fork_flag = FORK_ENTRY;
            set_lock(_FORK);
            fork_clear = 1;
            fork_dis_rec = get_distance();
            fork_dis = get_distance();
        }
        break;
    case FORK_ENTRY:
        if (get_distance() - fork_dis_rec > smartcar_param.fork_turn_distance) // ���������
        {
            if (fork_dir % 2 == 0)
                fork_flag = FORK_IN_L;
            else
                fork_flag = FORK_IN_R;
            fork_dis_rec = get_distance();
        }
        break;
    case FORK_IN_L:
        if ((get_distance() - fork_dis_rec > smartcar_param.fork_distance1) // �����泤��
            || (slope_left_error > smartcar_param.fork_L2))                 // ������ бƫ1000 ֱƫ-500 ��б<100 //fork_L2 1000
        {
            fork_flag = FORK_EXIT;
            fork_dis_rec = get_distance();
        }
        break;
    case FORK_IN_R:
        if ((get_distance() - fork_dis_rec > smartcar_param.fork_distance2) // �����泤��
            || (slope_left_error < -smartcar_param.fork_L2))                // ���Ҳ�� бƫ-1000 ֱƫ-600 ��б<100
        {
            fork_flag = FORK_EXIT;
            fork_dis_rec = get_distance();
        }
        break;
    case FORK_EXIT:
        if (get_distance() - fork_dis_rec > smartcar_param.fork_turn_distance) // �����泤��
        {
            fork_flag = 0;
            clear_lock();
            fork_dir++;
            fork_clear = 0;
        }
        break;
    default:
        break;
    }
    return fork_flag;
}
/* ----------------------------------------��⳵��---------------------------------------------- */
int detect_barn(void)
{
    static int barn_flag = 0;
    static float barn_ang_rec = 0;
    static float barn_dis_rec = 0;
    static float barn_dis = 0;
    static int barn_clear = 0;
    if (get_lock() != _BEND && get_lock() != _BARN)
        return 0;
    switch (barn_flag)
    {
    case 0:
        if (get_distance() > 1750 && falg == 0)
        {
            barn_flag = BARN_ENTRY;
            clear_lock();
            set_lock(_BARN);
            barn_dis = get_distance();
            falg = 1;
            //						beep(1, 1000,90);
        }
        break;
    case BARN_ENTRY:
        if (get_distance() - barn_dis > 180 && falg == 1) // ͣ������
        {

            barn_flag = BARN_IN;
            //					beep(1, 1000,90);
        }
        break;
    case BARN_IN:
        if (get_distance() - barn_dis > 800) // ͣ������
        {

            barn_flag = 0;
            //					beep(1, 1000,90);

            clear_lock();
        }
        break;
    default:
        break;
    }
    return barn_flag;
}

/* ----------------------------------------������---------------------------------------------- */
int detect_out_bound(void)
{
    int ret = 0;
    if (inductor_sum < 1000)
    {
        ret = OUT_BOUND;
        road_type = OUT_BOUND;
    }
    else
        ;
    return ret;
}

void analyze_road(void)
{
    int ret;
    int ii;
    int sw_value;
    uint8 huidu1, huidu2, huidu3, huidu4;
    road_type = BEND;
    sw_value = get_sw_value();
    if ((huidu2 = P11) == 0 && stop_dec == 0)
    {
        det_cir_flag = 0;
        circle_flag = 0; // Բ������
        circle_dis_rec = 0;
        circle_dis = 0;
        circle_clear = 0;
        clear_distance();
        falg = 0;
        stop_dec = 1;
        beep(1, 1000, 70);
    }
    if (stop_flag > 0 && (huidu2 = P11) == 0) // ����������
        dec_out = 1;
    if (dec_out == 1)
        road_type = FINISH_STOP;
    if (stop_dec == 1 && (dec_time--) < 0)
    {
        stop_flag++;
    }

    //    if(detect_out_bound())
    //    {
    //        return;
    //    }
    //    if(0 != (ret = detect_start()))
    //    {
    //        road_type = START;
    //    }
    //    if(sw_value & 0x1)
    //    {

    //        if(0 != (ret = detect_barn()))
    //        {
    //            road_type = ret;
    //            return;
    //        }

    //    }
    //    if(sw_value & 0x2)
    //    {
    //        if(0 != (ret = detect_fork()))
    //        {
    //            road_type = ret;
    //            return;
    //        }
    //    }
    //    if((sw_value == 0)||(sw_value == 3))
    //    {
    if (0 != (ret = detect_circle()))
    {
        road_type = ret;
        return;
    }
    //    }
}
/*=================================================================================================*/
/*����·����Ϣ���п���*/
float *steer_kp = &(steer_pid.Kp);
float *steer_ki = &(steer_pid.Ki);
float *steer_kd = &(steer_pid.Kd);

float angle_decision(float middle_line)
{
    float angle;
    if (middle_line > IMAGE_WIDTH / 2)
        middle_line = IMAGE_WIDTH / 2;
    else if (middle_line < -IMAGE_WIDTH / 2)
        middle_line = -IMAGE_WIDTH / 2;
    set_pid_target(&steer_pid, 0);

#if STEER_DIR
    angle = -positional_pid_realize(&steer_pid, middle_line);
#else
    angle = positional_pid_realize(&steer_pid, middle_line);
#endif

    if (angle > STEER_MAX_ANGLE)
        angle = STEER_MAX_ANGLE;
    else if (angle < -STEER_MAX_ANGLE)
        angle = -STEER_MAX_ANGLE;

    return angle;
}

static int stop_time = 60;
void decision(void)
{
    float err;
    switch (road_type)
    {

    case BEND:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        if (smartcar_status.goal_angle < -45)
        {
            smartcar_status.left_goal_speed = 1030;
            smartcar_status.right_goal_speed = 1030 - 300;
        }
        if (smartcar_status.goal_angle > 45)
        {
            smartcar_status.left_goal_speed = 1030 - 300;
            smartcar_status.right_goal_speed = 1030;
        }
        break;
    case CIRCLE_ENTRY:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = 930;
        smartcar_status.right_goal_speed = 930;
        smartcar_status.motor_close_circle = 1;
        if (circle_dir == 1) // ��
            smartcar_status.goal_angle = smartcar_param.circle_in_angle;
        else if (circle_dir == 2) // �һ�
            smartcar_status.goal_angle = -smartcar_param.circle_in_angle;
        else
            smartcar_status.goal_angle = angle_decision(err);
        break;
    case CIRCLE_IN:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = 930;
        smartcar_status.right_goal_speed = 930;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = -smartcar_param.circle_in_angle;
        break;
    case CIRCLE_READY_EXIT:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = -55;
        break;
    case CIRCLE_EXIT:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = 930;
        smartcar_status.right_goal_speed = 930;
        smartcar_status.motor_close_circle = 1;
        if (circle_dir == 1) // ��
            smartcar_status.goal_angle = smartcar_param.circle_exit_angle;
        else if (circle_dir == 2) // �һ�
            smartcar_status.goal_angle = -smartcar_param.circle_exit_angle;
        else
            smartcar_status.goal_angle = -55;

        break;
    case FORK:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case FORK_ENTRY:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        if (fork_dir % 2 == 0) // ���
            smartcar_status.goal_angle = smartcar_param.fork_in_angle;
        else // �Ҳ�
            smartcar_status.goal_angle = -smartcar_param.fork_in_angle;
        break;
    case FORK_IN_L:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case FORK_IN_R:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case FORK_EXIT:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        if (fork_dir % 2 == 0) // ���
            smartcar_status.goal_angle = smartcar_param.fork_out_angle;
        else // �Ҳ�
            smartcar_status.goal_angle = -smartcar_param.fork_out_angle;
        break;
    case BARN:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);

        break;
    case BARN_ENTRY:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed * 1.1;
        smartcar_status.right_goal_speed = smartcar_param.global_speed * 1.1;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case BARN_IN:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = 950;
        smartcar_status.right_goal_speed = 950;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case BARN_EXIT:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = 1300;
        smartcar_status.right_goal_speed = 1300;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case CROSS:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case RAMP:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case OUT_BOUND:
        smartcar_status.steer_on = 0;
        smartcar_status.motor_on = 0;
        smartcar_status.left_goal_speed = 0;
        smartcar_status.right_goal_speed = 0;
        smartcar_status.motor_close_circle = 0;
        *steer_kp = 0;
        *steer_ki = 0;
        *steer_kd = 0;
        smartcar_status.goal_angle = 0;
        break;
    case LONG_STRAIGHT:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    case FINISH_ENTRY:
        smartcar_status.steer_on = 1;
        smartcar_status.motor_on = 1;
        smartcar_status.left_goal_speed = smartcar_param.global_speed;
        smartcar_status.right_goal_speed = smartcar_param.global_speed;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        smartcar_status.goal_angle = smartcar_param.barn_stop_angle;
        break;
    case FINISH_STOP: // ͣ��
        smartcar_status.steer_on = 0;
        smartcar_status.motor_on = 0;
        smartcar_status.left_goal_speed = 0;
        smartcar_status.right_goal_speed = 0;
        smartcar_status.motor_close_circle = 1;
        *steer_kp = smartcar_param.bend_steer_kp;
        *steer_ki = smartcar_param.bend_steer_ki;
        *steer_kd = smartcar_param.bend_steer_kd;
        err = get_inductor_err();
        smartcar_status.goal_angle = angle_decision(err);
        break;
    default:
        break;
    }
}

int get_road_type(void)
{
    return road_type;
}