#ifndef _LEDS_H_
#define _LEDS_H_

void leds_play_intro(AdafruitNeoPixelRAAT * pLEDs);
void leds_fail(AdafruitNeoPixelRAAT * pLEDs, GAME_MODE mode);
void leds_update(AdafruitNeoPixelRAAT * pLEDs, GAME_MODE mode, uint8_t press_count);

#endif
