/**
 * Defines and data structures for the Waveshare WSLCD18 (ST7735S) 
 * SPI interface. 
 * 
 * Author: dev@c272.org
 **/

#include <stdint.h>

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

//The colour modes available for the ST7735S.
enum colour_mode {
	TWELVE_BIT = 3,
	SIXTEEN_BIT = 5,
	EIGHTEEN_BIT = 6,
	UNKNOWN = 7
};

//Memory data access address ordering types.
enum mdac_address_mode {
	//Goes left to right, from top to bottom (normal).
	LTR_TOP_TO_BOTTOM = 0b000,

	//Goes left to right, from bottom to top (Y-invert).
	LTR_BOTTOM_TO_TOP = 0b001,

	//Goes right to left, from top to bottom (X-invert).
	RTL_TOP_TO_BOTTOM = 0b010,

	//Goes right to left, from bottom to top (X/Y-invert).
	RTL_BOTTOM_TO_TOP = 0b011,

	//Goes top to bottom, from left to right (column mode).
	TTB_LEFT_TO_RIGHT = 0b100,

	//Goes bottom to top, from left to right (column-Y-invert).
	BTT_LEFT_TO_RIGHT = 0b101,

	//Goes top to bottom, from right to left (column-X-invert).
	TTB_RIGHT_TO_LEFT = 0b110,

	//Goes bottom to top, from right to left (column-X/Y-invert).
	BTT_RIGHT_TO_LEFT = 0b111
};

//Display refresh orders.
enum vrefresh_order { TOP_TO_BOTTOM = 0, BOTTOM_TO_TOP = 1 };
enum hrefresh_order { LEFT_TO_RIGHT = 0, RIGHT_TO_LEFT = 1 };

//Colour data ordering mode.
enum colour_data_format { RGB = 0, BGR = 1 };

//Memory data access control configuration structure.
struct mdac_config {
	mdac_address_mode address_mode : 3;
	vrefresh_order vertical_refresh : 1;
	colour_data_format colour_format : 1;
	hrefresh_order horizontal_refresh : 1;
	uint8_t resv : 2;

	mdac_config()
	{
		address_mode = LTR_TOP_TO_BOTTOM;
		vertical_refresh = TOP_TO_BOTTOM;
		horizontal_refresh = LEFT_TO_RIGHT;
		colour_format = RGB;
		resv = 0;
	}

} __attribute__((packed));

//Represents a single point on the Waveshare WSLCD18.
struct point {
	uint8_t x;
	uint8_t y;
};

//Represents a draw rectangle for the Waveshare WSLCD18.
struct rect {
	point top_left;
	point bottom_right;
};

} //namespace waveshare