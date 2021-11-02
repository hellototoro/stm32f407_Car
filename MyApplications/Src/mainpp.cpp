#include <display/Display.hpp>
#include <led/Led.hpp>
#include <r61509/R61509.hpp>
#include <stdio.h>
#include "mainpp.hpp"
#include "MyCar.hpp"
#include "tools/pid_tool/protocol.h"
#include "cmsis_os.h"

//debug
#define DEBUG_SPEED 0
#if DEBUG_SPEED
#define VALUE       getSpeed_RPM
#else
#define VALUE       getSumCounter
#endif

/* 外部变量声明 */

/* 本地变量定义&声明 */
MyCar car;
MyDrivers::Led led1(1);
R61509 r61509;
MyDrivers::Display display(&r61509);
uint8_t display_buffer[20];

/* 函数声明 */
void display_fram(void);
void loop(void);

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    car.timeout_interrput(htim);
}

void setup(void)
{
    display.init();
    display.setCharSize(24);
    display.clear(MyDrivers::lcd_color::WHITE);
    display_fram();

    car.init();
    car.power_on();
    //car.moveToDistance(1080,1080);
    //car.moveToSpeed(80.f, 80.f);
    //car.moveToDistance(1.f, 1.f);
    car.turn(90);

    //debug
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
    set_computer_value(SEND_STOP_CMD, CURVES_CH2, NULL, 0);    // 同步上位机的启动按钮状态
    int32_t temp = (int32_t)1080;
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &temp, 1);     // 给通道 1 发送目标值
    set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &temp, 1);     // 给通道 1 发送目标值
    pid_tool_start_receive();
}

void loop(void)
{
    //do something
}

void display_fram(void)
{
    display.showString(10, 10, (char*)"Input_l:");
    display.showString(10, 40, (char*)"Input_r:");
    display.showString(10, 70, (char*)"Output_l:");
    display.showString(10, 100, (char*)"Output_r:");
}

/* FreeRTOS 任务入口函数（C接口函数） */
extern "C" {
extern void MXvoid_TouchGFX_Process(void);

void LEDTask(void)
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
        osDelay(1000);
    }
}

void StartDisplayTask(void const * argument)
{
    int32_t temp;
    for(;;)
    {
        sprintf((char*)display_buffer, "%.02f", car.left_wheel.VALUE());
        display.showString(110, 10, (char*)display_buffer);
        temp = (int32_t)car.left_wheel.VALUE();
        set_computer_value(SEND_FACT_CMD, CURVES_CH1, &temp, 1);

        sprintf((char*)display_buffer, "%.02f", car.right_wheel.VALUE());
        display.showString(110, 40, (char*)display_buffer);
        temp = (int32_t)car.right_wheel.VALUE();
        set_computer_value(SEND_FACT_CMD, CURVES_CH2, &temp, 1);
        osDelay(100);
    }
}

void StartUartTask(void const * argument)
{
    osSemaphoreId semaphore = (osSemaphoreId) argument;
    for(;;)
    {
        osSemaphoreWait (semaphore, osWaitForever);
        analysis_rec_data();
    }
}

void StartGUITask(void const * argument)
{
    //MX_TouchGFX_Process();
    for(;;)
    {
        osDelay(1000);
    }
}

void StartLoopTask(void const * argument)
{
    //MX_TouchGFX_Process();
    for(;;)
    {
        loop();
        osDelay(10);
    }
}

}
