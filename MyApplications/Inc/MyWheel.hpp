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
#include <math.h>

class MyWheel
{
public:
    //double current_speed;       /* 当前轮子速度（线速度），单位：m/s */
    double target_value;
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

    void PID_Init(double s_kp, double s_ki, double s_kd, 
                  double l_kp, double l_ki, double l_kd) {
        motor.speedPID_Init(s_kp, s_ki, s_kd);
        motor.locationPID_Init(l_kp, l_ki, l_kd);
    }

    void start(void) {
        motor.start();
    }

    void run_front(void) {
        motor.run(MyDrivers::motor::plus);
    }

    void run_back(void) {
        motor.run(MyDrivers::motor::minus);
    }

    void stop(void) {
        motor.off();
    }

    void setSpeed(double speed) {
        target_value = speed;
        motor.setSpeed(speed);
    }

    void setLocation(double distance) {
        double counter = distance / ratio();
        target_value = counter;
        motor.setLocation((int32_t)counter);
    }

    double getSpeed_RPM(void) {
        return motor.getRPM();
    }

    double getTargetValue(void) {
        return target_value;
    }

    static void period_interrput(MyWheel &wheel, uint16_t period) {
        wheel.motor.period_interrput(wheel.motor, period, wheel.mileage, ratio());
    }
    
    virtual ~MyWheel() {}

private:
    constexpr static double diameter = 0.048f;      //轮子直径，单位：m

private:
    constexpr static double ratio() {
        return (M_PI * diameter) / MyDrivers::hw_encoder::resolution(4);
    }
};


#endif /* INC_MYWHEEL_HPP_ */
