#include "PID/fire_pid.h"

/**
  * @brief  PID参数初始化
    *	@note 	无
  * @retval 无
  */
void PID_param_init(Pid *pid, double kp, double ki, double kd, double outMin, double outMax)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->outMin = outMin;
    pid->outMax = outMax;
}

/**
  * @brief  设置目标值
  * @param  val		目标值
    *	@note 	无
  * @retval 无
  */
void set_pid_target(Pid *pid, double temp_val)
{
    pid->target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
    *	@note 	无
  * @retval 目标值
  */
double get_pid_target(Pid *pid)
{
    return pid->target_val;    // 设置当前的目标值
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
    *	@note 	无
  * @retval 无
  */
void set_p_i_d(Pid *pid, double p, double i, double d)
{
    pid->Kp = p;    // 设置比例系数 P
    pid->Ki = i;    // 设置积分系数 I
    pid->Kd = d;    // 设置微分系数 D
}

/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
    *	@note 	无
  * @retval 通过PID计算后的输出
  */
double location_pid_realize(Pid *pid, double actual_val)
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;
  
    /* 设定闭环死区 */
    if((pid->err >= -20) && (pid->err <= 20)) {
        pid->err = 0;
        pid->integral = 0;
        pid->err_last = 0;
    }
    
    pid->integral += pid->err;    // 误差累积

    /*PID算法实现*/
    pid->output_val = pid->Kp * pid->err + 
                      pid->Ki * pid->integral + 
                      pid->Kd * (pid->err - pid->err_last);

    if(pid->output_val > pid->outMax) pid->output_val = pid->outMax;
    else if(pid->output_val < pid->outMin) pid->output_val = pid->outMin;

    /*误差传递*/
    pid->err_last = pid->err;
    
    /*返回当前实际值*/
    return pid->output_val;
}

/**
  * @brief  速度PID算法实现
  * @param  actual_val:实际值
    *	@note 	无
  * @retval 通过PID计算后的输出
  */
double speed_pid_realize(Pid *pid, double actual_val)
{
    /*计算目标值与实际值的误差*/
    pid->err = pid->target_val-actual_val;

    if((pid->err < 0.2f )&& (pid->err > -0.2f))
        pid->err = 0.0f;

    pid->integral += pid->err;    // 误差累积

    /*PID算法实现*/
    pid->output_val = pid->Kp * pid->err + 
                      pid->Ki * pid->integral + 
                      pid->Kd * (pid->err - pid->err_last);

    if(pid->output_val > pid->outMax) pid->output_val = pid->outMax;
    else if(pid->output_val < pid->outMin) pid->output_val = pid->outMin;

    /*误差传递*/
    pid->err_last = pid->err;
    
    /*返回当前实际值*/
    return pid->output_val;
}

/************************************ END OF FIER *******************************************************/
