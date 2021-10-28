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

/* 外部变量声明 */

MyCar::MyCar() {
    // TODO Auto-generated constructor stub

}

void MyCar::init(void)
{
    left_wheel.init(MOTOR_PORT(LEFT,P), MOTOR_PIN(LEFT,P), //LEFT_MOTOR_P_GPIO_Port
                    MOTOR_PORT(LEFT,N), MOTOR_PIN(LEFT,N), 
                    PWM_HANDLE(LEFT), LEFT_CHANNEL, 
                    LEFT_encoder);
    right_wheel.init(MOTOR_PORT(RIGHT,P), MOTOR_PIN(RIGHT,P), 
                     MOTOR_PORT(RIGHT,N), MOTOR_PIN(RIGHT,N), 
                     PWM_HANDLE(RIGHT), RIGHT_CHANNEL,
                     RIGHT_encoder);
    HAL_TIM_Base_Start_IT(encoder_timer);
}

void MyCar::power_on(void)
{
    left_wheel.motor.start();
    right_wheel.motor.start();
}

void MyCar::move_front(double speed, double distance)
{
    left_wheel.motor.setSpeed(speed);
    left_wheel.motor.run(MyDrivers::motor::plus);

    right_wheel.motor.setSpeed(speed);
    right_wheel.motor.run(MyDrivers::motor::plus);
}

void MyCar::move_back(double speed, double distance)
{
    left_wheel.current_speed = speed;
    //left_wheel.motor.setDutyCycle(0);
    left_wheel.motor.run(MyDrivers::motor::minus);

    right_wheel.current_speed = speed;
    //right_wheel.motor.setDutyCycle(0);
    right_wheel.motor.run(MyDrivers::motor::minus);
}

void MyCar::turn_left(double speed, double angle)
{
    left_wheel.current_speed = speed;
    //left_wheel.motor.setDutyCycle(0);
    left_wheel.motor.run(MyDrivers::motor::plus);

    right_wheel.current_speed = speed;
    //right_wheel.motor.setDutyCycle(0);
    right_wheel.motor.run(MyDrivers::motor::minus);
}

void MyCar::turn_right(double speed, double angle)
{
    left_wheel.current_speed = speed;
    //left_wheel.motor.setDutyCycle(0);
    left_wheel.motor.run(MyDrivers::motor::minus);

    right_wheel.current_speed = speed;
    //right_wheel.motor.setDutyCycle(0);
    right_wheel.motor.run(MyDrivers::motor::plus);
}

void MyCar::stop(void)
{
    //left_wheel.motor.setDutyCycle(0);
    left_wheel.motor.off();

    //right_wheel.motor.setDutyCycle(0);
    right_wheel.motor.off();
}


MyCar::~MyCar() {
    // TODO Auto-generated destructor stub
}

