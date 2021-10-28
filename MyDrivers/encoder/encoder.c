#include <encoder/encoder.h>

void startEncoder(Encoder_HandleTypeDef **Encoder_Handle, uint8_t count)
{
    HAL_TIM_Base_Start_IT(encoder_timer);
    for (uint8_t i = 0; i < count; i++) {
        HAL_TIM_Encoder_Start_IT(Encoder_Handle[i], TIM_CHANNEL_ALL);
    }
}

void stopEncoder(Encoder_HandleTypeDef **Encoder_Handle,uint8_t count)
{
    for (uint8_t i = 0; i < count; i++) {
        HAL_TIM_Encoder_Stop_IT(Encoder_Handle[i], TIM_CHANNEL_ALL);
    }
}
