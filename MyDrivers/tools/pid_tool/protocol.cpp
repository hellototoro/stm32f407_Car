/**
  ******************************************************************************
  * @file    protocol.c
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   野火PID调试助手通讯协议解析
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include <string.h>
#include "tools/pid_tool/protocol.h"
#include "mainpp.hpp"
#include "MyCar.hpp"
//#include "motor_control.h"
//#include "PID/PID_v1.h"
#include "PID/fire_pid.h"

extern MyCar car;

static uint8_t recv_buf[PROT_FRAME_RECV_MAX_LEN];
static uint8_t recv_data[PROT_FRAME_RECV_MAX_LEN];
static uint8_t received_flag;

/**
  * @brief 计算校验和
  * @param ptr：需要计算的数据
  * @param len：需要计算的长度
  * @retval 校验和
  */
uint8_t check_sum(uint8_t init, uint8_t *ptr, uint8_t len )
{
  uint8_t sum = init;
  
  while(len--)
  {
    sum += *ptr;
    ptr++;
  }
  
  return sum;
}

/**
  * @brief 设置上位机的值
  * @param cmd：命令
  * @param ch: 曲线通道
  * @param data：参数指针
  * @param num：参数个数
  * @retval 无
  */
void set_computer_value(uint8_t cmd, uint8_t ch, void *data, uint8_t num)
{
  uint8_t sum = 0;    // 校验和
  num *= 4;           // 一个参数 4 个字节
  
  static packet_head_t set_packet;
  
  set_packet.head = FRAME_HEADER;     // 包头 0x59485A53
  set_packet.len  = 0x0B + num;      // 包长
  set_packet.ch   = ch;              // 设置通道
  set_packet.cmd  = cmd;             // 设置命令
  
  sum = check_sum(0, (uint8_t *)&set_packet, sizeof(set_packet));       // 计算包头校验和
  sum = check_sum(sum, (uint8_t *)data, num);                           // 计算参数校验和
  
  HAL_UART_Transmit(UartHandle, (uint8_t *)&set_packet, sizeof(set_packet), 0xFFFFF);    // 发送数据头
  HAL_UART_Transmit(UartHandle, (uint8_t *)data, num, 0xFFFFF);                          // 发送参数
  HAL_UART_Transmit(UartHandle, (uint8_t *)&sum, sizeof(sum), 0xFFFFF);                  // 发送校验和
}


static uint8_t find_header(uint8_t *buf)
{
    return (((buf[0] <<  0) | (buf[1] <<  8) | (buf[2] << 16) | (buf[3] << 24)) == FRAME_HEADER) ? \
    1 : 0;
}

void analysis_rec_data(void)
{
    if (received_flag) {
        if (check_sum(0, recv_data, recv_data[LEN_INDEX_VAL]-1) == recv_data[recv_data[LEN_INDEX_VAL] -1]) {
            switch (recv_data[CMD_INDEX_VAL])
            {
                case CMD_NONE:
                {
                    //return -1;
                    break;
                }

                case SET_P_I_D_CMD:
                {
                    uint32_t temp0 = COMPOUND_32BIT(&recv_data[13]);
                    uint32_t temp1 = COMPOUND_32BIT(&recv_data[17]);
                    uint32_t temp2 = COMPOUND_32BIT(&recv_data[21]);
                    
                    float p_temp, i_temp, d_temp;
                    
                    p_temp = *(float *)&temp0;
                    i_temp = *(float *)&temp1;
                    d_temp = *(float *)&temp2;
                    
                    //set_p_i_d(recv_data[CHX_INDEX_VAL],p_temp, i_temp, d_temp);    // 设置 P I D
                    set_p_i_d(&car.left_wheel.motor.speed_pid , p_temp, i_temp, d_temp);
                }
                break;

                case SET_TARGET_CMD:
                {
                    int actual_temp = COMPOUND_32BIT(&recv_data[13]);    // 得到数据
                    //set_pid_target(recv_data[CHX_INDEX_VAL],actual_temp);    // 设置目标值
                    set_pid_target(&car.left_wheel.motor.speed_pid, actual_temp);
                }
                break;
                
                case START_CMD:
                {
                    //Motor_move(FRONT);              // 启动电机
                    car.move_front(80.f);
                }
                break;
                
                case STOP_CMD:
                {
                    //Motor_move(STOP);              // 停止电机
                    car.stop();
                }
                break;
                
                case RESET_CMD:
                {
                    HAL_NVIC_SystemReset();          // 复位系统
                }
                break;
                
                case SET_PERIOD_CMD:
                {
                    //uint32_t temp = COMPOUND_32BIT(&recv_data[13]);     // 周期数
                    //set_period(recv_data[CHX_INDEX_VAL],temp);
                }
                break;

                default: 
                    //return -1;
                    break;
            }
        }
        received_flag = 0;
    }
}


void pid_tool_start_receive(void)
{
    HAL_UART_Receive_IT(UartHandle, recv_buf, PROT_FRAME_RECV_MIN_LEN);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t continus_flag = 0;
    if (huart == UartHandle) {
        if (continus_flag) {
            continus_flag = 0;
            received_flag = 1;
            memcpy(recv_data, recv_buf, recv_buf[LEN_INDEX_VAL]);
            pid_tool_start_receive();
            return;
        }
        if (find_header(recv_buf)) {
            uint8_t receive_len = recv_buf[LEN_INDEX_VAL];
            if (receive_len > PROT_FRAME_RECV_MIN_LEN) {
                //还要继续接收
                HAL_UART_Receive_IT(UartHandle, &recv_buf[PROT_FRAME_RECV_MIN_LEN], receive_len - PROT_FRAME_RECV_MIN_LEN);
                continus_flag = 1;
            } else {
                received_flag = 1;
                memcpy(recv_data, recv_buf, receive_len);
                pid_tool_start_receive();
            }
        } else {
            pid_tool_start_receive();
        }
    }
}



/* 串口回调函数 */

/* pid debug */
/*void set_p_i_d(uint8_t ch,float p_temp, float i_temp, float d_temp)
{
    if (ch == CURVES_CH1)
        Motor_PID_l.SetTunings(p_temp, i_temp, d_temp);
    else
        Motor_PID_r.SetTunings(p_temp, i_temp, d_temp);
}

void set_pid_target(uint8_t ch, float temp_val)
{
    if (ch == CURVES_CH1)
        set_target_speed(wheel_left, temp_val);
    else
        set_target_speed(wheel_right, temp_val);
}

void set_period(uint8_t ch,uint32_t period)
{
    if (ch == CURVES_CH1)
        Motor_PID_l.SetSampleTime(period);
    else
        Motor_PID_r.SetSampleTime(period);
}*/

/**********************************************************************************************/
