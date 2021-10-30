/*
 * motor.cpp
 *
 *  Created on: Oct 24, 2021
 *      Author: huang
 */

#include <motor/motor.hpp>
#include <math.h>

namespace MyDrivers {

void motor::init(GPIO_TypeDef* motor_port_p, uint16_t motor_pin_p, 
                 GPIO_TypeDef* motor_port_n, uint16_t motor_pin_n, 
                 PWM_HandleTypeDef *motor_PWM_Handle, uint32_t PWM_Channel, 
                 Encoder_HandleTypeDef *Encoder_Handle)
{
    this->motor_port_p = motor_port_p;
    this->motor_pin_p = motor_pin_p;

    this->motor_port_n = motor_port_n;
    this->motor_pin_n = motor_pin_n;

    this->motor_PWM_Handle = motor_PWM_Handle;
    this->PWM_Channel = PWM_Channel;
    encoder.init(Encoder_Handle);
}

void motor::start(void)
{
    encoder.start();
    HAL_TIM_PWM_Start(motor_PWM_Handle, PWM_Channel);
}

void motor::run(run_direction direction)
{
    bool pin_state = (direction == plus) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(motor_port_p, motor_pin_p, static_cast<GPIO_PinState>(pin_state));
    HAL_GPIO_WritePin(motor_port_n, motor_pin_n, static_cast<GPIO_PinState>(!pin_state));
}

void motor::off(void)
{
    HAL_GPIO_WritePin(motor_port_p, motor_pin_p, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor_port_n, motor_pin_n, GPIO_PIN_RESET);
}

void motor::setLocation(int32_t counter)
{
    if (speed_pid_tag) {
        speed_pid_tag = false;
        location_pid.output_val = 0.0;
        location_pid.err = 0.0;
        location_pid.err_last = 0.0;
        location_pid.integral = 0.0;
    }
    location_pid.target_val = counter + abs(encoder.sum_counter);
    location_pid_tag = true;
}

void motor::setSpeed(double speed)
{
    if (location_pid_tag) {
        location_pid_tag = false;
        speed_pid.output_val = 0.0;
        speed_pid.err = 0.0;
        speed_pid.err_last = 0.0;
        speed_pid.integral = 0.0;
    }
    speed_pid.target_val = speed;
    speed_pid_tag = true;
}

double motor::getRPM(void)
{
    return real_time_rpm;
}

/**
  * @brief  Changes the duty cycle of the PWM signal.
  *         D = (T/P)*PWM_RESOLUTION
  *           where T is the pulse duration and P is the PWM signal period
  * @param  D Duty cycle
  *         from 0 to 255
  * @retval None
  */
void motor::setDutyCycle(motor &_motor, uint16_t D)
{
    uint32_t P;    /* Pulse duration */
    uint32_t T;    /* PWM signal period */

    /* PWM signal period is determined by the value of the auto-reload register */
    T = __HAL_TIM_GET_AUTORELOAD(_motor.motor_PWM_Handle) + 1;

    /* Pulse duration is determined by the value of the compare register.       */
    /* Its value is calculated in order to match the requested duty cycle.      */
    P = (D*T)/pwm_resolution;
    __HAL_TIM_SET_COMPARE(_motor.motor_PWM_Handle, _motor.PWM_Channel, P);
}

void motor::period_interrput(motor &_motor, uint16_t period, double &mileage, double mileage_ratio)
{
    _motor.encoder.sum_counter = _motor.encoder.getEncoderCounter() + (_motor.encoder.overflow * _motor.encoder.getPeriod());

    _motor.encoder.delta_counter = _motor.encoder.sum_counter - _motor.encoder.last_counter;

    _motor.real_time_rpm = _motor.encoder.delta_counter * ratio(period);//(encoder.delta_counter * 1000 * 60.f) / (encoder.resolution(4) * (double)period);//转/分钟

    //encoder.sum_counter += encoder.delta_counter;
    mileage += _motor.encoder.delta_counter * mileage_ratio;

    _motor.encoder.last_counter = _motor.encoder.sum_counter;

    if (_motor.location_pid_tag) {
        setDutyCycle(_motor, location_pid_realize(&_motor.location_pid, abs(_motor.encoder.sum_counter)));
    }
    if (_motor.speed_pid_tag) {
        setDutyCycle(_motor, speed_pid_realize(&_motor.speed_pid, abs(_motor.real_time_rpm)));
    }
}

void motor::locationPID_Init(double kp, double ki, double kd)
{
    PID_param_init(&location_pid, kp, ki, kd, 0, 255);
}

void motor::speedPID_Init(double kp, double ki, double kd)
{
    PID_param_init(&speed_pid, kp, ki, kd, 0, 255);
}

} /* namespace MyDrivers */
