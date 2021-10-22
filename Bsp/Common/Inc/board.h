#ifndef COMMON_INC_BOARD_H_
#define COMMON_INC_BOARD_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************
 *               FSMC
 * **********************************************/
extern void LCD_IO_WriteData(uint16_t RegValue);
extern void LCD_IO_WriteReg(uint16_t Reg);
extern uint16_t LCD_IO_ReadData(uint16_t Reg);
extern uint16_t LCD_IO_ReadRamData(void);
extern void LCD_IO_WriteMultipleData(__IO uint16_t *pData, uint32_t Size);
extern void LCD_Delay(uint32_t delay);
extern uint32_t readRamAddress(void);


/*************************************************
 *               ultrasonic
 * **********************************************/
extern void send_trig(void);
extern void start_timer(void);

#ifdef __cplusplus
}
#endif


#endif
