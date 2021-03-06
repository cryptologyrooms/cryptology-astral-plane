/* Arduino Includes */

/* RAAT Includes */

#include "raat.hpp"

#include "string-param.hpp"

#include "adafruit-neopixel-raat.hpp"

#include "debounced-input.hpp"

/* Application Includes */

#include "application.h"
#include "leds.h"

/* Defines, typedefs, constants */

#define N_BUTTONS 7

/* Private Variables */

static char s_press_record[N_BUTTONS+1] = "0000000";
static bool s_pressed[N_BUTTONS] = {false};

static uint8_t s_press_count = 0;

static GAME_MODE s_mode = (GAME_MODE)-1;

/* Private Functions */

static void reset_game(const raat_devices_struct& devices, const raat_params_struct& params)
{
    memset(s_press_record, '0', N_BUTTONS);
    memset(s_pressed, false, N_BUTTONS);
    s_press_count = 0;
    devices.pMaglock->set(false);
    leds_flash(devices.pLEDs, params.pFailColour,
        500, params.pFlashCount->get()
    );
}

static bool update_buttons(DebouncedInput * const pButtons[N_BUTTONS])
{
    uint8_t button_count = 0;
    char button_pressed = '0';

    for (uint8_t i = 0; i<N_BUTTONS; i++)
    {
        if (pButtons[i]->check_low_and_clear())
        {
            if (!s_pressed[i])
            {
                s_pressed[i] = true;
                button_count++;
                button_pressed = '1' + i;
            }
        }
    }

    if ((button_count == 1) && (s_press_count < N_BUTTONS))
    {
        s_press_record[s_press_count++] = button_pressed;
        raat_logln(LOG_APP, "Button %c press (count %d)", button_pressed, s_press_count);
    }
    return button_pressed != '0';
}

static bool update_mode(DebouncedInput * const pModeSwitch)
{
    GAME_MODE new_mode;
    if (pModeSwitch->state())
    {
        new_mode = GAME_MODE_EASY;
    }
    else
    {
        new_mode = GAME_MODE_EXPERT;
    }

    bool mode_change = new_mode != s_mode;

    if (new_mode != s_mode)
    {
        raat_logln(LOG_APP, "Mode: %s", new_mode == GAME_MODE_EASY ? "easy" : "expert");
        s_mode = new_mode;
    }

    return mode_change;
}

static bool compare_buttons(char const * const to_match)
{
    for (uint8_t i = 0; i<s_press_count; i++)
    {
        if (to_match[i] != s_press_record[i])
        {
            return false;
        }
    }
    return true;
}

/* RAAT Functions */

void raat_custom_setup(const raat_devices_struct& devices, const raat_params_struct& params)
{
    (void)params;
    char mapping[8];
    params.pSegmentMapping->get(mapping);
    leds_set_mapping(mapping);
    leds_play_intro(devices.pLEDs);
}

void raat_custom_loop(const raat_devices_struct& devices, const raat_params_struct& params)
{
    if ((s_press_count < N_BUTTONS) && !devices.pActivate_Switch->state())
    {
        bool button_press = update_buttons(devices.pButtons);
        bool mode_change = update_mode(devices.pMode_Switch);
        uint8_t easy_mode_threshold = params.pEasyThreshold->get() + 1;
        char to_match[N_BUTTONS+1];
        params.pButtonOrder->get(to_match);

        if (button_press || mode_change)
        {
            uint8_t check_threshold = s_mode == GAME_MODE_EASY ? easy_mode_threshold : 7;

            if (s_press_count >= check_threshold)
            {
                bool match_result = compare_buttons(to_match);
                if (!match_result)
                {
                    raat_logln(LOG_APP, "Mo match, rst");
                    reset_game(devices, params);
                }
            }

            if (s_press_count == N_BUTTONS)
            {
                devices.pMaglock->set(true);
                leds_flash(devices.pLEDs, params.pSuccessColour,
                    500, params.pFlashCount->get()
                );
                devices.pLEDs->set_pixels(0, NLEDS-1, 
                    params.pSuccessColour->get(eR),
                    params.pSuccessColour->get(eG),
                    params.pSuccessColour->get(eB)
                );
                devices.pLEDs->show();
                delay(params.pFlashPersist->get()*1000);
                devices.pLEDs->clear();
                devices.pLEDs->show();
                return;
            }

            leds_update(devices.pLEDs, 
                params.pEasyColour, params.pExpertColour,
                s_mode, s_press_count);
        }
    }
}
