#include "Buttons.h"

//---------------------------------------------------------------------------
void Buttons::begin()
{
	pins[BTN_LEFT]  = BTN_LEFT_PIN;
	pins[BTN_UP]    = BTN_UP_PIN;
	pins[BTN_RIGHT] = BTN_RIGHT_PIN;
	pins[BTN_DOWN]  = BTN_DOWN_PIN;
	pins[BTN_A]     = BTN_A_PIN;
	pins[BTN_B]     = BTN_B_PIN;
	pins[BTN_C]     = BTN_C_PIN;
}
//---------------------------------------------------------------------------
void Buttons::update()
{
	for(uint8_t thisButton=0; thisButton<NUM_BTN-1; thisButton++)
	{
		// enable internal pull up resistors
		pinMode(pins[thisButton], INPUT_PULLUP);

		// if button pressed
		if(digitalRead(pins[thisButton]) == LOW)
		{
			// increase button hold time
			states[thisButton]++;
		}
		else
		{
			// button idle
			if(states[thisButton] == 0)
			{
				continue;
			}

			// if previously released
			if(states[thisButton] == 0xFF)
			{
				// set to idle
				states[thisButton] = 0;
			}
			else
			{
				// button just released
				states[thisButton] = 0xFF;
			}
		}

		// disable internal pull up resistors to save power
		pinMode(pins[thisButton], INPUT);
	}

	if(pressed(BTN_RIGHT) && pressed(BTN_LEFT))
	{
		states[BTN_C] = 1;
	}
	else
	{
		states[BTN_C] = 0;
	}
}
//---------------------------------------------------------------------------
boolean Buttons::pressed(uint8_t button)
{
	return (states[button] == 1) ? true : false;
}
//---------------------------------------------------------------------------
boolean Buttons::released(uint8_t button)
{
	return (states[button] == 0xFF) ? true : false;
}
//---------------------------------------------------------------------------
boolean Buttons::held(uint8_t button, uint8_t time)
{
	return (states[button] == time + 1) ? true : false;
}
//---------------------------------------------------------------------------
boolean Buttons::repeat(uint8_t button, uint8_t period)
{
	if(period <= 1)
	{
		if((states[button] != 0xFF) && (states[button]))
		{
			return true;
		}
	}
	else
	{
		if((states[button] != 0xFF) && ((states[button] % period) == 1))
		{
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
uint8_t Buttons::timeHeld(uint8_t button)
{
	return (states[button] != 0xFF) ? states[button] : 0;
}
