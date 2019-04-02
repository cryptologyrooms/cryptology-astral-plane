/* Arduino Includes */

/* RAAT Includes */

#include "raat.hpp"

#include "string-param.hpp"

#include "adafruit-neopixel-raat.hpp"

#include "debounced-input.hpp"

/* Application Includes */

/* Defines, typedefs, constants */

#define N_BUTTONS 7

typedef enum game_mode
{
    GAME_MODE_EASY,
    GAME_MODE_EXPERT
} GAME_MODE;

/* Private Variables */

static char s_press_record[N_BUTTONS+1] = "0000000";

static uint8_t s_press_index = 0;

static GAME_MODE s_mode = (GAME_MODE)-1;

/* Private Functions */

static void update_buttons(DebouncedInput * const pButtons[N_BUTTONS])
{
    uint8_t button_count = 0;
    char button_pressed = '0';

    for (uint8_t i = 0; i<N_BUTTONS; i++)
    {
        if (pButtons[i]->check_low_and_clear())
        {
            button_count++;
            button_pressed = '1' + i;
        }
    }

    if ((button_count == 1) && (s_press_index < N_BUTTONS))
    {
        s_press_record[s_press_index++] = button_pressed;
    } 
}

static void update_mode(DebouncedInput * const pModeSwitch)
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
    if (new_mode != s_mode)
    {
        raat_logln(LOG_APP, "New mode: %s", new_mode == GAME_MODE_EASY ? "easy" : "expert");
        s_mode = new_mode;
    }
}

static bool compare_buttons(char const * const to_match)
{
    for (uint8_t i = 0; i<s_press_index; i++)
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
    (void)devices;
    (void)params;

    //TODO: LED test
}

void raat_custom_loop(const raat_devices_struct& devices, const raat_params_struct& params)
{
    update_buttons(devices.pButtons);
    update_mode(devices.pMode_Switch);

    char to_match[N_BUTTONS+1];
    params.pButtonOrder->get(to_match);
   
    uint8_t check_threshold = s_mode == GAME_MODE_EASY ? 3 : 6;

    if (s_press_index >= check_threshold)
    {
        bool match_result = compare_buttons(to_match);
        if (!match_result)
        {
            raat_logln(LOG_APP, "Incorrect press %c (expected %c)",
                s_press_record[s_press_index],
                to_match[s_press_index]
            );
            memset(s_press_record, '0', N_BUTTONS);
            s_press_index = 0;
            //TODO: fail_leds(s_mode);
        }
    }

    if (s_press_index == N_BUTTONS)
    {
        //TODO: success_leds(s_mode);
    }
    //TODO: update_leds(s_mode, s_press_index);
}