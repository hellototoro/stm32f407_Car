#include <display/Display.hpp>
#include <led/Led.hpp>
#include <key/Key.hpp>
#include <r61509/R61509.hpp>
#include <stdio.h>
#include "mainpp.hpp"
#include "MyCar.hpp"
#include "tools/pid_tool/protocol.h"
#include "cmsis_os.h"
#include <tracking_mode/TrackingMode.hpp>

//debug
#define DEBUG_SPEED     1
#if DEBUG_SPEED
#define VALUE           getSpeed_RPM
#else
#define VALUE           getSumCounter
#endif

using namespace MyDrivers;

/* 外部变量声明 */

/* 本地变量定义&声明 */
MyCar car;
MyDrivers::Led led1(1);
MyDrivers::Led led2(2);
MyDrivers::Key key1(1);
R61509 r61509;
MyDrivers::Display display(&r61509);
uint8_t display_buffer[20];
TrackingMode tracking;
bool tracking_tag;

/* 函数声明 */
void display_fram(void);
void loop(void);

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    car.timeout_interrput(htim);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == TRACKING_LEFT_Pin || GPIO_Pin == TRACKING_RIGHT_Pin || GPIO_Pin == TRACKING_CENTRE_Pin)
    {
        tracking_tag = true;
    }
}

void setup(void)
{
    display.init();
    display.setCharSize(24);
    display.clear(MyDrivers::lcd_color::WHITE);
    display_fram();

    car.init();
    car.power_on();
    //car.moveToSpeed(80.f, 80.f);
    //car.moveToDistance(1.f, 1.f);
    //car.turn(90.f);


    //debug
    set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
    set_computer_value(SEND_STOP_CMD, CURVES_CH2, NULL, 0);    // 同步上位机的启动按钮状态
    int32_t temp = (int32_t)80.f;
    set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &temp, 1);     // 给通道 1 发送目标值
    set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &temp, 1);     // 给通道 1 发送目标值
    pid_tool_start_receive();
}

void loop(void)
{
    //do something
    if (key1.keyIsPress()) {
        led2.toggle();
        car.cmd_vel(MyWheel::rpm2Speed(100.f));
        //car.moveToSpeed(80.f, 80.f);
    }
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
extern void PID_ComputeTimerCallback(void const * argument);

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
    /* Create Timer */
    //osTimerDef(PID_ComputeTimer, PID_ComputeTimerCallback);
    //osTimerId TimerHandle = osTimerCreate(osTimer(PID_ComputeTimer), osTimerPeriodic, NULL);
    /* Start Timer */
    //osTimerStart(TimerHandle, 100);
    for(;;)
    {
        if (tracking_tag) {
            if (tracking.direction() == TrackingModeDir::left) {
                car.moveToSpeed(car.left_wheel.getSpeed_RPM() - 10, car.left_wheel.getSpeed_RPM());
            } else if (tracking.direction() == TrackingModeDir::right) {
                car.moveToSpeed(car.right_wheel.getSpeed_RPM(), car.right_wheel.getSpeed_RPM() + 10);
            } else {
                car.cmd_vel(MyWheel::rpm2Speed(car.right_wheel.getSpeed_RPM()));
                tracking_tag = false;
            }
        }
        osDelay(300);
    }
}

void StartDisplayTask(void const * argument)
{
    int32_t temp;
    for(;;)
    {
        sprintf((char*)display_buffer, "%.02f", car.left_wheel.VALUE());
        display.showString(110, 10, (char*)display_buffer);
        sprintf((char*)display_buffer, "%.02f", car.left_wheel.motor.speed_pid.output_val);
        display.showString(120, 70, (char*)display_buffer);
        temp = (int32_t)car.left_wheel.VALUE();
        set_computer_value(SEND_FACT_CMD, CURVES_CH1, &temp, 1);

        sprintf((char*)display_buffer, "%.02f", car.right_wheel.VALUE());
        display.showString(110, 40, (char*)display_buffer);
        sprintf((char*)display_buffer, "%.02f", car.right_wheel.motor.speed_pid.output_val);
        display.showString(120, 100, (char*)display_buffer);
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

void StartLoopTask(void const * argument)
{
    //MX_TouchGFX_Process();
    for(;;)
    {
        loop();
        osDelay(10);
    }
}

void StartKeyTask(void const * argument)
{
    for(;;)
    {
        key1.keyTask();
        osDelay(100);
    }
}

}
