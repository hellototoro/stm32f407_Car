/*
 * motor.hpp
 *
 *  Created on: Oct 24, 2021
 *      Author: huang
 */

#ifndef MOTOR_MOTOR_HPP_
#define MOTOR_MOTOR_HPP_

#define USE_ARDUINO_PID         0
#define USE_ABSOLUTE_LOCATION   0
#define RPM_TIME_USE_S          0

#include <encoder/HW_Encoder.hpp>
#include "main.h"
#include "PID/fire_pid.h"
#if USE_ARDUINO_PID
#include "PID/PID_v1.h"
#endif

namespace MyDrivers {

class Motor {
public:
    HW_Encoder encoder;
    Pid location_pid;
    Pid speed_pid;
    #if USE_ARDUINO_PID
    PID pid;
    double Setpoint, Input, Output;
    double Kp = 2, Ki = 5, Kd = 1;
    #endif

    enum run_direction {
        plus,
        minus,
        none
    };

public:
    Motor();
    void init(GPIO_TypeDef* motor_port_p, uint16_t motor_pin_p, 
              GPIO_TypeDef* motor_port_n, uint16_t motor_pin_n, 
              PWM_HandleTypeDef *motor_PWM_Handle, uint32_t PWM_Channel, 
              Encoder_HandleTypeDef *Encoder_Handle);
    void locationPID_Init(double kp, double ki, double kd);
    void speedPID_Init(double kp, double ki, double kd);
    #if USE_ARDUINO_PID
    void PID_Init(void);
    #endif
    void start(void);
    void run(run_direction direction);
    void setMoveDirection(double value);
    void off(void);
    void resetLocation_PID(void);
    void resetSpeed_PID(void);
    void setLocation(int32_t counter);
    void setSpeed(double speed);
    double getRPM(void);
    bool runTypeIsSpeed(void);
    bool runTypeIsDistance(void);
    static void monitorMaxSpeed(Motor &motor);
    bool isMaxSpeed(void);
    static void period_interrput(Motor &_motor, uint16_t period, double &mileage, double mileage_ratio);
    static void pid_cal(Motor &motor);
    virtual ~Motor() {}

private:
    double real_time_rpm;               /* 当前轮子转速：单位：转/秒 */
    constexpr static uint16_t pwm_resolution = 255;
    constexpr static double rpm_ratio(uint16_t period) {
        #if RPM_TIME_USE_S
        return (1000.f / (HW_Encoder::resolution(4) * (double)period));
        #else
        return (1000 * 60.f) / (HW_Encoder::resolution(4) * (double)period);
        #endif
    }

    GPIO_TypeDef *motor_port_p, *motor_port_n;
    uint16_t motor_pin_p, motor_pin_n;
    PWM_HandleTypeDef *PWM_Handle;
    uint32_t PWM_Channel;

    run_direction direction;
    bool location_pid_tag;
    bool speed_pid_tag;
    bool maxSpeed_tag;
    uint16_t time_counter;
private:
    static void setDutyCycle(Motor &motor, uint16_t D);

};

} /* namespace MyDrivers */

#endif /* MOTOR_MOTOR_HPP_ */
