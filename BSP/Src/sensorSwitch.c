#include "sensorSwitch.h"

static void Cd4051Deinit(Cd4051* self)
{
	vPortFree(self);
}

static void Cd4051SetState(Cd4051* self, FunctionalState state)
{
	if (self->INH == NULL)
		return;
	if (ENABLE == state)
		LL_GPIO_ResetOutputPin(self->INH->group, self->INH->pin);
	else
		LL_GPIO_SetOutputPin(self->INH->group, self->INH->pin);
}

static void Cd4051SetChannel(Cd4051* self, uint8_t channel)
{
	for (int i = 0; i < 3; i++)
	{
		if (channel & 0x01)
			LL_GPIO_SetOutputPin(self->ABC[i].group, self->ABC[i].pin);
		else
			LL_GPIO_ResetOutputPin(self->ABC[i].group, self->ABC[i].pin);
		channel >>= 1;
	}
}

Cd4051* Cd4051Construct(Gpio* ABC, Gpio* INH)
{
	Cd4051* self = (Cd4051*)pvPortMalloc(sizeof(Cd4051));
	assert_param(self != NULL);

	self->ABC = ABC;
	self->INH = INH;
	self->interface.deinit = Cd4051Deinit;
	self->interface.setState = Cd4051SetState;
	self->interface.setChannel = Cd4051SetChannel;
	self->interface.maxChannel = 8;
	return self;
}

// 静态实例
static Gpio ABCRow[3] = {
	{GPIOB, LL_GPIO_PIN_0},
	{GPIOB, LL_GPIO_PIN_1},
	{GPIOB, LL_GPIO_PIN_2}
};

static Gpio ABCCol[3] = {
	{GPIOB, LL_GPIO_PIN_6},
	{GPIOB, LL_GPIO_PIN_7},
	{GPIOB, LL_GPIO_PIN_8}
};

static Gpio INHCol[4] = {
	{GPIOB, LL_GPIO_PIN_9},
	{GPIOC, LL_GPIO_PIN_13},
	{GPIOC, LL_GPIO_PIN_14},
	{GPIOC, LL_GPIO_PIN_15}
};

static Cd4051 cd4051Row[1] = {
	{
		.interface = {
			.deinit = Cd4051Deinit,
			.setState = Cd4051SetState,
			.setChannel = Cd4051SetChannel,
			.maxChannel = 8
		},
		.ABC = ABCRow,
		.INH = NULL
	}
};

static Cd4051 cd4051Col[4] = {
	{
		.interface = {
			.deinit = Cd4051Deinit,
			.setState = Cd4051SetState,
			.setChannel = Cd4051SetChannel,
			.maxChannel = 8
		},
		.ABC = ABCCol,
		.INH = &INHCol[0],
	}, {
		.interface = {
			.deinit = Cd4051Deinit,
			.setState = Cd4051SetState,
			.setChannel = Cd4051SetChannel,
			.maxChannel = 8
		},
		.ABC = ABCCol,
		.INH = &INHCol[1],
	}, {
		.interface = {
			.deinit = Cd4051Deinit,
			.setState = Cd4051SetState,
			.setChannel = Cd4051SetChannel,
			.maxChannel = 8
		},
		.ABC = ABCCol,
		.INH = &INHCol[2],
	}, {
		.interface = {
			.deinit = Cd4051Deinit,
			.setState = Cd4051SetState,
			.setChannel = Cd4051SetChannel,
			.maxChannel = 8
		},
		.ABC = ABCCol,
		.INH = &INHCol[3],
	}
};

SensorSwitchI* rowSwitchs[1] = {
	(SensorSwitchI*)&cd4051Row[0]
};

SensorSwitchI* colSwitchs[4] = {
	(SensorSwitchI*)&cd4051Col[0],
	(SensorSwitchI*)&cd4051Col[1],
	(SensorSwitchI*)&cd4051Col[2],
	(SensorSwitchI*)&cd4051Col[3]
};
