/* RAAT Includes */

#include "raat.hpp"

#include "adafruit-neopixel-raat.hpp"

/* Application Includes */

#include "application.h"
#include "leds.h"

static uint8_t s_mapping[7];

typedef void (*segment_set_fn)(AdafruitNeoPixelRAAT *, uint8_t, uint8_t, uint8_t);

static void set_star(AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b)
{
    pLEDs->set_pixels(0, 29, r, g, b);
}

static void set_topleft(AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b)
{
    pLEDs->set_pixels(30, 41, r, g, b);
    pLEDs->set_pixels(65, 78, r, g, b);
}

static void set_topright(AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b)
{
    pLEDs->set_pixels(42, 64, r, g, b);
}

static void set_midleft(AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b)
{
    pLEDs->set_pixels(79, 89, r, g, b);
    pLEDs->set_pixels(117, 129, r, g, b);
}

static void set_midright(AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b)
{
    pLEDs->set_pixels(90, 116, r, g, b);
}

static void set_botleft(AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b)
{
    pLEDs->set_pixels(130, 139, r, g, b);
    pLEDs->set_pixels(163, 173, r, g, b);
}

static void set_botright(AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b)
{
    pLEDs->set_pixels(140, 162, r, g, b);
}

static const segment_set_fn s_segment_set_fns[7] = {
    set_star,
    set_topleft,
    set_topright,
    set_midleft,
    set_midright,
    set_botleft,
    set_botright
};

static void flash(
    AdafruitNeoPixelRAAT * pLEDs, uint8_t r, uint8_t g, uint8_t b,
    uint16_t ms_delay, uint8_t count)
{
    pLEDs->reset();

    delay(ms_delay);
    
    for (uint8_t i=0; i<count; i++)
    {
        pLEDs->set_pixels(0, NLEDS-1, r, g, b);
        pLEDs->show();

        delay(ms_delay);
        
        pLEDs->reset();

        delay(ms_delay);
    }

}

void leds_set_mapping(char * mapping)
{
    for(uint8_t i=0; i<7; i++)
    {
        s_mapping[i] = mapping[i] - '1';
    }
}

void leds_play_intro(AdafruitNeoPixelRAAT * pLEDs)
{
    delay(100);
    
    pLEDs->reset();

    for(uint8_t i=0; i<7; i++)
    {
        uint8_t led_map = s_mapping[i];
        
        for(uint8_t bright=0; bright<32; bright++)
        {
            s_segment_set_fns[led_map](pLEDs, bright, bright, bright);
            pLEDs->show();
            delay(2);
        }
        delay(100);
    }

    pLEDs->reset();

}

void leds_update(AdafruitNeoPixelRAAT * pLEDs,
    EightBitRGBParam * pEasyColour,
    EightBitRGBParam * pExpertColour,
    GAME_MODE mode,
    uint8_t press_count
    )
{
    static uint8_t old_press_count = 0;
    static GAME_MODE old_game_mode = (GAME_MODE)-1;

    if ((old_press_count != press_count)  || (old_game_mode != mode))
    {
        old_press_count = press_count;
        old_game_mode = mode;
           
        pLEDs->clear();
        for(uint8_t i=0; i<press_count; i++)
        {
            if (mode == GAME_MODE_EXPERT)
            {
                s_segment_set_fns[s_mapping[i]](pLEDs,
                    pExpertColour->get(eR),
                    pExpertColour->get(eG),
                    pExpertColour->get(eB)
                );
            }
            else if (mode == GAME_MODE_EASY)
            {
                s_segment_set_fns[s_mapping[i]](pLEDs,
                    pEasyColour->get(eR),
                    pEasyColour->get(eG),
                    pEasyColour->get(eB)
                );
            }
            
        }
        pLEDs->show();
    }
}

void leds_flash(AdafruitNeoPixelRAAT * pLEDs, EightBitRGBParam * pRGB, uint16_t flash_length, uint8_t flash_count)
{
    flash(pLEDs, pRGB->get(eR), pRGB->get(eG), pRGB->get(eB), flash_length, flash_count);
}
