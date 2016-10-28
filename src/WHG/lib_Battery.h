#ifndef BATTERY_H
#define	BATTERY_H

#include <Arduino.h>
#include "lib_settings.c"

class Battery
{
public:
	void begin();
	void update();

	boolean  show;
	uint8_t  level;
	uint16_t voltage;
	uint16_t thresholds[NUM_LVL];
	uint8_t  nextUpdate;
};

#endif
