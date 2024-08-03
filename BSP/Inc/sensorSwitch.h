#pragma once

#include "main.h"

typedef struct sensorSwitchI {
	void (*deinit)(struct sensorSwitchI*);
	void (*setState)(struct sensorSwitchI*, FunctionalState);
	void (*setChannel)(struct sensorSwitchI*, uint8_t);
	uint8_t maxChannel;
}SensorSwitchI;



typedef struct cd4051 {
	SensorSwitchI interface;
	Gpio* ABC;
	Gpio* INH;
}Cd4051;

Cd4051* Cd4051Construct(Gpio* ABC, Gpio* INH);

// static Instance
extern SensorSwitchI* rowSwitchs[1];
extern SensorSwitchI* colSwitchs[4];