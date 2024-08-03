#include "weight.h"

static void RectangleDraw(WeightI* self) 
{
	uint16_t x, y, width, height;
	if (NULL != self->parent)
	{
		x = self->x + self->parent->x;
		y = self->y + self->parent->y;
		width = self->width + self->x <= self->parent->width ? self->width : self->parent->width - self->x;
		height = self->height + self->y <= self->parent->height ? self->height : self->parent->height - self->y;
	}
	else
	{
		x = self->x;
		y = self->y;
		width = self->width;
		height = self->height;
	}
	self->lcdHandler->fillWindow(self->lcdHandler ,x, y, width, height, self->background);
}

static void RectangleClean(WeightI* self)
{
	uint16_t x, y, width, height;
	Color color = { 0xff, 0xff, 0xff };
	if (NULL != self->parent)
	{
		x = self->x + self->parent->x;
		y = self->y + self->parent->y;
		width = self->width + self->x <= self->parent->width ? self->width : self->parent->width - self->x;
		height = self->height + self->y <= self->parent->height ? self->height : self->parent->height - self->y;
		color = self->parent->background;
	}
	else
	{
		x = self->x;
		y = self->y;
		width = self->width;
		height = self->height;
	}
	self->lcdHandler->fillWindow(self->lcdHandler, x, y, width, height, color);
}

static void RectangleDeinit(WeightI* self)
{
	self->clean(self);
	vPortFree(self);
}

void RectangleInit(WeightI* self)
{
	self->deinit = RectangleDeinit;
	self->draw = RectangleDraw;
	self->clean = RectangleClean;
}

WeightI* WeightConstruct(
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

static void gridViewDraw(GridView* self)
{
	uint16_t x, y, width, height;
	if (NULL != self->interface.parent)
	{
		x = self->interface.x + self->interface.parent->x;
		y = self->interface.y + self->interface.parent->y;
		width = self->interface.width + self->interface.x <= self->interface.parent->width ?
			self->interface.width : self->interface.parent->width - self->interface.x;
		height = self->interface.height + self->interface.y <= self->interface.parent->height ?
			self->interface.height : self->interface.parent->height - self->interface.y;
	}
	else
	{
		x = self->interface.x;
		y = self->interface.y;
		width = self->interface.width;
		height = self->interface.height;
	}
	uint16_t rowSpace = self->rowSpace;
	uint16_t colSpace = self->colSpace;
	uint16_t row = self->row;
	uint16_t col = self->col;
	uint16_t* data = self->data;
	uint16_t cellWidth = (width - (col - 1) * colSpace) / col;
	uint16_t cellHeight = (height - (row - 1) * rowSpace) / row;
	for (uint16_t i = 0; i < row; i++)
		for (uint16_t j = 0; j < col; j++)
		{
			uint16_t value = data[i * col + j];
			self->interface.lcdHandler->fillWindow(
				self->interface.lcdHandler, 
				i * (cellWidth + colSpace) + x, 
				j * (cellHeight + rowSpace) + y, 
				cellWidth, cellHeight, adc2Color(value));
		}
}

GridView* GridViewConstruct(
	WeightI* parent,
	uint16_t x,
	uint16_t y,
	uint16_t width,
	uint16_t height,
	uint8_t row,
	uint8_t col,
	uint8_t rowSpace,
	uint8_t colSpace,
	uint16_t* data)
{
	GridView* gridView = (GridView*)pvPortMalloc(sizeof(GridView));
	WeightI* temp = WeightConstruct(parent, x, y, width, height, int2Color(WHITE), RectangleInit);
	gridView->interface = *temp;
	vPortFree(temp);
	gridView->row = row;
	gridView->col = col;
	gridView->rowSpace = rowSpace;
	gridView->colSpace = colSpace;
	gridView->data = data;
	gridView->interface.draw = gridViewDraw;
	return gridView;
}