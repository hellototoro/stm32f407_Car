/**
  ******************************************************************************
  * @file    motor_control.h
  * @brief
  * @data    2021-7-28
  * @author  jee
  ******************************************************************************
  */
#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#include <encoder/encoder.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 宏 */
//电机移动方向
#define Motor_move(dir) Motor_move##dir()   //dir: @ RIGHT LEFT FRONT BACK STOP

typedef enum {
    Dir_front = 0,
    Dir_back,
    Dir_right,
    Dir_left
}Motor_MoveDirTypeDef;

typedef struct {
    //double set_speed;
    //double set_rpm;
    double current_speed;       /* 当前轮子速度（线速度），单位：m/s */
    double current_rpm;         /* 当前轮子转速：单位：转/秒 */
    int32_t mileage;    /* 当前轮子总里程，单位：m */
}wheel_info;

typedef struct {
    encoder_info left_encoder,right_encoder;
    wheel_info left_wheel,right_wheel;
}motor_info;

/* 提供的函数接口 */
extern void Motor_init(void);
extern void Motor_moveFRONT(void);
extern void Motor_moveRIGHT(void);
extern void Motor_moveBACK(void);
extern void Motor_moveLEFT(void);
extern void Motor_moveSTOP(void);
extern void Motor_lock(void);
extern void Motor_start(void);
extern void Motor_encoderTask(void);
extern double getSpeed(Motor_WheelType wheel);
extern double getSpeed_RPM(Motor_WheelType wheel);
extern double getMileage(Motor_WheelType wheel);
extern void Motor_PID_Input(Motor_WheelType wheel, uint16_t input);

#ifdef __cplusplus
}
#endif

#endif
