#include "sensorSwitchHandler.h"

static void init(
	SensorSwitch* self,
	SensorSwitchI** rowSwitchs,
	uint8_t maxRowSwitchsCount,
	SensorSwitchI** colSwitchs,
	uint8_t maxColSwitchsCount,
	void (*setRow)(SensorSwitch*, uint8_t),
	void (*setCol)(SensorSwitch*, uint8_t))
{
	self->rowSwitchs = rowSwitchs;
	self->maxRowSwitchsCount = maxRowSwitchsCount;
	self->colSwitchs = colSwitchs;
	self->maxColSwitchsCount = maxColSwitchsCount;
	self->setRow = setRow;
	self->setCol = setCol;
}

static void deinit(SensorSwitch* self)
{
	for (int i = 0; i < self->maxRowSwitchsCount; i++)
		self->rowSwitchs[i]->deinit(self->rowSwitchs[i]);
	for (int i = 0; i < self->maxColSwitchsCount; i++)
		self->colSwitchs[i]->deinit(self->colSwitchs[i]);
}

static void setRow(SensorSwitch* self, uint8_t row)
{
	assert_param(row < self->maxRowSwitchsCount * self->rowSwitchs[0]->maxChannel);
	// 默认所有开关maxChannel一致
	int rowSwitchIndex = row / self->rowSwitchs[0]->maxChannel;
	for (int i = 0; i < self->maxRowSwitchsCount; i++)
		if (i == rowSwitchIndex)
		{
			self->rowSwitchs[i]->setState(self->rowSwitchs[i], ENABLE);
			self->rowSwitchs[i]->setChannel(self->rowSwitchs[i], row % self->rowSwitchs[i]->maxChannel);
		}
		else
			self->rowSwitchs[i]->setState(self->rowSwitchs[i], DISABLE);
}

static void setCol(SensorSwitch* self, uint8_t col)
{
	assert_param(col < self->maxColSwitchsCount * self->colSwitchs[0]->maxChannel);
	// 默认所有开关maxChannel一致
	int colSwitchIndex = col / self->colSwitchs[0]->maxChannel;
	for (int i = 0; i < self->maxColSwitchsCount; i++)
		if (i == colSwitchIndex)
		{
			self->colSwitchs[i]->setState(self->colSwitchs[i], ENABLE);
			self->colSwitchs[i]->setChannel(self->colSwitchs[i], col % self->colSwitchs[i]->maxChannel);
		}
		else
			self->colSwitchs[i]->setState(self->colSwitchs[i], DISABLE);
}

// 静态初始化
SensorSwitch sensorSwitchInstance = {
	.row = 0,
	.col = 0,
	.rowSwitchs = rowSwitchs,
	.maxRowSwitchsCount = 1,
	.colSwitchs = colSwitchs,
	.maxColSwitchsCount = 4,
	.init = init,
	.deinit = deinit,
	.setRow = setRow,
	.setCol = setCol
};