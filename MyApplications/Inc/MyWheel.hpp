/*
 * MyWheel.hpp
 *
 *  Created on: Oct 25, 2021
 *      Author: huang
 */

#ifndef INC_MYWHEEL_HPP_
#define INC_MYWHEEL_HPP_

#include "main.h"
#include <motor/motor.hpp>

class MyWheel
{
public:
    double current_speed;       /* 当前轮子速度（线速度），单位：m/s */
    double target_speed;
    double mileage;             /* 当前轮子总里程，单位：m */
    MyDrivers::motor motor;

public:
    MyWheel() {}
    void init(GPIO_TypeDef* motor_port_p, uint16_t motor_pin_p, 
              GPIO_TypeDef* motor_port_n, uint16_t motor_pin_n, 
              PWM_HandleTypeDef *motor_PWM_Handle, uint32_t PWM_Channel, 
              Encoder_HandleTypeDef *Encoder_Handle) {
        motor.init(motor_port_p, motor_pin_p, motor_port_n, motor_pin_n, motor_PWM_Handle, PWM_Channel, Encoder_Handle);
    }
    void loopTask(uint16_t period) {
        motor.loopTask(period, mileage, ratio());
    }
    virtual ~MyWheel() {}

private:
    constexpr static double diameter = 0.048f;      //轮子直径，单位：m
    constexpr static double PI = 3.1415f;

private:
    constexpr double ratio() {
        return (PI * diameter) / motor.encoder.resolution(4);
    }
};


#endif /* INC_MYWHEEL_HPP_ */
