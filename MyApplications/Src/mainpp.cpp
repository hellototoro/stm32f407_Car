#include <stdio.h>
#include "mainpp.hpp"
#include "led/led.hpp"
#include "oled/oled.hpp"
#include "delay.h"
#include "encoder/encoder.h"
#include "motor_control.h"
#include "PID/PID_v1.h"
#include "tools/pid_tool/protocol.h"
#include "s6d04d1/s6d04d1.hpp"
#include "r61509/r61509.hpp"
#include "display/display.hpp"

MyDrivers::led led1(1);

//oled display;
//s6d04d1 s6d0;
r61509 r61509;
MyDrivers::display tft_display(&r61509);

Encoder_HandleTypeDef *encoders[] = {LEFT_encoder, RIGHT_encoder};
uint8_t display_buffer[20];

/* PID相关变量 */
//Define Variables we'll be connecting to
double Setpoint_l, Input_l, Output_l;
double Setpoint_r, Input_r, Output_r;

//Specify the links and initial tuning parameters
//double Kp=2, Ki=5, Kd=1;
double Kp_l=2.567, Ki_l=7.500, Kd_l=0.001;//150 ~ 200 rpm
double Kp_r=0.017, Ki_r=5.5, Kd_r=0.001;//150 ~ 200 rpm
PID Motor_PID_l(&Input_l, &Output_l, &Setpoint_l, Kp_l, Ki_l, Kd_l, DIRECT);
PID Motor_PID_r(&Input_r, &Output_r, &Setpoint_r, Kp_r, Ki_r, Kd_r, DIRECT);

/* 外部变量声明 */
extern motor_info motor;


/* 函数声明 */
void display_fram(void);
void led_task(void);
void motor_pid_control(void);


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //delay_interrupt_callback(htim);
    if (htim == encoder_timer) {/* @ TIM6_TICK_FREQ_DEFAULT ms定时器 */
        Motor_encoderTask();
    } else if (htim == LEFT_encoder) {
        updateEncoderCounter(LEFT_encoder, &motor.left_encoder);
    } else if (htim == RIGHT_encoder) {
        updateEncoderCounter(RIGHT_encoder, &motor.right_encoder);
    }
}

void setup(void)
{
    //delay_start();
    tft_display.init();
    tft_display.setCharSize(24);
    tft_display.clear(MyDrivers::lcd_color::WHITE);
    display_fram();

    startEncoder(encoders, 2);

    Motor_init();
    //Motor_move(FRONT);
    //turn the PID on
    Setpoint_l = 50.f;
    Setpoint_r = 50.f;
    Motor_PID_l.SetMode(AUTOMATIC);
    Motor_PID_r.SetMode(AUTOMATIC);
    Motor_PID_l.SetOutputLimits(0, 255);
    Motor_PID_r.SetOutputLimits(0, 255);
    Motor_PID_l.SetSampleTime(50);
    Motor_PID_r.SetSampleTime(50);

    //debug
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
    set_computer_value(SEND_STOP_CMD, CURVES_CH2, NULL, 0);    // 同步上位机的启动按钮状态
    int32_t temp = (int32_t)Setpoint_l;
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &temp, 1);     // 给通道 1 发送目标值
    set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &temp, 1);     // 给通道 1 发送目标值
    pid_tool_start_receive();
}

void loop(void)
{
	int32_t temp;
    Input_l = getSpeed_RPM(wheel_left);
    sprintf((char*)display_buffer, "%.02f", Input_l);
    tft_display.showString(110, 10, (char*)display_buffer);
    temp = (int32_t)Input_l;
    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &temp, 1);

    Input_r = getSpeed_RPM(wheel_right);
    sprintf((char*)display_buffer, "%.02f", Input_r);
    tft_display.showString(110, 40, (char*)display_buffer);
    temp = (int32_t)Input_r;
    set_computer_value(SEND_FACT_CMD, CURVES_CH2, &temp, 1);

    if (Motor_PID_l.Compute() == true) {
        Motor_PID_Input(wheel_left, Output_l);
        sprintf((char*)display_buffer, "%.02f", Output_l);
        tft_display.showString(120, 100, (char*)display_buffer);
    }

    if (Motor_PID_r.Compute() == true) {
        Motor_PID_Input(wheel_right, Output_r);
        sprintf((char*)display_buffer, "%.02f", Output_r);
        tft_display.showString(120, 70, (char*)display_buffer);
    }
    /* 接收数据处理 */
    //debug
    analysis_rec_data();
    led_task();
}

void display_fram(void)
{
	tft_display.showString(10, 10, (char*)"Input_l:");
	tft_display.showString(10, 40, (char*)"Input_r:");
	tft_display.showString(10, 100, (char*)"Output_l:");
	tft_display.showString(10, 70, (char*)"Output_r:");
}

void set_target_speed(Motor_WheelType wheel, double speed)
{
    (wheel == wheel_left) ? Setpoint_l = speed : Setpoint_r = speed;
}

void led_task(void)
{
    static uint32_t tick = 0;
    uint32_t now = HAL_GetTick();
    if( (now - tick) >= 1000) {
        led1.toggle();
        tick = now;
    }
}
