#include "lcdHandler.h"

static void init(
	LcdHandler* self, 
	LcdI* lcd, 
	osSemaphoreId* lcdBusySemaphoreHandle,
	uint32_t timeoutMs) 
{
	self->lcd = lcd;
	self->timeoutMs = timeoutMs;
	self->busySemaphore = lcdBusySemaphoreHandle;
	self->lcd->init(self->lcd);
	osSemaphoreRelease(*(self->busySemaphore));
	self->fillWindow(self, 0, 0, self->lcd->width, self->lcd->height, int2Color(WHITE));
}

static void deinit(LcdHandler* self) 
{
	osSemaphoreWait(*(self->busySemaphore), osWaitForever);
	self->lcd->deinit(self->lcd);
	osSemaphoreDelete(*(self->busySemaphore));
}

static void point(LcdHandler* self, uint16_t x, uint16_t y, Color color) 
{
	if (osErrorOS == osSemaphoreWait(*(self->busySemaphore), self->timeoutMs)) {
		printf("lcdHandler point: timeout\n");
		return;
	}
	self->lcd->point(self->lcd, x, y, color);
	osSemaphoreRelease(*(self->busySemaphore));
}

static void line(LcdHandler* self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color) 
{
    if (osErrorOS == osSemaphoreWait(*(self->busySemaphore), self->timeoutMs)) {
        printf("lcdHandler line: timeout\n");
        return;
    }
	int dx = x2 > x1 ? x2 - x1 : x1 - x2;
	int dy = y2 > y1 ? y2 - y1 : y1 - y2;
	if (dx == 0 || dy == 0)
		self->lcd->fillWindow(self->lcd, x1, y1, x2, y2, color);
	else 
	{
		int sx = x1 < x2 ? 1 : -1;
		int sy = y1 < y2 ? 1 : -1;
		int err = (dx > dy ? dx : -dy) / 2, e2;

		while (1) {
			self->lcd->point(self->lcd, x1, y1, color);
			if (x1 == x2 && y1 == y2) {
				break;
			}
			e2 = err;
			if (e2 > -dx) {
				err -= dy;
				x1 += sx;
			}
			if (e2 < dy) {
				err += dx;
				y1 += sy;
			}
		}
	}
    osSemaphoreRelease(*(self->busySemaphore));
}

static void fillWindow(LcdHandler* self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) 
{
	if (osErrorOS == osSemaphoreWait(*(self->busySemaphore), self->timeoutMs)) {
		printf("lcdHandler fillWindow: timeout\n");
		return;
	}
	self->lcd->fillWindow(self->lcd, x, y, x + width - 1, y + height - 1, color);
	osSemaphoreRelease(*(self->busySemaphore));
}

static void image(LcdHandler* self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color* colors) 
{
	if (osErrorOS == osSemaphoreWait(*(self->busySemaphore), self->timeoutMs)) {
		printf("lcdHandler image: timeout\n");
		return;
	}
	self->lcd->image(self->lcd, x, y, width, height, colors);
	osSemaphoreRelease(*(self->busySemaphore));
}

LcdHandler lcdInstance = {
	.lcd = &st7789vInstance,
	.busySemaphore = &lcdBusySemaphoreHandle,
	.timeoutMs = 100,
	.init = init,
	.deinit = deinit,
	.point = point,
	.line = line,
	.fillWindow = fillWindow,
	.image = image
};