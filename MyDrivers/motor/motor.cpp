/*
 * motor.cpp
 *
 *  Created on: Oct 24, 2021
 *      Author: huang
 */

#include <motor/motor.hpp>

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

void motor::setLocation(double location)
{
    location_pid.output_val = 0.0;
    location_pid.err = 0.0;
    location_pid.err_last = 0.0;
    location_pid.integral = 0.0;
    location_pid.target_val = location;
    location_pid_tag = true;
    speed_pid_tag = false;
}

void motor::setSpeed(double speed)
{
    speed_pid.output_val = 0.0;
    speed_pid.err = 0.0;
    speed_pid.err_last = 0.0;
    speed_pid.integral = 0.0;
    speed_pid.target_val = speed;
    speed_pid_tag = true;
    location_pid_tag = false;
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
void motor::setDutyCycle(uint16_t D)
{
    uint32_t P;    /* Pulse duration */
    uint32_t T;    /* PWM signal period */

    /* PWM signal period is determined by the value of the auto-reload register */
    T = __HAL_TIM_GET_AUTORELOAD(motor_PWM_Handle) + 1;

    /* Pulse duration is determined by the value of the compare register.       */
    /* Its value is calculated in order to match the requested duty cycle.      */
    P = (D*T)/pwm_resolution;
    __HAL_TIM_SET_COMPARE(motor_PWM_Handle, PWM_Channel, P);
}

void motor::loopTask(uint16_t period, double &mileage, double mileage_ratio)
{
    int32_t counter = encoder.getEncoderCounter() + (encoder.overflow * encoder.getPeriod());

    encoder.delta_counter = counter - encoder.last_counter;

    real_time_rpm = encoder.delta_counter * ratio(period);//(encoder.delta_counter * 1000 * 60.f) / (encoder.resolution(4) * (double)period);//转/分钟

    //encoder.sum_counter += encoder.delta_counter;
    mileage += encoder.delta_counter * mileage_ratio;

    encoder.last_counter = counter;

    if (location_pid_tag) {
        setDutyCycle(location_pid_realize(&location_pid, counter));
    }
    if (speed_pid_tag) {
        setDutyCycle(speed_pid_realize(&speed_pid, real_time_rpm));
    }
}

void motor::locationPidInit(double kp, double ki, double kd)
{
    PID_param_init(&location_pid, kp, ki, kd, 0, 255);
}

void motor::speedPidInit(double kp, double ki, double kd)
{
    PID_param_init(&speed_pid, kp, ki, kd, 0, 255);
}

} /* namespace MyDrivers */
