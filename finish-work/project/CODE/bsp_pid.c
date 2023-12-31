#include "bsp_pid.h"

_pid left_motor_pid, right_motor_pid;
_pid steer_pid;

//_pid pid_speed_erect_angle;
//_pid pid_angle;
//_pid pid_erect_angle_speed;
//_pid differential_pid ;
/**
 * @brief  PID参数初始化
 *	@note 	无
 * @retval 无
 */
void PID_param_init(void)
{
  left_motor_pid.Kp = smartcar_param.motor_kp;
  left_motor_pid.Ki = smartcar_param.motor_ki;
  left_motor_pid.Kd = smartcar_param.motor_kd;
  right_motor_pid.Kp = smartcar_param.motor_kp;
  right_motor_pid.Ki = smartcar_param.motor_ki;
  right_motor_pid.Kd = smartcar_param.motor_kd;

  steer_pid.Kp = smartcar_param.bend_steer_kp;
  steer_pid.Ki = smartcar_param.bend_steer_ki;
  steer_pid.Kd = smartcar_param.bend_steer_kd;
}

/**
 * @brief  设置目标值
 * @param  val		目标值
 *	@note 	无
 * @retval 无
 */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val; // 设置当前的目标值
}

/**
 * @brief  获取目标值
 * @param  无
 *	@note 	无
 * @retval 目标值
 */
float get_pid_target(_pid *pid)
{
  return pid->target_val; // 设置当前的目标值
}

/**
 * @brief  设置比例、积分、微分系数
 * @param  p：比例系数 P
 * @param  i：积分系数 i
 * @param  d：微分系数 d
 *	@note 	无
 * @retval 无
 */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
  pid->Kp = p; // 设置比例系数 P
  pid->Ki = i; // 设置积分系数 I
  pid->Kd = d; // 设置微分系数 D
}

/**
 * @brief  位置PID算法实现
 * @param  actual_val:实际值
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float positional_pid_realize(_pid *pid, float actual_val)
{
  /*计算目标值与实际值的误差*/
  pid->err = pid->target_val - actual_val;

  pid->integral += pid->err;

  pid->actual_val = pid->Kp * pid->err +
                    pid->Ki * pid->integral +
                    pid->Kd * (pid->err - pid->err_next);

  pid->err_last = pid->err_next;
  pid->err_next = pid->err;
  return pid->actual_val;
}

/**
 * @brief  直立角度PID算法实现
 * @param  actual_val:实际值
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float erect_angle_positional_pid_realize(_pid *pid, float actual_val)
{
  pid->err = pid->target_val - actual_val;

  if ((pid->err >= -50) && (pid->err <= 50))
  {
    pid->err = 0;
    pid->integral = 0;
  }

  if (pid->err > -200 && pid->err < 200)
  {
    pid->integral += pid->err;

    if (pid->integral > pid->integral_up_limit)
      pid->integral = pid->integral_up_limit;
    else if (pid->integral < pid->integral_down_limit)
      pid->integral = pid->integral_down_limit;
  }

  pid->actual_val = pid->Kp * pid->err +
                    pid->Ki * pid->integral +
                    pid->Kd * (pid->err - pid->err_next);

  pid->err_last = pid->err_next;
  pid->err_next = pid->err;
  return pid->actual_val;
}

/**
 * @brief  增量PID算法实现
 * @param  actual_val:实际值
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float incremental_pid_realize(_pid *pid, float actual_val)
{
  /*计算目标值与实际值的误差*/
  pid->err = pid->target_val - actual_val;

  /* 限定闭环死区*/
  if ((pid->err < 0.2f) && (pid->err > -0.2f))
    pid->err = 0.0f;

  /*PID算法实现*/
  pid->actual_val += pid->Kp * (pid->err - pid->err_next) + pid->Ki * pid->err + pid->Kd * (pid->err - 2 * pid->err_next + pid->err_last);
  /*传递误差*/
  pid->err_last = pid->err_next;
  pid->err_next = pid->err;
  /*返回当前实际值*/
  return pid->actual_val;
}

/**
 * @brief  范围限定
 * @param  上、下限；类型： 1 8位； 2 16位；3 浮点
 *	@note 	无
 * @retval 无
 */
void range_protect(void *value, float upper_limit, float lower_limit, uint8 type)
{
  switch (type)
  {
  case 1: //
  {
    uint8 *p = (uint8 *)value;
    if ((*p) > (uint8)upper_limit)
      *p = (uint8)upper_limit;
    else if ((*p) < (uint8)lower_limit)
      *p = (uint8)lower_limit;
    break;
  }
  case 2: // int16_t
  {
    int16 *p = (int16 *)value;
    if ((*p) > (int16)upper_limit)
      *p = (int16)upper_limit;
    else if ((*p) < (int16)lower_limit)
      *p = (int16)lower_limit;
    break;
  }
  case 3: // float
  {
    float *p = (float *)value;
    if ((*p) > (float)upper_limit)
      *p = (float)upper_limit;
    else if ((*p) < (float)lower_limit)
      *p = (float)lower_limit;
    break;
  }
  }
}
