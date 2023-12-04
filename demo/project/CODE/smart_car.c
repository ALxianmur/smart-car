/*
 * smart_cat.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "smart_car.h"

#define IMAGE_WIDTH 80

/*参数 index*/
int motor_start_flag = 0;

SMARTCAR_PARAM smartcar_param ;
SMARTCAR_STATUS smartcar_status;

sbit Hall = P2^6;

/*=========================================判断路面信息变量================================================*/
int ele_lock = _BEND;
int road_type = BEND;
static float start_dis = 0;
int circle_dir = 0; // 1 为 左环 ， 2 为 右环 ， 0 为无环

/**
  * @brief  智能车参数初始化
  * @note
  * @param  None
  * @retval None
  */
void SMARTCAR_Init(void)
{
    float *p_param;
    int i;
    p_param = (float *) &smartcar_param;
    for(i = 0; i < sizeof(SMARTCAR_PARAM)/4; i++)
    {
        *p_param++ = 0;
    }
    smartcar_param.bend_steer_kp_H    = 0.0035;
    smartcar_param.bend_steer_kp_L    = 4.5 ;
    smartcar_param.bend_steer_kd      = 4.0 ;
    smartcar_param.motor_kp           = 3.5;
    smartcar_param.motor_ki           = 0.2;
    smartcar_param.motor_kd           = 1.4;
    smartcar_param.global_speed       = 500.0;
    smartcar_param.circle_pid_ratio   = 1.0 ;
    smartcar_param.circle_in_angle    = 25.0 ;
    smartcar_param.circle_ready_in_dis= 45.0; 
    smartcar_param.circle_in_dis	  = 78.0;
    smartcar_param.circle_len		  = 265.0;
    smartcar_param.circle_exit_dis    = 53.0;
    smartcar_param.circle_exit_angle  = 26.0 ;
    smartcar_param.cir_L_1  		  = 13000;
    smartcar_param.cir_L_2  		  = 150;
    smartcar_param.cir_L_3  		  = 80;
    smartcar_param.start_dis          = 55.0 ;
    smartcar_param.start_angle        = 38.0 ;
    smartcar_param.barn_in_dis    	  =	55.0 ;
    smartcar_param.barn_stop_angle    = 38.0 ;
    smartcar_param.barn_stop_distance = -65.0 ;
    smartcar_param.ramp_distance      = 30.0 ;
    smartcar_param.road_block_in_angle= 40.0;
    smartcar_param.road_block_in_dis  = 35.0;
    smartcar_param.road_block_exit_angle 	= -40.0;
    smartcar_param.road_block_exit_dis 		= 35.0;
    smartcar_param.road_block_len 			= 20.0;
    smartcar_param.road_block_front_dis1	= 700.0;
    smartcar_param.road_block_front_dis2	= 500.0;
    smartcar_param.length_total       = 1500.0 ;
    smartcar_param.length_block       = 300.0 ; 
    smartcar_param.length_ramp        = 1.0 ;
    smartcar_param.length_circle1     = 500.0 ;
    smartcar_param.length_barn        = 1.0 ;
    smartcar_param.run_direction      = 1.0 ;
    smartcar_param.limit_H_err        = 4.0 ;
    smartcar_param.limit_L_err        = 1.0 ;
	smartcar_param.steer_middle_value = 665;
	smartcar_param.sw_value 		  = 8.0;
}

/**
  * @brief  智能车参数复位（恢复默认设置）
  * @note
  * @param  None
  * @retval None
  */
void SMARTCAR_ResetConfig(void)
{
    SMARTCAR_Init();
}

/* ----------------------------------------检测出界---------------------------------------------- */
int detect_out_bound(void)
{
    int ret = 0;

	if( inductor_sum < 500 )
	{
		ret = OUT_BOUND;
		road_type = OUT_BOUND;
		beep(1, 100, 50);
	}
		else
			;
    return ret;
}

void analyze_road(void)
{
    road_type = BEND;
	
    if(detect_out_bound())
    {
        return;
    }

}
/*=================================================================================================*/
void decision(void)
{
    switch (road_type)
    {
	case OUT_BOUND:
        smartcar_status.steer_on = 0;
        smartcar_status.motor_on = 0;
        smartcar_status.left_goal_speed = 0;
        smartcar_status.right_goal_speed = 0;
        smartcar_status.motor_close_circle = 0;
//				*steer_kp = 0;
//        *steer_kd = 0;
//				*steer_ki = 0;
        smartcar_status.goal_angle = 0;
        break;
    default:
        break;
    }
}

int get_road_type(void)
{
    return road_type;
}