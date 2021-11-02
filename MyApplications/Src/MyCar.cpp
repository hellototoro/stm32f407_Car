/*
 * MyCar.cpp
 *
 *  Created on: Oct 24, 2021
 *      Author: huang
 */

#include "MyCar.hpp"

using namespace MyDrivers;

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
                        0.158, 0.0, 0.0); //位置环pid
    right_wheel.PID_Init(5.0, 2.0, 0.0, //速度环pid
                         0.158, 0.0, 0.0);//位置环pid
    HAL_TIM_Base_Start_IT(encoder_timer);
}

void MyCar::power_on(void)
{
    left_wheel.start();
    right_wheel.start();
}

void MyCar::moveToSpeed(double speed_l, double speed_r)
{
    left_wheel.setSpeed(speed_l);
    right_wheel.setSpeed(speed_r);
}

void MyCar::moveToDistance(double distance_l, double distance_r)
{
    left_wheel.setLocation(distance_l);

    right_wheel.setLocation(distance_r);
}

void MyCar::turn(double angle, double speed)
{
    double distance = angle * ratio;

    left_wheel.setLocation(distance);

    right_wheel.setLocation(distance*(-1));
}

void MyCar::stop(void)
{
    if (left_wheel.motor.runTypeIsSpeed()) {
        left_wheel.setSpeed(0);
    } else {
        left_wheel.setLocation(0);
    }
    left_wheel.stop();

    if (right_wheel.motor.runTypeIsSpeed()) {
        right_wheel.setSpeed(0);
    } else {
        right_wheel.setLocation(0);
    }
    right_wheel.stop();
}

void MyCar::timeout_interrput(TIM_HandleTypeDef *htim)
{
    if (htim == encoder_timer) {/* @ ENCODER_TICK_FREQ_DEFAULT ms定时器 */
        Motor::period_interrput(left_wheel.motor, ENCODER_TICK_FREQ_DEFAULT, left_wheel.mileage, left_wheel.ratio(4));
        Motor::period_interrput(right_wheel.motor, ENCODER_TICK_FREQ_DEFAULT, right_wheel.mileage, right_wheel.ratio(4));
    } else if (htim == ENCODER_HANDLE(LEFT)) {
        HW_Encoder::interrput(left_wheel.motor.encoder);
    } else if (htim == ENCODER_HANDLE(RIGHT)) {
        HW_Encoder::interrput(right_wheel.motor.encoder);
    }
}

MyCar::~MyCar() {
    // TODO Auto-generated destructor stub
}

