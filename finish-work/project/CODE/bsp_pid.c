#include "bsp_pid.h"

_pid left_motor_pid, right_motor_pid;
_pid steer_pid;

//_pid pid_speed_erect_angle;
//_pid pid_angle;
//_pid pid_erect_angle_speed;
//_pid differential_pid ;
/**
 * @brief  PID������ʼ��
 *	@note 	��
 * @retval ��
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
 * @brief  ����Ŀ��ֵ
 * @param  val		Ŀ��ֵ
 *	@note 	��
 * @retval ��
 */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val; // ���õ�ǰ��Ŀ��ֵ
}

/**
 * @brief  ��ȡĿ��ֵ
 * @param  ��
 *	@note 	��
 * @retval Ŀ��ֵ
 */
float get_pid_target(_pid *pid)
{
  return pid->target_val; // ���õ�ǰ��Ŀ��ֵ
}

/**
 * @brief  ���ñ��������֡�΢��ϵ��
 * @param  p������ϵ�� P
 * @param  i������ϵ�� i
 * @param  d��΢��ϵ�� d
 *	@note 	��
 * @retval ��
 */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
  pid->Kp = p; // ���ñ���ϵ�� P
  pid->Ki = i; // ���û���ϵ�� I
  pid->Kd = d; // ����΢��ϵ�� D
}

/**
 * @brief  λ��PID�㷨ʵ��
 * @param  actual_val:ʵ��ֵ
 *	@note 	��
 * @retval ͨ��PID���������
 */
float positional_pid_realize(_pid *pid, float actual_val)
{
  /*����Ŀ��ֵ��ʵ��ֵ�����*/
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
 * @brief  ֱ���Ƕ�PID�㷨ʵ��
 * @param  actual_val:ʵ��ֵ
 *	@note 	��
 * @retval ͨ��PID���������
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
 * @brief  ����PID�㷨ʵ��
 * @param  actual_val:ʵ��ֵ
 *	@note 	��
 * @retval ͨ��PID���������
 */
float incremental_pid_realize(_pid *pid, float actual_val)
{
  /*����Ŀ��ֵ��ʵ��ֵ�����*/
  pid->err = pid->target_val - actual_val;

  /* �޶��ջ�����*/
  if ((pid->err < 0.2f) && (pid->err > -0.2f))
    pid->err = 0.0f;

  /*PID�㷨ʵ��*/
  pid->actual_val += pid->Kp * (pid->err - pid->err_next) + pid->Ki * pid->err + pid->Kd * (pid->err - 2 * pid->err_next + pid->err_last);
  /*�������*/
  pid->err_last = pid->err_next;
  pid->err_next = pid->err;
  /*���ص�ǰʵ��ֵ*/
  return pid->actual_val;
}

/**
 * @brief  ��Χ�޶�
 * @param  �ϡ����ޣ����ͣ� 1 8λ�� 2 16λ��3 ����
 *	@note 	��
 * @retval ��
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
