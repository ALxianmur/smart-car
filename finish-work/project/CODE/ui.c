/*
 * ui.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "ui.h"

MenuItem items[] = {
    // 共24个参数 3屏  step ,  max , min, title
    {0.2, 100.0, 0.0, "bend_kp"},    // bend_steer_kp;
    {0.1, 10.0, 0.0, "bend_ki"},     // bend_steer_ki;
    {0.1, 20.0, 0.0, "bend_kd"},     // bend_steer_kd;
    {1, 100.0, 0.0, "moto_kp"},      // motor_kp;
    {0.1, 20.0, 0.0, "moto_kd"},     // motor_ki;
    {0.1, 20.0, 0.0, "moto_ki"},     // motor_kd;
    {0.1, 10.0, 0.0, "cir_pid"},     // circle_pid_ratio;
    {1, 30.0, -30.0, "cirinag"},     // circle_in_angle;
    {10, 30.0, 0.0, "cir_i_d"},      // circle_in_dis;
    {10, 30.0, 0.0, "cir_len"},      // circle_len;
    {10, 30.0, 0.0, "cir_o_d"},      // circle_exit_dis;
    {1, 30.0, -30.0, "cirouta"},     // circle_exit_angle;
    {100, 8000.0, 1000.0, "cir_L1"}, // cir_L_1;  4000
    {10, 1000.0, 100.0, "cir_L2"},   // cir_L_2;  500
    {10, 1000.0, 10.0, "cir_L3"},    // cir_L_3;  100
    {50, 500.0, 0.0, "glb_spd"},     // global_speed;
    {50, 500.0, 0.0, "stt_dis"},     // start_dis;
    {1, 30.0, -30.0, "statagl"},     // start_angle;
    {1, 30.0, -30.0, "stopagl"},     // barn_stop_angle;
    {50, 500.0, 0.0, "stopdis"},     // barn_stop_distance ;
    {50, 500.0, 0.0, "rmapdis"},     // ramp_distance;
    {50, 500.0, 0.0, "fokdis1"},     // fork_distance1;
    {50, 500.0, 0.0, "fokdis2"},     // fork_distance2;
    {50, 500.0, 0.0, "foktdis"},     // fork_trun_distance;
    {1, 30.0, -30.0, "fokinag"},     // fork_in_angle;
    {1, 30.0, -30.0, "fokotag"},     // fork_out_angle;
    {50, 8000.0, 100.0, "forkL1"},   // fork_L1;
    {50, 8000.0, 100.0, "forkL2"},   // fork_L2;
    {100, 50000.0, 0.0, "len_tol"},  // length_total ;
    {100, 50000.0, 0.0, "len_fok"},  // length_fork ;
    {100, 50000.0, 0.0, "len_rap"},  // length_ramp ;
    {100, 50000.0, 0.0, "len_cir"},  // length_circle1 ;
    {100, 50000.0, 0.0, "len_ban"},  // length_barn;
    {1, 1.0, 0.0, "run_dir"},        // run_direction;
};

/*=================================================================================================*/
/*  智能车综合按键处理显示*/
void key_process(void)
{
    static int param_index = 0;
    unsigned char str[32];
    int key_value = 0;
    int i, page_index;
    MenuItem *p_param_item;
    float *p_param;

    p_param_item = items;
    p_param = (float *)&smartcar_param;
    page_index = param_index / 10;

    /*主屏获取显示按键值并修改 param_index*/
    key_value = get_key_value();
    /*主屏第一行显示 sw 按键值 参数序列号 road_type值
    sprintf(str,"sw:%d key:%d no:%2d rd:%2d ", (int)get_sw_value(),key_value, param_index,get_road_type());	*/
    /*第一行 显示 sw ：中线 ：控制角度： 4个电感值	*/
    sprintf(str, "%1d:%4d:%3.0f:%4d %4d %4d %4d", (int)get_sw_value(), (int)get_inductor_err(), (smartcar_status.goal_angle), (int)(inductor_value_list[0]), (int)(inductor_value_list[1]), (int)(inductor_value_list[3]), (int)(inductor_value_list[4]));
    ips114_showstr(0, 0, str);

    /*第4~8行 主屏显示参数*/
    for (i = 0; i < 10; i++)
    {
        if ((i + page_index * 10) == param_index)
            ips114_showstr(i % 2 * (120 - 4), 3 + i / 2, "*");
        else
            ips114_showstr(i % 2 * (120 - 4), 3 + i / 2, " ");

        if ((i + 10 * page_index) < (sizeof(SMARTCAR_PARAM) / 4))
        {
            ips114_showstr(i % 2 * (120) + 8, 3 + i / 2, (p_param_item + (i + page_index * 10))->title);
            if ((p_param_item + (i + page_index * 10))->step_level < 1)
                sprintf(str, "%5.1f", *(p_param + (i + page_index * 10)));
            else
                sprintf(str, "%5.0f", *(p_param + (i + page_index * 10)));
            ips114_showstr(i % 2 * (120) + 8 * 9, 3 + i / 2, str);
        }
        else
        {
            ips114_showstr(i % 2 * (120) + 8, 3 + i / 2, "       ");
            ips114_showstr(i % 2 * (120) + 8 * 9, 3 + i / 2, "      ");
        }
    }
    /*根据按键修改参数*/
    p_param_item = items;
    p_param = (float *)&smartcar_param;
    switch (key_value)
    {
    case 1: // 值 + step
        *(p_param + param_index) += (p_param_item + param_index)->step_level;
        if (*(p_param + param_index) > (p_param_item + param_index)->max_value)
            *(p_param + param_index) = (p_param_item + param_index)->min_value;
        break;
    case 2: // 值 - step
        *(p_param + param_index) -= (p_param_item + param_index)->step_level;
        if (*(p_param + param_index) < (p_param_item + param_index)->min_value)
            *(p_param + param_index) = (p_param_item + param_index)->max_value;
        break;
    case 3:
        param_index--;
        if (param_index < 0)
            param_index = sizeof(SMARTCAR_PARAM) / 4 - 1;
        break;
    case 4:
        param_index++;
        if (param_index >= sizeof(SMARTCAR_PARAM) / 4)
        {
            param_index = 0;
            motor_start_flag = 1;
        }
        break;
    default:
        break;
    }
}

/*=================================================================================================*/
/*  智能车显示*/
void display_Handle(void)
{
    unsigned char str[32];

    /*第一行 在函数中显示按键值或者4个电感值*/
    key_process();

    /*第二行显示 左速度 右速度 距离 lock值 road_type值 及电机是否打开 */
    sprintf(str, "%4d:%4d %6dcm,%1d:%2d,on:%1d", (int)get_left_speed(), (int)get_right_speed(), (int)get_distance(), (int)get_lock(), (int)get_road_type(), motor_start_flag);
    ips114_showstr(0, 1, str);

    /*第三行 显示 电感和 横电感和 斜电感和 左边横电感相对偏差 左边直电感相对偏差 */
    sprintf(str, "%5.0f %4.0f %4.0f:%5.0f,%5.0f", inductor_sum, horizon_sum, slope_sum, horizon_left_error, slope_left_error);
    sprintf(str, "mid:%3dSpdl:%.1fL:%dR:%d ", (int)get_inductor_err(),
            (smartcar_status.right_goal_speed), get_left_duty(), get_right_duty());
    ips114_showstr(0, 2, str);
}