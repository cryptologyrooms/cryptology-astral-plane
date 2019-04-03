#ifndef _LEDS_H_
#define _LEDS_H_

void leds_play_intro(AdafruitNeoPixelRAAT * pLEDs);
void leds_fail(AdafruitNeoPixelRAAT * pLEDs);
void leds_update(AdafruitNeoPixelRAAT * pLEDs, GAME_MODE mode, uint8_t press_count);
void leds_success(AdafruitNeoPixelRAAT * pLEDs);

#endif
