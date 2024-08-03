#include "weight.h"

static void rectangleDraw(WeightI* self) 
{
	uint16_t x = self->x + self->parent->x;
	uint16_t y = self->y + self->parent->y;
	uint16_t width = self->width + x <= self->parent->width ? self->width : self->parent->width - x;
	uint16_t height = self->height + y <= self->parent->height ? self->height : self->parent->height - y;
	self->lcdHandler->fillWindow(self->lcdHandler ,x, y, width, height, self->background);
}

static void rectangleClean(WeightI* self)
{
	uint16_t x = self->x + self->parent->x;
	uint16_t y = self->y + self->parent->y;
	uint16_t width = self->width + x <= self->parent->width ? self->width : self->parent->width - x;
	uint16_t height = self->height + y <= self->parent->height ? self->height : self->parent->height - y;
	self->lcdHandler->fillWindow(self->lcdHandler, x, y, width, height, self->parent->background);
}

static void rectangleDeinit(WeightI* self)
{
	self->clean(self);
	vPortFree(self);
}

void rectangleInit(WeightI* self)
{
	self->deinit = rectangleDeinit;
	self->draw = rectangleDraw;
	self->clean = rectangleClean;
}

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
	void (*init)(struct weightI*))
{
	WeightI* weight = (WeightI*)pvPortMalloc(sizeof(WeightI));
	weight->parent = parent;
	weight->x = x;
	weight->y = y;
	weight->width = width;
	weight->height = height;
	//weight->alignment = alignment;
	//weight->margin = margin;
	//weight->border = border;
	//weight->padding = padding;
	weight->background = background;
	weight->lcdHandler = &lcdInstance;
	weight->init = init;
	weight->init(weight);
	return weight;
}