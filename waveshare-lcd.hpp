/**
 * A Waveshare WSLCD18 driver, targeted at the micro:bit v2 CODAL platform.
 * This file is licensed under the GPLv3 license.
 * 
 * Author: dev@c272.org
 **/
#include "MicroBitIO.h"
#include "NRF52SPI.h"
#include "waveshare-spi.hpp"

#define WSLCD18_SIZE_X 128
#define WSLCD18_SIZE_Y 160
#define WSLCD_MEM_SIZE WSLCD18_SIZE_X*WSLCD18_SIZE_Y*2
#define WSLCD_MEM_CHUNKS 16

namespace waveshare
{

//Represents the display driver for the Waveshare WSLCD18.
class display {
    public:
	display(MicroBitIO *io);
	~display();
	void init();
	void reset();
	void sw_reset();

	void set_display_enabled(bool enabled);
	void set_sleep(bool enabled);
	void set_draw_window(rect window);
	void set_spi_freq(uint32_t rate);
	void set_framerate(uint8_t rtna, uint8_t fpa, uint8_t bpa);

	void clear_screen(colour16 colour);
	void send_pixel_data(rect draw_window, uint8_t *data, int len);
	void send_pixel_data_async(rect draw_window, uint8_t *data, int len,
				   codal::PVoidCallback callback = NULL);

	static rect area;

    private:

	//We don't support these outside of the driver as structs are built
	//upon the assumption of RGB uint16 MDAC & colour mode configs.
	void set_colour_mode(colour_mode colour_mode);
	void set_mdac_config(mdac_config config);

	void send_command(uint8_t op, int delay = 0, uint8_t *data = NULL,
			  int len = 0);
	void send_command_async(uint8_t op, uint8_t *data, int len,
				codal::PVoidCallback callback = NULL);

	//Pin representations for the ST7735S.
	NRF52Pin *miso;
	NRF52Pin *mosi;
	NRF52Pin *sck;
	NRF52Pin *lcd_cs;
	NRF52Pin *ram_cs;
	NRF52Pin *lcd_dc;
	NRF52Pin *lcd_rst;
	NRF52Pin *lcd_bl;
	NRF52SPI *spi = NULL;

	//The initialisation state of the display.
	bool initialised = false;
};

} //namespace waveshare