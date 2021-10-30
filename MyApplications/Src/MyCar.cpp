/*
 * MyCar.cpp
 *
 *  Created on: Oct 24, 2021
 *      Author: huang
 */

#include "MyCar.hpp"


#define MOTOR_PORT(dir,polarity)    dir##_MOTOR_##polarity##_GPIO_Port  //dir: @ LEFT RIGHT FRONT BACK
                                                                        //polarity: @ P N
#define MOTOR_PIN(dir,polarity)     dir##_MOTOR_##polarity##_Pin        //dir: @ LEFT RIGHT FRONT BACK
                                                                        //polarity: @ P N

#define PWM_HANDLE(dir)         dir##_motor_pwm                         //dir: @ LEFT RIGHT FRONT BACK
#define LEFT_CHANNEL            TIM_CHANNEL_1
#define RIGHT_CHANNEL           TIM_CHANNEL_2
#define CHANNEL(dir)            dir##_CHANNEL

#define ENCODER_HANDLE(dir)     dir##_encoder

/* 外部变量声明 */

MyCar::MyCar() {
    // TODO Auto-generated constructor stub

}

void MyCar::init(void)
{
    left_wheel.init(MOTOR_PORT(LEFT,P), MOTOR_PIN(LEFT,P), //LEFT_MOTOR_P_GPIO_Port
                    MOTOR_PORT(LEFT,N), MOTOR_PIN(LEFT,N), 
                    PWM_HANDLE(LEFT), CHANNEL(LEFT), 
                    ENCODER_HANDLE(LEFT));
    right_wheel.init(MOTOR_PORT(RIGHT,P), MOTOR_PIN(RIGHT,P), 
                     MOTOR_PORT(RIGHT,N), MOTOR_PIN(RIGHT,N), 
                     PWM_HANDLE(RIGHT), CHANNEL(RIGHT),
                     ENCODER_HANDLE(RIGHT));
    left_wheel.PID_Init(5.0, 2.0, 0.0,  //速度环pid
                        0.0, 0.0, 0.0); //位置环pid
    right_wheel.PID_Init(5.0, 0.0013, 0.0, //速度环pid
                         0.0, 0.0, 0.0);//位置环pid
    HAL_TIM_Base_Start_IT(encoder_timer);
}

void MyCar::power_on(void)
{
    left_wheel.start();
    right_wheel.start();
}

void MyCar::move_front(double speed, double distance)
{
    if (distance > 0.1f) {
        left_wheel.setLocation(distance);
        right_wheel.setLocation(distance);
    } else {
        left_wheel.setSpeed(speed);
        right_wheel.setSpeed(speed);
    }
    left_wheel.run_front();
    right_wheel.run_front();
}

void MyCar::move_back(double speed, double distance)
{
    if (distance > 0.f) {
        left_wheel.setLocation(distance);
        right_wheel.setLocation(distance);
    } else {
        left_wheel.setSpeed(speed);
        right_wheel.setSpeed(speed);
    }
    left_wheel.run_back();
    right_wheel.run_back();
}

void MyCar::turn_left(double speed, double angle)
{
    double distance = angle * ratio;

    left_wheel.setLocation(distance);
    left_wheel.run_front();

    right_wheel.setLocation(distance);
    right_wheel.run_back();
}

void MyCar::turn_right(double speed, double angle)
{
    double distance = angle * ratio;

    left_wheel.setLocation(distance);
    left_wheel.run_back();

    right_wheel.setLocation(distance);
    right_wheel.run_front();
}

void MyCar::stop(void)
{
    left_wheel.stop();
    right_wheel.stop();
}

void MyCar::timeout_interrput(TIM_HandleTypeDef *htim)
{
    if (htim == encoder_timer) {/* @ ENCODER_TICK_FREQ_DEFAULT ms定时器 */
        MyWheel::period_interrput(left_wheel, ENCODER_TICK_FREQ_DEFAULT);
        MyWheel::period_interrput(right_wheel, ENCODER_TICK_FREQ_DEFAULT);
    } else if (htim == ENCODER_HANDLE(LEFT)) {
        MyDrivers::hw_encoder::interrput(left_wheel.motor.encoder);
    } else if (htim == ENCODER_HANDLE(RIGHT)) {
        MyDrivers::hw_encoder::interrput(right_wheel.motor.encoder);
    }
}

MyCar::~MyCar() {
    // TODO Auto-generated destructor stub
}

