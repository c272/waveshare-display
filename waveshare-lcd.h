/**
 * A Waveshare WSLCD18 driver, targeted at the micro:bit v2 CODAL platform.
 * This file is licensed under the GPLv3 license.
 * 
 * Author: dev@c272.org
 **/

#include "MicroBitIO.h"

//Pin-out definitions for the ST7735S.
#define ST_BASEID 100
#define ST_PIN_MISO MICROBIT_PIN_P14
#define ST_PIN_MOSI MICROBIT_PIN_P15
#define ST_PIN_SCK MICROBIT_PIN_P13
#define ST_PIN_LCD_CS MICROBIT_PIN_P16
#define ST_PIN_RAM_CS MICROBIT_PIN_P2
#define ST_PIN_LCD_DC MICROBIT_PIN_P12
#define ST_PIN_LCD_RST MICROBIT_PIN_P8
#define ST_PIN_LCD_BL MICROBIT_PIN_P1

//Opcode definitions for the ST7735S.
#define ST_OP_NOP 0x0
#define ST_OP_SWRESET 0x1
#define ST_OP_RDDID 0x4
#define ST_OP_RDDST 0x9
#define ST_OP_RDDPM 0xA
#define ST_OP_RDDMADCTL 0xB
#define ST_OP_RDDIM 0xD
#define ST_OP_RDDSM 0xE
#define ST_OP_RDDSDR 0xF
#define ST_OP_SLPIN 0x10
#define ST_OP_SLPOUT 0x11
#define ST_OP_PTLON 0x12
#define ST_OP_NORON 0x13
#define ST_OP_INVOFF 0x20
#define ST_OP_INFON 0x21
#define ST_OP_GAMSET 0x26
#define ST_OP_DISPOFF 0x28
#define ST_OP_DISPON 0x29
#define ST_OP_CASET 0x2A
#define ST_OP_RASET 0x2B
#define ST_OP_RAMWR 0x2C
#define ST_OP_RGBSET 0x2D
#define ST_OP_RAMRD 0x2E
#define ST_OP_PTLAR 0x30
#define ST_OP_SCRLAR 0x33
#define ST_OP_TEOFF 0x34
#define ST_OP_TEON 0x35
#define ST_OP_MADCTL 0x36
#define ST_OP_VSCSAD 0x37
#define ST_OP_IDMOFF 0x38
#define ST_OP_IDMON 0x39
#define ST_OP_COLMOD 0x3A
#define ST_OP_RDID1 0xDA
#define ST_OP_RDID2 0xDB
#define ST_OP_RDID3 0xDC
#define ST_OP_FRMCTR1 0xB1
#define ST_OP_FRMCTR2 0xB2
#define ST_OP_FRMCTR3 0xB3
#define ST_OP_INVCTR 0xB4
#define ST_OP_PWCTR1 0xC0
#define ST_OP_PWCTR2 0xC1
#define ST_OP_PWCTR3 0xC2
#define ST_OP_PWCTR4 0xC3
#define ST_OP_PWCTR5 0xC4
#define ST_OP_VMCTR1 0xC5
#define ST_OP_VMOFCTR 0xC7
#define ST_OP_WRID2 0xD1
#define ST_OP_WRID3 0xD2
#define ST_OP_NVFCTR1 0xD9
#define ST_OP_NVFCTR2 0xDE
#define ST_OP_NVFCTR3 0xDF
#define ST_OP_GMCTRP1 0xE0
#define ST_OP_GMCTRN1 0xE1
#define ST_OP_GCV 0xFC

namespace waveshare 
{

//Represents the display driver for the Waveshare WSLCD18.
class display
{
    public:
        display();
        void init();

    private:
        //Pin representations for the ST7735S.
        NRF52Pin miso;
        NRF52Pin mosi;
        NRF52Pin sck;
        NRF52Pin lcd_cs;
        NRF52Pin ram_cs;
        NRF52Pin lcd_dc;
        NRF52Pin lcd_rst;
        NRF52Pin lcd_bl;

        //The micro:bit IO instance for this driver.
        MicroBitIO* io;
};

} //namespace waveshare