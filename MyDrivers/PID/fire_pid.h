#ifndef __FIRE_PID_H
#define __FIRE_PID_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    double target_val;           //目标值
    double output_val;           //实际值
    double err;                  //定义偏差值
    double err_last;             //定义上一个偏差值
    double Kp,Ki,Kd;             //定义比例、积分、微分系数
    double integral;             //定义积分值
    double outMin, outMax;
}Pid;

extern void PID_param_init(Pid *pid, double kp, double ki, double kd, double outMin, double outMax);
extern void set_pid_target(Pid *pid, double temp_val);
extern double get_pid_target(Pid *pid);
extern void set_p_i_d(Pid *pid, double p, double i, double d);
extern double location_pid_realize(Pid *pid, double actual_val);
extern double speed_pid_realize(Pid *pid, double actual_val);

#ifdef __cplusplus
}
#endif

#endif
