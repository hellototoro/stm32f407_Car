/*
 * r61509.hpp
 *
 *  Created on: Oct 22, 2021
 *      Author: huang
 */

#ifndef COMPONENTS_R61509_R61509_HPP_
#define COMPONENTS_R61509_R61509_HPP_

#include "main.h"

#include "board.h"
#include "display/tft_lcd.hpp"

class r61509 : public MyDrivers::tft_lcd {
public:
    r61509();
    virtual uint16_t getID(void);
    virtual void init(void);
    virtual void prepareWrite(void);
    virtual void setCursor(uint16_t Xpos,uint16_t Ypos);
    virtual void setDisplayWindow(uint16_t Xpos,uint16_t Ypos,uint16_t width,uint16_t height);
    virtual void setDisplayDir(MyDrivers::displayDir _displayDir);
    virtual uint16_t getPixelWidth(void);
    virtual uint16_t getPixelHeight(void);
    virtual uint32_t getRamAddress(void);

    virtual uint16_t readPixel(uint16_t Xpos, uint16_t Ypos);
    virtual void writePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode);
    virtual void drawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
    virtual void drawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
    virtual void drawLine(uint16_t RGBCode, uint16_t startXpos, uint16_t startYpos, uint16_t endXpos, uint16_t endYpos);
    virtual void drawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
    virtual void drawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata);

    virtual void displayOn(void);
    virtual void displayOff(void);
    virtual ~r61509();

protected:
    void readID(void);
    void setScanMode(MyDrivers::displayDir _displayDir);
    uint16_t readReg(uint8_t LCDReg);
    void writeReg(uint8_t LCDReg, uint16_t LCDRegValue);

private:
    uint16_t width;
    uint16_t height;
    uint16_t x_cmd;
    uint16_t y_cmd;
    MyDrivers::displayDir dir;
    //MyDrivers::scanMode mode;
    uint16_t id;
};


#define R7               0x0007
#define R32              0x0200
#define R33              0x0201
#define R34              0x0202
#define R80              0x0210
#define R81              0x0211
#define R82              0x0212
#define R83              0x0213
#define R3               0x0003


#endif /* COMPONENTS_R61509_R61509_HPP_ */
