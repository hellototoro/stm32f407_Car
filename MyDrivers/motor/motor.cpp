/*
 * motor.cpp
 *
 *  Created on: Oct 24, 2021
 *      Author: huang
 */

#include <math.h>
#include <motor/Motor.hpp>

namespace MyDrivers {

Motor::Motor() :
direction { none }, location_pid_tag { false }, speed_pid_tag {false}, maxSpeed_tag { false }, 
time_counter { 0 }

{

}


void Motor::init(GPIO_TypeDef* motor_port_p, uint16_t motor_pin_p, 
                 GPIO_TypeDef* motor_port_n, uint16_t motor_pin_n, 
                 PWM_HandleTypeDef *PWM_Handle, uint32_t PWM_Channel, 
                 Encoder_HandleTypeDef *Encoder_Handle)
{
    this->motor_port_p = motor_port_p;
    this->motor_pin_p = motor_pin_p;

    this->motor_port_n = motor_port_n;
    this->motor_pin_n = motor_pin_n;

    this->PWM_Handle = PWM_Handle;
    this->PWM_Channel = PWM_Channel;
    encoder.init(Encoder_Handle);
}

void Motor::start(void)
{
    encoder.start();
    HAL_TIM_PWM_Start(PWM_Handle, PWM_Channel);
}

void Motor::run(run_direction direction)
{
    bool pin_state = (direction == plus) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    this->direction = direction;
    HAL_GPIO_WritePin(motor_port_p, motor_pin_p, static_cast<GPIO_PinState>(pin_state));
    HAL_GPIO_WritePin(motor_port_n, motor_pin_n, static_cast<GPIO_PinState>(!pin_state));
}

void Motor::setMoveDirection(double value)
{
    (value > 0) ? 
    ((direction != plus)  ? run(plus)  : (void)value) :
    ((direction != minus) ? run(minus) : (void)value);
}

void Motor::off(void)
{
    direction = none;
    HAL_GPIO_WritePin(motor_port_p, motor_pin_p, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor_port_n, motor_pin_n, GPIO_PIN_RESET);
}

void Motor::resetLocation_PID(void)
{
    location_pid_tag = false;
    clear_pid_status(&location_pid);
}

void Motor::resetSpeed_PID(void)
{
    speed_pid_tag = false;
    clear_pid_status(&speed_pid);
}


void Motor::setLocation(int32_t counter)
{
    if (speed_pid_tag) {
        speed_pid_tag = false;
        location_pid.output_val = 0.0;
        location_pid.err = 0.0;
        location_pid.err_last = 0.0;
        location_pid.integral = 0.0;
    }
    location_pid_tag = true;
    #if USE_ABSOLUTE_LOCATION
    location_pid.target_val = counter;
    setMoveDirection(location_pid.target_val - encoder.sum_counter);
    #else
    location_pid.target_val = counter + encoder.sum_counter;
    setMoveDirection(counter);
    #endif
}

void Motor::setSpeed(double speed)
{
    if (location_pid_tag) {
        location_pid_tag = false;
        speed_pid.output_val = 0.0;
        speed_pid.err = 0.0;
        speed_pid.err_last = 0.0;
        speed_pid.integral = 0.0;
    }
    speed_pid_tag = true;
    speed_pid.target_val = speed;
    setMoveDirection(speed);
}

double Motor::getRPM(void)
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
void Motor::setDutyCycle(Motor &motor, uint16_t D)
{
    uint32_t P;    /* Pulse duration */
    uint32_t T;    /* PWM signal period */

    /* PWM signal period is determined by the value of the auto-reload register */
    T = __HAL_TIM_GET_AUTORELOAD(motor.PWM_Handle) + 1;

    /* Pulse duration is determined by the value of the compare register.       */
    /* Its value is calculated in order to match the requested duty cycle.      */
    P = (D*T)/pwm_resolution;
    __HAL_TIM_SET_COMPARE(motor.PWM_Handle, motor.PWM_Channel, P);
}

void Motor::period_interrput(Motor &motor, uint16_t period, double &mileage, double mileage_ratio)
{
    motor.encoder.sum_counter = motor.encoder.getEncoderCounter() + 
                                (motor.encoder.overflow * motor.encoder.getPeriod());

    motor.encoder.delta_counter = motor.encoder.sum_counter - motor.encoder.last_counter;

    motor.real_time_rpm = motor.encoder.delta_counter * rpm_ratio(period);//(encoder.delta_counter * 1000 * 60.f) / (encoder.resolution(4) * (double)period);//转/分钟

    mileage += motor.encoder.delta_counter * mileage_ratio;

    motor.encoder.last_counter = motor.encoder.sum_counter;

    pid_cal(motor);
}

void Motor::pid_cal(Motor &motor)
{
    if (motor.location_pid_tag) {
        location_pid_realize(&motor.location_pid, motor.encoder.sum_counter);
        motor.setMoveDirection(motor.location_pid.output_val);
        setDutyCycle(motor, abs(motor.location_pid.output_val));
    }
    if (motor.speed_pid_tag) {
        speed_pid_realize(&motor.speed_pid, motor.real_time_rpm);
        //motor.setMoveDirection(motor.speed_pid.output_val);
        setDutyCycle(motor, abs(motor.speed_pid.output_val));
    }
    monitorMaxSpeed(motor);
}

void Motor::monitorMaxSpeed(Motor &motor)
{
    if (motor.pwm_resolution ==static_cast<uint16_t>(abs(motor.speed_pid.output_val)) ) {
        if ( ++motor.time_counter > 2 ) {
            motor.maxSpeed_tag = true;
        }
    } else {
        motor.time_counter = 0;
    }
}

void Motor::locationPID_Init(double kp, double ki, double kd)
{
    PID_param_init(&location_pid, kp, ki, kd, -255, 255);
}

void Motor::speedPID_Init(double kp, double ki, double kd)
{
    PID_param_init(&speed_pid, kp, ki, kd, -255, 255);
}

bool Motor::runTypeIsSpeed(void)
{
    return speed_pid_tag;
}

bool Motor::runTypeIsDistance(void)
{
    return location_pid_tag;
}

bool Motor::isMaxSpeed(void)
{
    return maxSpeed_tag;
}

#if USE_ARDUINO_PID
void Motor::PID_Init(void)
{
    pid.init(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
    pid.SetMode(AUTOMATIC);
    pid.SetOutputLimits(-255, 255);
    pid.SetSampleTime(50);
}
#endif

} /* namespace MyDrivers */
