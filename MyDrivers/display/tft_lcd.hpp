/*
 * tft_lcd.hpp
 *
 *  Created on: Oct 22, 2021
 *      Author: huang
 */

#ifndef DISPLAY_TFT_LCD_HPP_
#define DISPLAY_TFT_LCD_HPP_

#include "main.h"

namespace MyDrivers {

enum class scanMode{
    L2R_U2D = 0,    //从左到右,从上到下
    L2R_D2U,        //从左到右,从下到上
    R2L_U2D,        //从右到左,从上到下
    R2L_D2U,        //从右到左,从下到上

    U2D_L2R,        //从上到下,从左到右
    U2D_R2L,        //从上到下,从右到左
    D2U_L2R,        //从下到上,从左到右
    D2U_R2L         //从下到上,从右到左
};

enum class displayDir{
    vertical = 0,
    horizontal
};

class TFT_Lcd {
public:
    virtual ~TFT_Lcd() {

    }

    virtual uint16_t getID(void) = 0;
    virtual void init(void) = 0;
    virtual void prepareWrite(void) = 0;
    virtual void setCursor(uint16_t Xpos,uint16_t Ypos) = 0;
    virtual void setDisplayWindow(uint16_t Xpos,uint16_t Ypos,uint16_t width,uint16_t height) = 0;
    //virtual void setScanMode(scanMode mode) = 0;
    virtual void setDisplayDir(displayDir _displayDir) = 0;
    virtual uint16_t getPixelWidth(void) = 0;
    virtual uint16_t getPixelHeight(void) = 0;
    virtual uint32_t getRamAddress(void) = 0;

    virtual void writePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode) = 0;
    virtual uint16_t readPixel(uint16_t Xpos, uint16_t Ypos) = 0;
    virtual void drawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length) = 0;
    virtual void drawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length) = 0;
    virtual void drawLine(uint16_t RGBCode, uint16_t startXpos, uint16_t startYpos, uint16_t endXpos, uint16_t endYpos) = 0;
    virtual void drawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp) = 0;
    virtual void drawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata) = 0;

    virtual void displayOn(void) = 0;
    virtual void displayOff(void) = 0;

};

} /* namespace MyDrivers */

#endif /* DISPLAY_TFT_LCD_HPP_ */
