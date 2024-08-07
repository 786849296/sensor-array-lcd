#pragma once

#include "main.h"
#include "ctp.h"

typedef struct ctpHandler {
	struct ctpI* ctp;
	osSemaphoreId* busySemaphore;
	void (*init)(
		struct ctpHandler* self,
		struct ctpI* ctp,
		osSemaphoreId* busySemaphore);
	void (*deinit)(struct ctp*);
	GESTURE (*getGesture)(struct ctp*);
	void (*getPos)(struct ctp*, uint16_t*, uint16_t*);
}CtpHandler;
	
extern CtpHandler ctpHandler;