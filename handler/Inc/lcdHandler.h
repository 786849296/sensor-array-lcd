#pragma once

#include "lcd.h"

typedef struct lcdHandler {
	LcdI* lcd;
	osSemaphoreId* busySemaphore;
	uint32_t timeoutMs;
	void (*init)(
		struct lcdHandler* self,
		LcdI* lcd,
		osSemaphoreId* lcdBusySemaphoreHandle,
		uint32_t timeoutMs);
	void (*deinit)(struct lcdHandler*);
	void (*point)(struct lcdHandler*, uint16_t, uint16_t, Color);
	void (*line)(struct lcdHandler*, uint16_t, uint16_t, uint16_t, uint16_t, Color);
	void (*fillWindow)(struct lcdHandler*, uint16_t, uint16_t, uint16_t, uint16_t, Color);
	void (*image)(struct lcdHandler*, uint16_t, uint16_t, uint16_t, uint16_t, Color*);
} LcdHandler;

extern LcdHandler lcdInstance;
