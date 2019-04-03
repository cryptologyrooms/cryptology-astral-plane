/* RAAT Includes */

#include "raat.hpp"

#include "adafruit-neopixel-raat.hpp"

/* Application Includes */

#include "application.h"
#include "leds.h"

static void flash_leds(
    AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b,
    uint16_t ms_delay, uint8_t count)
{
    pLEDs->clear();
    pLEDs->show();
    delay(ms_delay);
    
    for (uint8_t i=0; i<count; i++)
    {
        pLEDs->set_pixels(0, 6, r, g, b);
        pLEDs->show();
        delay(ms_delay);
        pLEDs->clear();
        pLEDs->show();
        delay(ms_delay);
    }

}

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

void leds_fail(AdafruitNeoPixelRAAT * pLEDs)
{
    flash_leds(pLEDs, 96, 0, 0, 500, 1);
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

void leds_success(AdafruitNeoPixelRAAT * pLEDs)
{
    flash_leds(pLEDs, 0, 96, 0, 500, 1);
}
