/*
 * smart_cat.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */
#ifndef __SMARTCAR_H__
#define __SMARTCAR_H__

#include "headfile.h"

typedef struct  SmartCar_Param										//智能车调整参数
{
    float bend_steer_kp_H;
    float bend_steer_kp_L;
    float bend_steer_kd;
    float motor_kp;
    float motor_ki;
    float motor_kd;
    float global_speed;
    float circle_pid_ratio;
    float circle_in_angle;
    float circle_ready_in_dis; 
    float circle_in_dis;
    float circle_len;
    float circle_exit_dis;
    float circle_exit_angle;
    float cir_L_1;
    float cir_L_2;
    float cir_L_3;
    float start_dis;
    float start_angle;
	float barn_in_dis;
    float barn_stop_angle;   
    float barn_stop_distance ;
    float ramp_distance;
    float road_block_in_angle;
    float road_block_in_dis;
    float road_block_exit_angle;
    float road_block_exit_dis;
    float road_block_len;
    float road_block_front_dis1;
    float road_block_front_dis2;
    float length_total ;
    float length_block ;
    float length_ramp ;
    float length_circle1 ;
    float length_barn;
    float run_direction;
    float limit_H_err;
    float limit_L_err;
	float steer_middle_value;
	float sw_value;
} SMARTCAR_PARAM;

typedef struct SmartCar_Status     //智能车运行参数
{
    unsigned char steer_on;
    unsigned char motor_on;
    float goal_angle;
    unsigned char motor_close_circle;
    float left_goal_speed;
    float right_goal_speed;
    int left_duty_u;
    int right_duty_u;
} SMARTCAR_STATUS;

enum Road_Type
{
    BEND              = 0,
    START             = 10,
    START_ENTRY       = 11, 
    START_IN          = 12, 
    START_EXIT        = 13, 
    CIRCLE            = 20,
    CIRCLE_READY_ENTRY= 21,
    CIRCLE_ENTRY      = 22,
    CIRCLE_IN         = 23,
    CIRCLE_READY_EXIT = 24,
    CIRCLE_EXIT       = 25,
    BLOCK             = 30,
    BLOCK_ENTRY1      = 31, 
    BLOCK_ENTRY2      = 32, 
    BLOCK_IN          = 33, 
    BLOCK_EXIT1       = 34, 
    BLOCK_EXIT2       = 35, 
    BARN              = 40,
    BARN_ENTRY        = 41, 
    BARN_IN           = 42, 
    BARN_EXIT         = 43, 
    CROSS             = 50,
    RAMP              = 60,
    OUT_BOUND         = 70,
    LONG_STRAIGHT     = 80,
    FINISH		      = 90,
    FINISH_ENTRY      = 91,
    FINISH_IN	      = 92,
    FINISH_STOP       = 93,
};

enum ele_lock_ty
{
    _BEND = 0,
    _START = 1,
    _CIRCLE = 2,
    _BLOCK = 3,
    _BARN = 4,
    _CROSS = 5,
    _RAMP = 6,
    _OUT_BOUND = 7,
    _FINISH = 9,
};


extern SMARTCAR_PARAM smartcar_param;
extern SMARTCAR_STATUS smartcar_status;
extern int motor_start_flag;

void SMARTCAR_Init(void);
void SMARTCAR_ResetConfig(void);
void analyze_road(void);
void decision(void);
int get_road_type(void);

#endif