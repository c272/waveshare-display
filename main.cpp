#include "waveshare-lcd.hpp"
#include "MicroBit.h"

MicroBit uBit;

int main()
{
    uBit.init();

    waveshare::display display(&uBit.io);
    display.init();

    //Clear screen.
    waveshare::colour16 colour;
    colour.red = 0;
    colour.green = 0b111111;
    colour.blue = 0;
    display.clear_screen(colour);

    while (1)
        uBit.display.scroll("DONE!");
}