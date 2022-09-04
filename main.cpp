#include "waveshare-lcd.hpp"
#include "MicroBit.h"

MicroBit uBit;

int main()
{
    uBit.init();

    waveshare::display display(&uBit.io);
    display.init();

    //Clear screen.
    display.clear_screen(0xC5C5);

    while (1)
        uBit.display.scroll("DONE!");
}