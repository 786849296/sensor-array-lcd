#include "layout.h"

static void CanvasDraw(LayoutI* self)
{
	for (int i = 0; i < self->itemsCount; i++)
        self->items[i]->draw(self->items[i]);
}

static void CanvasClean(LayoutI* self)
{
    self->interface.lcdHandler->fillWindow(self->interface.lcdHandler, self->interface.x, self->interface.y, self->interface.width, self->interface.height, self->interface.background);
}

static void CanvasDeinit(LayoutI* self)
{
	self->interface.clean(self);
	for (int i = 0; i < self->itemsCount; i++)
		self->items[i]->deinit(self->items[i]);
	vPortFree(self);
}

static void CanvasInit(LayoutI* self)
{
	self->interface.deinit = CanvasDeinit;
	self->interface.draw = CanvasDraw;
	self->interface.clean = CanvasClean;
}

LayoutI* CanvasConstruct(
    WeightI* parent,
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    Color background,
    WeightI** items,
    int itemsCount)
{
    LayoutI* layout = (LayoutI*)pvPortMalloc(sizeof(LayoutI));
	WeightI* temp = WeightConstruct(parent, x, y, width, height, background, CanvasInit);
	layout->interface = *temp;
	vPortFree(temp);
	layout->items = items;
	layout->itemsCount = itemsCount;
    for (int i = 0; i < itemsCount; i++)
        layout->items[i]->parent = layout;
	layout->interface.clean(&layout->interface);
	return layout;
}

static void GridDraw(Grid* self)
{
	uint16_t xSave = self->interface.interface.x;
	uint16_t ySave = self->interface.interface.y;
	uint16_t widthSave = self->interface.interface.width;
	uint16_t heightSave = self->interface.interface.height;
	for (int i = 0; i < self->interface.itemsCount; i++)
	{
		uint8_t row = self->itemsPos[2 * i];
		uint8_t col = self->itemsPos[2 * i + 1];
		self->interface.interface.width = self->colDef[col];
		self->interface.interface.height = self->rowDef[row];
		for (int i = 0; i < col; i++)
			self->interface.interface.x += self->colDef[i];
		for (int i = 0; i < row; i++)
			self->interface.interface.y += self->rowDef[i];
		self->interface.items[i]->draw(self->interface.items[i]);
		self->interface.interface.x = xSave;
		self->interface.interface.y = ySave;
	}
	self->interface.interface.width = widthSave;
	self->interface.interface.height = heightSave;
}

Grid* GridConstruct(
	WeightI* parent,
	uint16_t x,
	uint16_t y,
	uint16_t width,
	uint16_t height,
	Color background,
	WeightI** items,
	int itemsCount,
	uint16_t* rowDef,
	uint16_t* colDef,
	uint8_t* itemsPos)
{
	Grid* grid = (Grid*)pvPortMalloc(sizeof(Grid));
	LayoutI* temp = CanvasConstruct(parent, x, y, width, height, background, items, itemsCount);
	grid->interface = *temp;
	vPortFree(temp);
	for (int i = 0; i < itemsCount; i++)
		grid->interface.items[i]->parent = grid;
	grid->interface.interface.draw = GridDraw;
	grid->rowDef = rowDef;
	grid->colDef = colDef;
	grid->itemsPos = itemsPos;
	return grid;
}
