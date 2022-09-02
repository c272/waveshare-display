/**
 * A Waveshare WSLCD18 driver, targeted at the micro:bit v2 CODAL platform.
 * This file is licensed under the GPLv3 license.
 * 
 * Author: dev@c272.org
 **/

#include "waveshare-lcd.h"

namespace waveshare
{

//Constructor for the display driver. Initialises pins from the micro:bit.
display::display() :
    miso(ST_BASEID + ST_PIN_MISO, ST_PIN_MISO, PIN_CAPABILITY_DIGITAL),
    mosi(ST_BASEID + ST_PIN_MOSI, ST_PIN_MOSI, PIN_CAPABILITY_DIGITAL),
    sck(ST_BASEID + ST_PIN_SCK, ST_PIN_SCK, PIN_CAPABILITY_DIGITAL),
    lcd_cs(ST_BASEID + ST_PIN_LCD_CS, ST_PIN_LCD_CS, PIN_CAPABILITY_DIGITAL),
    ram_cs(ST_BASEID + ST_PIN_RAM_CS, ST_PIN_RAM_CS, PIN_CAPABILITY_DIGITAL),
    lcd_dc(ST_BASEID + ST_PIN_LCD_DC, ST_PIN_LCD_DC, PIN_CAPABILITY_DIGITAL),
    lcd_rst(ST_BASEID + ST_PIN_LCD_RST, ST_PIN_LCD_RST, PIN_CAPABILITY_DIGITAL),
    lcd_bl(ST_BASEID + ST_PIN_LCD_BL, ST_PIN_LCD_BL, PIN_CAPABILITY_DIGITAL)
{

}

//Initialises the Waveshare's display.
void display::init()
{
    //...
}

} //namespace waveshare