/*
 * smart_cat.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */
#ifndef __SMARTCAR_H__
#define __SMARTCAR_H__

#include "headfile.h"

typedef struct SmartCar_Param // ���ܳ���������
{
  float bend_steer_kp;
  float bend_steer_ki;
  float bend_steer_kd;
  float motor_kp;
  float motor_ki;
  float motor_kd;
  float circle_pid_ratio;
  float circle_in_angle;
  float circle_in_dis;
  float circle_len;
  float circle_exit_dis;
  float circle_exit_angle;
  float cir_L_1;
  float cir_L_2;
  float cir_L_3;
  float global_speed;
  float start_dis;
  float start_angle;
  float barn_stop_angle;
  float barn_stop_distance;
  float ramp_distance;
  float fork_distance1;
  float fork_distance2;
  float fork_turn_distance;
  float fork_in_angle;
  float fork_out_angle;
  float fork_L1;
  float fork_L2;
  float length_total;
  float length_fork;
  float length_ramp;
  float length_circle1;
  float length_barn;
  float run_direction;
} SMARTCAR_PARAM;

typedef struct SmartCar_Status // ���ܳ����в���
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
  BEND = 0,
  START = 10,
  // ����
  CIRCLE = 20,
  CIRCLE_ENTRY = 21,
  CIRCLE_IN = 22,
  CIRCLE_READY_EXIT = 23,
  CIRCLE_EXIT = 24,
  // ���
  FORK = 30,
  FORK_ENTRY = 31,
  FORK_IN_L = 32,
  FORK_IN_R = 33,
  FORK_EXIT = 34,
  // ����
  BARN = 40,
  BARN_ENTRY = 41,
  BARN_IN = 42,
  BARN_EXIT = 43,
  CROSS = 50,
  RAMP = 60,
  OUT_BOUND = 70,
  LONG_STRAIGHT = 80,
  FINISH_ENTRY = 90,
  FINISH_STOP = 91,
};

enum ele_lock_ty
{
  _BEND = 0,
  _START = 1,
  _CIRCLE = 2,
  _FORK = 3,
  _BARN = 4,
  _CROSS = 5,
  _RAMP = 6,
  _OUT_BOUND = 7,
};

extern SMARTCAR_PARAM smartcar_param;
extern SMARTCAR_STATUS smartcar_status;
extern int motor_start_flag;

void SMARTCAR_Init(void);
void SMARTCAR_ResetConfig(void);
void analyze_road(void);
void decision(void);
int get_road_type(void);
int get_lock(void);

#endif