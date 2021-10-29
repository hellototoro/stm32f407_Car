
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif   

/* 数据接收缓冲区大小 */
#define PROT_FRAME_RECV_MIN_LEN 11
#define PROT_FRAME_RECV_MAX_LEN 25

/* 校验数据的长度 */
#define PROT_FRAME_LEN_CHECKSUM    1

/* 数据头结构体 */
typedef struct
{
  uint32_t head;    // 包头
  uint8_t ch;       // 通道
  uint32_t len;     // 包长度
  uint8_t cmd;      // 命令
//  uint8_t sum;      // 校验和
  
}__attribute__((packed)) packet_head_t;

#define FRAME_HEADER     0x59485A53    // 帧头

/* 通道宏定义 */
#define CURVES_CH1      0x01
#define CURVES_CH2      0x02
#define CURVES_CH3      0x03
#define CURVES_CH4      0x04
#define CURVES_CH5      0x05

/* 指令(下位机 -> 上位机) */
#define SEND_TARGET_CMD      0x01     // 发送上位机通道的目标值
#define SEND_FACT_CMD        0x02     // 发送通道实际值
#define SEND_P_I_D_CMD       0x03     // 发送 PID 值（同步上位机显示的值）
#define SEND_START_CMD       0x04     // 发送启动指令（同步上位机按钮状态）
#define SEND_STOP_CMD        0x05     // 发送停止指令（同步上位机按钮状态）
#define SEND_PERIOD_CMD      0x06     // 发送周期（同步上位机显示的值）

/* 指令(上位机 -> 下位机) */
#define SET_P_I_D_CMD        0x10     // 设置 PID 值
#define SET_TARGET_CMD       0x11     // 设置目标值
#define START_CMD            0x12     // 启动指令
#define STOP_CMD             0x13     // 停止指令
#define RESET_CMD            0x14     // 复位指令
#define SET_PERIOD_CMD       0x15     // 设置周期

/* 空指令 */
#define CMD_NONE             0xFF     // 空指令

/* 索引值宏定义 */
#define HEAD_INDEX_VAL       0x3u     // 包头索引值（4字节）
#define CHX_INDEX_VAL        0x4u     // 通道索引值（1字节）
#define LEN_INDEX_VAL        0x5u     // 包长索引值（4字节）
#define CMD_INDEX_VAL        0x9u     // 命令索引值（1字节）

#define EXCHANGE_H_L_BIT(data)      ((((data) << 24) & 0xFF000000) |\
                                     (((data) <<  8) & 0x00FF0000) |\
                                     (((data) >>  8) & 0x0000FF00) |\
                                     (((data) >> 24) & 0x000000FF))     // 交换高低字节

#define COMPOUND_32BIT(data)        (((*(data-0) << 24) & 0xFF000000) |\
                                     ((*(data-1) << 16) & 0x00FF0000) |\
                                     ((*(data-2) <<  8) & 0x0000FF00) |\
                                     ((*(data-3) <<  0) & 0x000000FF))      // 合成为一个字
                                     

/**
  * @brief 设置上位机的值
  * @param cmd：命令
  * @param ch: 曲线通道
  * @param data：参数指针
  * @param num：参数个数
  * @retval 无
  */
extern void set_computer_value(uint8_t cmd, uint8_t ch, void *data, uint8_t num);
extern void pid_tool_start_receive(void);
extern void analysis_rec_data(void);
//extern void set_p_i_d(uint8_t ch,float p_temp, float i_temp, float d_temp);
//extern void set_pid_target(uint8_t ch,float temp_val);
extern void set_period(uint8_t ch,uint32_t period);

#ifdef __cplusplus
}
#endif   

#endif
