#pragma once

#include "main.h"
#include "layout.h"

typedef struct router {
	void (*init)(struct router*, );
	void (*deinit)(struct router*);
	Layout** pages;
	uint8_t pagesCount;
	uint8_t index;
}Router;