#include "waveshare-lcd.hpp"
#include "MicroBit.h"

MicroBit uBit;

int main()
{
    uBit.init();
    waveshare::display display(&uBit.io);
    display.init();

    //Set up colours.
    auto green = waveshare::colour16(0, 63, 0);
    auto red = waveshare::colour16(31, 0, 0);

    while (1) {
        display.clear_screen(green);
        uBit.sleep(1000);
        display.clear_screen(red);
        uBit.sleep(1000);
    }
}