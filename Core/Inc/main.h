/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef TIM_HandleTypeDef PWM_HandleTypeDef;
typedef TIM_HandleTypeDef Encoder_HandleTypeDef;

typedef enum {
    OFF = 0,
    ON
}bool_switch;

typedef enum {
    wheel_right = 0,
    wheel_left
}Motor_WheelType;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM6_TICK_FREQ_DEFAULT 20
#define LED1_Pin GPIO_PIN_9
#define LED1_GPIO_Port GPIOF
#define LED2_Pin GPIO_PIN_10
#define LED2_GPIO_Port GPIOF
#define RIGHT_MOTOR_P_Pin GPIO_PIN_0
#define RIGHT_MOTOR_P_GPIO_Port GPIOC
#define RIGHT_MOTOR_N_Pin GPIO_PIN_1
#define RIGHT_MOTOR_N_GPIO_Port GPIOC
#define LEFT_MOTOR_P_Pin GPIO_PIN_2
#define LEFT_MOTOR_P_GPIO_Port GPIOC
#define LEFT_MOTOR_N_Pin GPIO_PIN_3
#define LEFT_MOTOR_N_GPIO_Port GPIOC
#define LEFT_ENCODER_A_Pin GPIO_PIN_0
#define LEFT_ENCODER_A_GPIO_Port GPIOA
#define LEFT_ENCODER_B_Pin GPIO_PIN_1
#define LEFT_ENCODER_B_GPIO_Port GPIOA
#define LEFT_PWM_Pin GPIO_PIN_2
#define LEFT_PWM_GPIO_Port GPIOA
#define RIGHT_PWM_Pin GPIO_PIN_3
#define RIGHT_PWM_GPIO_Port GPIOA
#define RIGHT_ENCODER_A_Pin GPIO_PIN_6
#define RIGHT_ENCODER_A_GPIO_Port GPIOA
#define RIGHT_ENCODER_B_Pin GPIO_PIN_7
#define RIGHT_ENCODER_B_GPIO_Port GPIOA
#define ULTRASONIC_TRIG_Pin GPIO_PIN_4
#define ULTRASONIC_TRIG_GPIO_Port GPIOC
#define ULTRASONIC_ECHO_Pin GPIO_PIN_5
#define ULTRASONIC_ECHO_GPIO_Port GPIOC
#define TOUCH_IRQ_Pin GPIO_PIN_1
#define TOUCH_IRQ_GPIO_Port GPIOB
#define TOUCH_IRQ_EXTI_IRQn EXTI1_IRQn
#define LCD_TE_SIGNAL_Pin GPIO_PIN_15
#define LCD_TE_SIGNAL_GPIO_Port GPIOB
#define LCD_TE_SIGNAL_EXTI_IRQn EXTI15_10_IRQn
#define SOFT_I2C_SCL_Pin GPIO_PIN_6
#define SOFT_I2C_SCL_GPIO_Port GPIOD
#define SOFT_I2C_SDA_Pin GPIO_PIN_7
#define SOFT_I2C_SDA_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
//extern DCMI_HandleTypeDef *DCMI_Handle;
extern SRAM_HandleTypeDef *SRAM_Handle;
extern DMA_HandleTypeDef *SRAMToLCD_DMA_Handle;
extern RTC_HandleTypeDef *RtcHandle;
extern I2C_HandleTypeDef *I2C_Handle;
extern TIM_HandleTypeDef *usDelay_timer_Handle;
extern UART_HandleTypeDef *UartHandle;
extern PWM_HandleTypeDef *LEFT_motor_pwm;
extern PWM_HandleTypeDef *RIGHT_motor_pwm;
extern TIM_HandleTypeDef *encoder_timer;
extern Encoder_HandleTypeDef *LEFT_encoder;
extern Encoder_HandleTypeDef *RIGHT_encoder;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
