/*
 * motor.hpp
 *
 *  Created on: Oct 24, 2021
 *      Author: huang
 */

#ifndef MOTOR_MOTOR_HPP_
#define MOTOR_MOTOR_HPP_

#include <encoder/HW_Encoder.hpp>
#include "main.h"
#include "PID/fire_pid.h"

namespace MyDrivers {

class Motor {
public:
    HW_Encoder encoder;
    Pid location_pid;
    Pid speed_pid;

    enum run_direction {
        plus,
        minus
    };

public:
    Motor() {}
    void init(GPIO_TypeDef* motor_port_p, uint16_t motor_pin_p, 
              GPIO_TypeDef* motor_port_n, uint16_t motor_pin_n, 
              PWM_HandleTypeDef *motor_PWM_Handle, uint32_t PWM_Channel, 
              Encoder_HandleTypeDef *Encoder_Handle);
    void locationPID_Init(double kp, double ki, double kd);
    void speedPID_Init(double kp, double ki, double kd);
    void start(void);
    void run(run_direction direction);
    void off(void);
    void setLocation(int32_t counter);
    void setSpeed(double speed);
    double getRPM(void);
    static void period_interrput(Motor &_motor, uint16_t period, double &mileage, double mileage_ratio);
    virtual ~Motor() {}

private:
    double real_time_rpm;               /* 当前轮子转速：单位：转/秒 */
    constexpr static uint8_t pwm_resolution = 255;
    constexpr static double ratio(uint16_t period) {
        return (1000 * 60.f) / (HW_Encoder::resolution(4) * period);
    }

    GPIO_TypeDef *motor_port_p, *motor_port_n;
    uint16_t motor_pin_p, motor_pin_n;
    PWM_HandleTypeDef *PWM_Handle;
    uint32_t PWM_Channel;

    bool location_pid_tag;
    bool speed_pid_tag;
private:
    static void setDutyCycle(Motor &_motor, uint16_t D);

};

} /* namespace MyDrivers */

#endif /* MOTOR_MOTOR_HPP_ */
