#include <stdio.h>
#include "mainpp.hpp"
#include "led/led.hpp"
#include "MyCar.hpp"
//#include "PID/PID_v1.h"
#include "tools/pid_tool/protocol.h"
#include "r61509/r61509.hpp"
#include "display/display.hpp"
#include "cmsis_os.h"
//#include "app_touchgfx.h"

/* 外部变量声明 */

/* 本地变量定义&声明 */
MyCar car;
MyDrivers::led led1(1);
r61509 r61509;
MyDrivers::display tft_display(&r61509);
uint8_t display_buffer[20];

/* PID相关变量 */
//Define Variables we'll be connecting to
//double Setpoint_l, Input_l, Output_l;
//double Setpoint_r, Input_r, Output_r;
//Specify the links and initial tuning parameters
//double Kp=2, Ki=5, Kd=1;
//double Kp_l=2.567, Ki_l=7.500, Kd_l=0.001;//150 ~ 200 rpm
//double Kp_r=0.017, Ki_r=5.5, Kd_r=0.001;//150 ~ 200 rpm
//PID Motor_PID_l(&Input_l, &Output_l, &Setpoint_l, Kp_l, Ki_l, Kd_l, DIRECT);
//PID Motor_PID_r(&Input_r, &Output_r, &Setpoint_r, Kp_r, Ki_r, Kd_r, DIRECT);

/* 函数声明 */
void display_fram(void);
void led_task(void);
//void motor_pid_control(void);


void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    car.timeout_interrput(htim);
}

void setup(void)
{
    tft_display.init();
    tft_display.setCharSize(24);
    tft_display.clear(MyDrivers::lcd_color::WHITE);
    display_fram();

    car.init();
    car.power_on();
    //car.move_front(80.f);

    //turn the PID on
    //Setpoint_l = 80.f;
    //Setpoint_r = 80.f;
    //Motor_PID_l.SetMode(AUTOMATIC);
    //Motor_PID_r.SetMode(AUTOMATIC);
    //Motor_PID_l.SetOutputLimits(0, 255);
    //Motor_PID_r.SetOutputLimits(0, 255);
    //Motor_PID_l.SetSampleTime(50);
    //Motor_PID_r.SetSampleTime(50);

    //debug
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
    set_computer_value(SEND_STOP_CMD, CURVES_CH2, NULL, 0);    // 同步上位机的启动按钮状态
    int32_t temp = (int32_t)car.left_wheel.getTargetValue();
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &temp, 1);     // 给通道 1 发送目标值
    set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &temp, 1);     // 给通道 1 发送目标值
    pid_tool_start_receive();
}

void loop(void)
{
    /*int32_t temp;
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
        tft_display.showString(120, 70, (char*)display_buffer);
    }

    if (Motor_PID_r.Compute() == true) {
        Motor_PID_Input(wheel_right, Output_r);
        sprintf((char*)display_buffer, "%.02f", Output_r);
        tft_display.showString(120, 100, (char*)display_buffer);
    }*/
    /* 接收数据处理 */
    //debug
    //analysis_rec_data();


    int32_t temp;
    sprintf((char*)display_buffer, "%.02f", car.left_wheel.getSpeed_RPM());
    tft_display.showString(110, 10, (char*)display_buffer);
    temp = (int32_t)car.left_wheel.motor.getRPM();
    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &temp, 1);
    led_task();
    //debug
    analysis_rec_data();
}

void display_fram(void)
{
    tft_display.showString(10, 10, (char*)"Input_l:");
    tft_display.showString(10, 40, (char*)"Input_r:");
    tft_display.showString(10, 70, (char*)"Output_l:");
    tft_display.showString(10, 100, (char*)"Output_r:");
}

void set_target_speed(Motor_WheelType wheel, double speed)
{
    (wheel == wheel_left) ? car.left_wheel.setSpeed(speed) : car.right_wheel.setSpeed(speed);
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

/* FreeRTOS 任务入口函数（C接口函数） */
extern "C" {
extern void MX_TouchGFX_Process(void);

void StartLEDTask(void const * argument)
{
    for(;;)
    {
        led1.toggle();
        osDelay(1000);
    }
}

void StartCarTask(void const * argument)
{
    for(;;)
    {
        //car.left_wheel.loopTask(50);
        //car.right_wheel.loopTask(50);
        osDelay(1000);
    }
}

void StartDisplayTask(void const * argument)
{
    int32_t temp;
    for(;;)
    {
        sprintf((char*)display_buffer, "%.02f", car.left_wheel.getSpeed_RPM());
        tft_display.showString(110, 10, (char*)display_buffer);
        temp = (int32_t)car.left_wheel.motor.getRPM();
        set_computer_value(SEND_FACT_CMD, CURVES_CH1, &temp, 1);

        sprintf((char*)display_buffer, "%.02f", car.right_wheel.getSpeed_RPM());
        tft_display.showString(110, 40, (char*)display_buffer);
        temp = (int32_t)car.right_wheel.getSpeed_RPM();
        set_computer_value(SEND_FACT_CMD, CURVES_CH2, &temp, 1);
        osDelay(100);
    }
}

void StartUartTask(void const * argument)
{
    osSemaphoreId semaphore = (osSemaphoreId) argument;
    for(;;)
    {
        if (semaphore != NULL) {
            osSemaphoreWait (semaphore, osWaitForever);
            analysis_rec_data();
        } else {
        	osDelay(100);
        }
    }
}

void StartGUITask(void const * argument)
{
    //MX_TouchGFX_Process();
    for(;;)
    {
        osDelay(10);
    }
}

}
