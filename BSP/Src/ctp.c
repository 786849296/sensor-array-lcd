#include "ctp.h"

static void _Cst816tWrite(Cst816t* ctp, uint8_t reg, uint8_t* data, int len) {
	LL_I2C_GenerateStartCondition(ctp->I2Cx);
	while (!LL_I2C_IsActiveFlag_SB(ctp->I2Cx));
	LL_I2C_TransmitData8(ctp->I2Cx, ctp->addr << 1);
	while (!LL_I2C_IsActiveFlag_ADDR(ctp->I2Cx));
	LL_I2C_ClearFlag_ADDR(ctp->I2Cx);
	LL_I2C_TransmitData8(ctp->I2Cx, reg);
	while (!LL_I2C_IsActiveFlag_TXE(ctp->I2Cx));
	for (int i = 0; i < len; i++)
	{
		LL_I2C_TransmitData8(ctp->I2Cx, data);
		while (!LL_I2C_IsActiveFlag_TXE(ctp->I2Cx));
	}
	LL_I2C_GenerateStopCondition(ctp->I2Cx);
}

static void _Cst816tRead(Cst816t* ctp, uint8_t reg, uint8_t* data, int len) {
	LL_I2C_GenerateStartCondition(ctp->I2Cx);
	while (!LL_I2C_IsActiveFlag_SB(ctp->I2Cx));
	LL_I2C_TransmitData8(ctp->I2Cx, ctp->addr << 1);
	while (!LL_I2C_IsActiveFlag_ADDR(ctp->I2Cx));
	LL_I2C_ClearFlag_ADDR(ctp->I2Cx);
	LL_I2C_TransmitData8(ctp->I2Cx, reg);
	while (!LL_I2C_IsActiveFlag_TXE(ctp->I2Cx));
	LL_I2C_TransmitData8(ctp->I2Cx, (ctp->addr << 1) | 1);
	while (!LL_I2C_IsActiveFlag_ADDR(ctp->I2Cx));
	LL_I2C_ClearFlag_ADDR(ctp->I2Cx);
	for (int i = 0; i < len; i++)
	{
		while (!LL_I2C_IsActiveFlag_RXNE(ctp->I2Cx));
		data[i] = LL_I2C_ReceiveData8(ctp->I2Cx);
	}
	LL_I2C_GenerateStopCondition(ctp->I2Cx);
}

static void Cst816tInit(Cst816t* ctp) {
	LL_GPIO_ResetOutputPin(ctp->RST.group, ctp->RST.pin);
	osDelay(10);
	LL_GPIO_SetOutputPin(ctp->RST.group, ctp->RST.pin);
	osDelay(50);

	uint8_t id;
	_Cst816tRead(ctp, 0xA7, &id, 1);
	printf("cst816t ID: %d\n", id);
	assert_param(id == 0xb5);
	
	_Cst816tWrite(ctp, 0xfa, (uint8_t[]){ctp->mode}, 1);
	//_Cst816tWrite(ctp, 0xfe, (uint8_t[]){1}, 1);
}

static void Cst816tDeinit(Cst816t* ctp) {
	vPortFree(ctp);
}

static void Cst816tGetAll(Cst816t* ctp) {
	uint8_t data[6];
	_Cst816tRead(ctp, 0x01, data, 6);
	ctp->ctpI.gesture = data[0];
	ctp->ctpI.fingerNum = data[1];
	ctp->ctpI.state = data[2] & 0xf0;
	ctp->ctpI.x = (uint16_t)(data[2] & 0x0f) << 8 | data[3];
	ctp->ctpI.y = (uint16_t)(data[4] & 0x0f) << 8 | data[5];
}

Cst816t* Cst816tConstruct(I2C_TypeDef* I2Cx, Gpio RST, Gpio INT, uint8_t addr, CST816T_MODE mode)
{
	Cst816t* ctp = (Cst816t*)pvPortMalloc(sizeof(Cst816t));
	ctp->ctpI.init = Cst816tInit;
	ctp->ctpI.deinit = Cst816tDeinit;
	ctp->ctpI.getAll = Cst816tGetAll;
	ctp->I2Cx = I2Cx;
	ctp->RST = RST;
	ctp->INT = INT;
	ctp->addr = addr;
	ctp->mode = mode;
	ctp->ctpI.init(ctp);
	return ctp;
}

Cst816t cst816tInstance = {
	.ctpI = {
		.init = Cst816tInit,
		.deinit = Cst816tDeinit,
		.getAll = Cst816tGetAll,
	},
	.I2Cx = I2C2,
	.RST = {
		.group = GPIOA,
		.pin = LL_GPIO_PIN_15,
	},
	.INT = {
		.group = GPIOA,
		.pin = LL_GPIO_PIN_5,
	},
	.addr = 0x15,
	.mode = GESTURE_MODE,
};