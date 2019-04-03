/* RAAT Includes */

#include "raat.hpp"

#include "adafruit-neopixel-raat.hpp"

/* Application Includes */

#include "application.h"
#include "leds.h"

void leds_play_intro(AdafruitNeoPixelRAAT * pLEDs)
{
    for(uint8_t i=0; i<7; i++)
    {
        pLEDs->setPixelColor(i, 128,0,0);
        pLEDs->show();
        delay(100);
    } 

    for(uint8_t i=0; i<7; i++)
    {
        pLEDs->setPixelColor(i, 0,128,0);
        pLEDs->show();
        delay(100);
    } 

    for(uint8_t i=0; i<7; i++)
    {
        pLEDs->setPixelColor(i, 0,0,128);
        pLEDs->show();
        delay(100);
    }

    pLEDs->clear();
    pLEDs->show();

}

void leds_fail(AdafruitNeoPixelRAAT * pLEDs, GAME_MODE mode)
{
    if (mode == GAME_MODE_EXPERT)
    {
        pLEDs->set_pixels(0, 6, 96, 0, 0);
        pLEDs->show();
        delay(250);
        pLEDs->set_pixels(0, 6, 0, 0, 0);
        pLEDs->show();
        delay(250);
        pLEDs->set_pixels(0, 6, 96, 0, 0);
        pLEDs->show();
        delay(250);
        pLEDs->set_pixels(0, 6, 0, 0, 0);
        pLEDs->show();
    }
}

void leds_update(AdafruitNeoPixelRAAT * pLEDs, GAME_MODE mode, uint8_t press_count)
{
    pLEDs->clear();
    for(uint8_t i=0; i<press_count; i++)
    {
        if (mode == GAME_MODE_EXPERT)
        {
            pLEDs->setPixelColor(i, 64, 64, 0);
        }
        else if (mode == GAME_MODE_EASY)
        {
            pLEDs->setPixelColor(i, 0, 64, 0);
        }       
    }
    pLEDs->show();
}
