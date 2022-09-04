/**
 * A Waveshare WSLCD18 driver, targeted at the micro:bit v2 CODAL platform.
 * This file is licensed under the GPLv3 license.
 * 
 * Author: dev@c272.org
 **/

#include "waveshare-lcd.hpp"
#include "CodalFiber.h"

namespace waveshare
{

//A rectangle representing the entire available display area.
rect display::area = { { 0, 0 }, { WSLCD18_SIZE_X - 1, WSLCD18_SIZE_Y - 1 } };

//Constructor for the display driver.
//Grabs pin references from the micro:bit IO.
display::display(MicroBitIO *io)
{
	mosi = &io->P15;
	miso = &io->P14;
	sck = &io->P13;
	lcd_cs = &io->P16;
	ram_cs = &io->P2;
	lcd_dc = &io->P12;
	lcd_rst = &io->P8;
	lcd_bl = &io->P1;
}

//Destructor for the display driver.
display::~display()
{
	if (spi != NULL)
		delete spi;
}

//Initialises the Waveshare's display.
void display::init()
{
	//Ignore if we already initialised.
	if (initialised)
		return;

	//Create SPI interface with SPI pin references, set frequency
	//to default of 8MB/s.
	spi = new NRF52SPI(*mosi, *miso, *sck);
	set_update_rate(NRF_SPIM_FREQ_8M);

	//Enable backlight.
	//todo: Using analog values with this hangs forever. Why?
	lcd_bl->setDigitalValue(1);

	//Reset hardware, software.
	reset();
	sw_reset();

	//Reconfigure display settings.
	set_colour_mode(colour_mode::SIXTEEN_BIT);
	mdac_config config;
	set_mdac_config(config);

	//Enable the display.
	set_sleep(false);
	set_display_enabled(true);
}

//Resets the LCD display to a default state.
void display::reset()
{
	//Perform HIGH -> 200ms -> LOW -> 200ms -> HIGH on pin RST.
	lcd_rst->setDigitalValue(1);
	fiber_sleep(200);
	lcd_rst->setDigitalValue(0);
	fiber_sleep(200);
	lcd_rst->setDigitalValue(1);
}

//Performs a software reset on the ST7735S.
void display::sw_reset()
{
	send_command(ST_OP_SWRESET, 150);
}

//Set whether or not the display should reflect the current contents of RAM.
//When disabled, the screen will simply show a blank (white) frame buffer.
void display::set_display_enabled(bool enabled)
{
	send_command(enabled ? ST_OP_DISPON : ST_OP_DISPOFF);
}

//Enables or disables sleep mode on the display.
void display::set_sleep(bool enabled)
{
	send_command(enabled ? ST_OP_SLPIN : ST_OP_SLPOUT, 200);
}

//Sets the pixel format of the display
void display::set_colour_mode(colour_mode colour_mode)
{
	uint8_t mode = (uint8_t)colour_mode;
	send_command(ST_OP_COLMOD, 10, &mode, 1);
}

//Sets the memory data access control configuration.
void display::set_mdac_config(mdac_config config)
{
	uint8_t *data = reinterpret_cast<uint8_t *>(&config);
	send_command(ST_OP_MADCTL, 0, data, sizeof(config));
}

//Sets the draw window that will be drawn into in a following
//data write operation. Takes a draw rectangle.
void display::set_draw_window(rect window)
{
	uint8_t col_args[] = { 0x0, window.top_left.x, 0x0,
			       window.bottom_right.x };
	uint8_t row_args[] = { 0x0, window.top_left.y, 0x0,
			       window.bottom_right.y };
	send_command(ST_OP_CASET, 0, col_args, 4);
	send_command(ST_OP_RASET, 0, row_args, 4);
}

//Sets the update rate (in bits/second) for the display.
void display::set_update_rate(uint32_t rate)
{
	spi->setFrequency(rate);
}

//Clears the screen with a given colour.
void display::clear_screen(uint16_t colour)
{
	//Set draw window to entire draw plane.
	set_draw_window(area);

	//Send RAM start command.
	send_command(ST_OP_RAMWR);

	//Switch to data mode (LCD_DC high), transmit.
	lcd_cs->setDigitalValue(0);
	lcd_dc->setDigitalValue(1);
	for (int i = 0; i < WSLCD_MEM_SIZE; i++)
		spi->transfer(reinterpret_cast<uint8_t *>(&colour),
			      sizeof(colour), NULL, 0);

	//Re-idle chip.
	lcd_cs->setDigitalValue(1);
}

//Sends pixel data to the display to draw inside a given draw window.
void display::send_pixel_data(rect draw_window, uint8_t *data, int len)
{
	//Set the draw window, RAM write synchronously.
	set_draw_window(draw_window);
	send_command(ST_OP_RAMWR, 0, data, len);
}

//Sends pixel data to the display for draw inside a given draw window, asynchronously.
void display::send_pixel_data_async(rect draw_window, uint8_t *data, int len,
				    codal::PVoidCallback callback)
{
	//Set the draw window, RAM write asynchronously.
	set_draw_window(draw_window);
	send_command_async(ST_OP_RAMWR, data, len, callback);
}

//Sends a single command over SPI to the ST7735S.
// Parameters:
// - An opcode defined in 'waveshare-spi.hpp' (ST_OP_*).
// - A delay to apply upon command completion (optional).
// - Parameter data to send with the command (optional).
// - The length of the parameter data to send (optional).
void display::send_command(uint8_t op, int delay, uint8_t *data, int len)
{
	//Set LCD_CS low (chip active), LCD_DC low (command mode).
	lcd_cs->setDigitalValue(0);
	lcd_dc->setDigitalValue(0);

	//Transmit opcode.
	spi->write(op);

	//Switch to data mode (LCD_DC high), transmit any arguments.
	lcd_dc->setDigitalValue(1);
	spi->transfer(data, len, NULL, 0);

	//Set LCD_CS high (chip idle).
	lcd_cs->setDigitalValue(1);

	//Delay for the provided amount of time.
	if (delay > 0)
		fiber_sleep(delay);
}

//Sends pixel data to the display to draw inside a given draw window
//in an asynchronous fashion.
// Parameters:
// - An opcode defined in 'waveshare-spi.hpp' (ST_OP_*).
// - Parameter data to send with the command (optional).
// - The length of the parameter data to send (optional).
// - A callback for when the async transfer completes (optional).
// Returns: The result of the SPI command.
void display::send_command_async(uint8_t op, uint8_t *data, int len,
				 codal::PVoidCallback callback)
{
	//Set LCD_CS low (chip active), LCD_DC low (command mode).
	lcd_cs->setDigitalValue(0);
	lcd_dc->setDigitalValue(0);

	//Transmit opcode.
	spi->write(op);

	//Switch to data mode (LCD_DC high), transmit arguments asynchronously.
	lcd_dc->setDigitalValue(1);
	spi->startTransfer(data, len, NULL, 0, callback, NULL);
}

} //namespace waveshare