/*
 * @Author: your name
 * @Date: 2021-05-25 16:27:16
 * @LastEditTime: 2021-05-28 11:08:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /rosserial_stm32/Bsp/oled/Inc/oled.h
 */
#ifndef _OLED_HPP_
#define _OLED_HPP_

#include <sw_i2c/Inc/stm32_iic.hpp>
#include "hw_i2c/HW_I2C.hpp"


class oled 
{
public:
    oled();
    oled(I2C_HandleTypeDef *oled_IIC_Handle);
    void WriteCmd(uint8_t byte);
    void WriteDat(uint8_t byte);
    void init(void);
    void SetPos(unsigned char x,unsigned char y);
    void Fill(unsigned char fill_Data);
    void CLS(void);
    void On(void);
    void Off(void);
    void ShowStr(unsigned char x,unsigned char y,const char ch[],unsigned char TextSize);
    void ShowNumber(unsigned char x,unsigned char y,int num,unsigned char TextSize);
    void Lcd_Address(uint8_t page,uint8_t column);
    void Clear_Screen(void);
    void Display_Frame(void);
private:
	char speed_buf[50];
    Stm32IIC oled_iic;
    I2C_HandleTypeDef *oled_IIC_Handle;
    MyDrivers::hw_iic oled_hw_iic;
};

#endif


