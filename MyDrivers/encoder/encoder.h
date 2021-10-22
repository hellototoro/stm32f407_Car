#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int8_t direction;
    int32_t delta_counter;  /* 编码器单位时间内的增量 */
}encoder_info;

void startEncoder(Encoder_HandleTypeDef **Encoder_Handle,uint8_t count);
void stopEncoder(Encoder_HandleTypeDef **Encoder_Handle,uint8_t count);
void getEncoderInfo(Encoder_HandleTypeDef *Encoder_Handle,encoder_info *encoder);

#ifdef __cplusplus
}
#endif

#endif
