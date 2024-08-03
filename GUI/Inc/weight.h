#pragma once

#include "lcdHandler.h"

typedef enum {
	ABSOLUTE = 0,
	ALIGNMENT_TOP_LEFT,
	ALIGNMENT_TOP,
	ALIGNMENT_TOP_RIGHT,
	ALIGNMENT_LEFT,
	ALIGNMENT_CENTER,
	ALIGNMENT_RIGHT,
	ALIGNMENT_BOTTOM_LEFT,
	ALIGNMENT_BOTTOM,
	ALIGNMENT_BOTTOM_RIGHT
}ALIGNMENT;

typedef struct ltrb {
	uint8_t top;
	uint8_t bottom;
	uint8_t left;
	uint8_t right;
}LTRB;

typedef struct border {
	LTRB width;
	Color color;
	// uint8_t radius;
	// style
}Border;

typedef struct weightI {
	void (*init)(struct weightI*);
	void (*deinit)(struct weightI*);
	void (*draw)(struct weightI*);
	void (*clean)(struct weightI*);
	void (*click)(struct weightI*);
	struct weightI* parent;
	LcdHandler* lcdHandler;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	// TODO
	//ALIGNMENT alignment;
	//LTRB margin;
	//Border border;
	//LTRB padding;
	Color background;
}WeightI;

WeightI* weightConstruct(
	WeightI* parent,
	uint16_t x, 
	uint16_t y, 
	uint16_t width, 
	uint16_t height, 
	//ALIGNMENT alignment, 
	//LTRB margin, 
	//Border border, 
	//LTRB padding, 
	Color background,
	void (*init)(struct weightI*));

void rectangleInit(WeightI* self);