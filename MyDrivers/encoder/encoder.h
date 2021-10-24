#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    __IO int32_t delta_counter;  /* 编码器单位时间内的增量 */
    __IO int32_t last_counter;  /* 编码器单位时间内的增量 */
    __IO int16_t overflow_count;    /* 定时器溢出次数 */
}encoder_info;

extern void startEncoder(Encoder_HandleTypeDef **Encoder_Handle,uint8_t count);
extern void stopEncoder(Encoder_HandleTypeDef **Encoder_Handle,uint8_t count);
extern void getEncoderInfo(Encoder_HandleTypeDef *Encoder_Handle,encoder_info *encoder);
extern void updateEncoderCounter(Encoder_HandleTypeDef *Encoder_Handle, encoder_info *encoder);

#ifdef __cplusplus
}
#endif

#endif
