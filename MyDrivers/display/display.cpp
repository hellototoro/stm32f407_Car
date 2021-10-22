/*
 * lcd.cpp
 *
 *  Created on: Aug 5, 2021
 *      Author: huang
 */

#include "display/display.hpp"

namespace MyDrivers {

display::display(tft_lcd* panelType) {
    // TODO Auto-generated constructor stub
    DrawProp.BackColor = lcd_color::WHITE;
    DrawProp.pFont     = &Font24;
    DrawProp.TextColor = lcd_color::BLACK;
    tft_panel = panelType;
}

void display::init(void)
{
    tft_panel->init();
}

/**
  * @brief  Sets the LCD text color.
  * @param  Color: Text color code RGB(5-6-5)
  */
void display::setTextColor(lcd_color Color)
{
    DrawProp.TextColor = Color;
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: Color of the background
  */
void display::clear(lcd_color Color)
{ 
    uint32_t counter = 0;
    
    for(counter = 0; counter < tft_panel->getPixelHeight(); counter++)
    {
        tft_panel->drawHLine(static_cast<uint16_t>(Color), 0, counter, tft_panel->getPixelWidth());
    }
}

uint32_t display::getDisplayAddress()
{
    return tft_panel->getRamAddress();
}

display::~display() {
	// TODO Auto-generated destructor stub
}

} /* namespace MyDrivers */
