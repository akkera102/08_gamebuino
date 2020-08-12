#ifndef BUTTONS_H
#define	BUTTONS_H

#include <Arduino.h>
#include "lib_settings.c"

class Buttons
{
public:
	void    begin();
	void    update();

	boolean pressed(uint8_t button);
	boolean released(uint8_t button);
	boolean held(uint8_t button, uint8_t time);
	boolean repeat(uint8_t button, uint8_t period);
	uint8_t timeHeld(uint8_t button);

	uint8_t pins[NUM_BTN];
	uint8_t states[NUM_BTN];
};

#endif
