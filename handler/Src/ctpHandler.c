#include "ctpHandler.h"

static void init(
	CtpHandler* self,
	struct ctpI* ctp,
	osSemaphoreId* busySemaphore)
{
	self->ctp = ctp;
	self->busySemaphore = busySemaphore;
	self->ctp->init(self->ctp);
	osSemaphoreRelease(*(self->busySemaphore));
}

static void deinit(CtpHandler* self)
{
	osSemaphoreWait(*(self->busySemaphore), osWaitForever);
	self->ctp->deinit(self->ctp);
	osSemaphoreDelete(*(self->busySemaphore));
}

static GESTURE getGesture(CtpHandler* self)
{
	osSemaphoreWait(*(self->busySemaphore), osWaitForever);
	self->ctp->getAll(self->ctp);
	osSemaphoreRelease(*(self->busySemaphore));
	return self->ctp->gesture;
}

static void getPos(CtpHandler* self, uint16_t* x, uint16_t* y)
{
	osSemaphoreWait(*(self->busySemaphore), osWaitForever);
	self->ctp->getAll(self->ctp);
	osSemaphoreRelease(*(self->busySemaphore));
	*x = self->ctp->x;
	*y = self->ctp->y;
}

CtpHandler ctpHandler = {
	.init = init,
	.deinit = deinit,
	.getGesture = getGesture,
	.getPos = getPos
};