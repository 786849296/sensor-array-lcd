#pragma once

#include "sensorSwitch.h"

typedef struct sensorSwitch {
	int row;
	int col;
	SensorSwitchI** rowSwitchs;
	uint8_t maxRowSwitchsCount;
	SensorSwitchI** colSwitchs;
	uint8_t maxColSwitchsCount;
	void (*init)(
		struct sensorSwitch*, 
		SensorSwitchI**,
		uint8_t,
		SensorSwitchI**,
		uint8_t,
		void (*)(struct sensorSwitch*, uint8_t),
		void (*)(struct sensorSwitch*, uint8_t));
	void (*deinit)(struct sensorSwitch*);
	void (*setRow)(struct sensorSwitch*, uint8_t);
	void (*setCol)(struct sensorSwitch*, uint8_t);
}SensorSwitch;

extern SensorSwitch sensorSwitchInstance;