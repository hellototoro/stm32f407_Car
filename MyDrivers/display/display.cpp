/*
 * lcd.cpp
 *
 *  Created on: Aug 5, 2021
 *      Author: huang
 */

#include <display/Display.hpp>
#include "display/inc/tft_lcd_font.h"

namespace MyDrivers {

Display::Display(TFT_Lcd* panelType) {
    // TODO Auto-generated constructor stub
    DrawProp.BackColor = lcd_color::WHITE;
    DrawProp.pFont     = &Font24;
    DrawProp.TextColor = lcd_color::BLACK;
    charSize = 12;
    charMode = 0;
    tft_panel = panelType;
}

void Display::init(void)
{
    tft_panel->init();
}

void Display::setCharSize(uint8_t size)
{
    charSize = size;
}

/**
  * @brief  Sets the LCD text color.
  * @param  Color: Text color code RGB(5-6-5)
  */
void Display::setTextColor(lcd_color Color)
{
    DrawProp.TextColor = Color;
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: Color of the background
  */
void Display::clear(lcd_color Color)
{ 
    uint32_t counter = 0;
    
    for(counter = 0; counter < tft_panel->getPixelHeight(); counter++)
    {
        tft_panel->drawHLine(static_cast<uint16_t>(Color), 0, counter, tft_panel->getPixelWidth());
    }
}

uint32_t Display::getDisplayAddress()
{
    return tft_panel->getRamAddress();
}

void Display::showChar(uint16_t x,uint16_t y,char ch)
{
	char temp, t1;
    uint16_t y0=y;
    uint8_t csize = ( charSize/8 + ( (charSize%8) ? 1 : 0) ) * (charSize/2);
    uint8_t index = ch - ' ';
    for(uint8_t t = 0; t < csize; t++)
    {   
        if(charSize == 12)      temp = ascii_1206[index][t];
        else if(charSize == 16) temp = ascii_1608[index][t];
        else if(charSize == 24) temp = ascii_2412[index][t];
        else return;
        for( t1 = 0; t1 < 8; t1++ )
        {
            if( temp & 0x80 ) {
                tft_panel->writePixel(x, y, static_cast<uint16_t>(DrawProp.TextColor));
            } else if( charMode == 0 ) {
                tft_panel->writePixel(x, y, static_cast<uint16_t>(DrawProp.BackColor));
            }
            temp <<= 1;
            y++;
            if( y >= tft_panel->getPixelHeight() )
                return;
            if( (y - y0) == charSize )
            {
                y = y0;
                x++;
                if( x >= tft_panel->getPixelWidth() )
                    return;
                break;
            }
        }
    }
}

void Display::showString(uint16_t x,uint16_t y,char* str)
{
    uint8_t x0 = x;
    uint16_t width = tft_panel->getPixelWidth() + x;
    uint16_t height = tft_panel->getPixelHeight() + y;
    while( ( *str <= '~' ) && ( *str >= ' ' ) )
    {       
        if( x >= width ) {
            x = x0;
            y += charSize;
        }
        if( y >= height )
            break;
        showChar(x, y, *str);
        x += charSize/2;
        str++;
    }  
}

//m^n
uint32_t Display::power(uint8_t m,uint8_t n)
{
    uint32_t result = 1;
    while(n--) result *= m;
    return result;
}


Display::~Display() {
    // TODO Auto-generated destructor stub
}

} /* namespace MyDrivers */
