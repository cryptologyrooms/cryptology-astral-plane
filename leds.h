#ifndef _LEDS_H_
#define _LEDS_H_

void leds_set_mapping(char * mapping);
void leds_play_intro(AdafruitNeoPixelRAAT * pLEDs);

void leds_update(AdafruitNeoPixelRAAT * pLEDs,
    EightBitRGBParam * pEasyColour,
    EightBitRGBParam * pExpertColour,
    GAME_MODE mode,
    uint8_t press_count
    );

void leds_flash(AdafruitNeoPixelRAAT * pLEDs, EightBitRGBParam * pRGB, uint16_t flash_length, uint8_t flash_count);
void leds_set_all(AdafruitNeoPixelRAAT * pLEDs, EightBitRGBParam * pRGB);

#endif
