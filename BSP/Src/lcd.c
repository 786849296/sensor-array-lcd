#include "lcd.h"

typedef enum 
{
    CMD = 0,
    DATA = 1
}DC;

/**
 * @brief private function
 *        д��8λ���ݵ�ST7789V������
 * @note  ָ��+���ݲ�Ϊһ�����ڣ�����������cs
 * 
 * @param self ST7789V����ָ��
 * @param dc ����/����ѡ��
 * @param data Ҫд�������
 */
static void _St7789vWrite8(St7789v* self, DC dc, uint8_t data)
{
    if (CMD == dc)
    { 
        // TODO��NSS�����Ż�
        if (!LL_GPIO_IsOutputPinSet(self->spiI->NSS.group, self->spiI->NSS.pin))
        {
            while (LL_SPI_IsActiveFlag_BSY(self->spiI->SPIx));
            LL_GPIO_SetOutputPin(self->spiI->NSS.group, self->spiI->NSS.pin);
        }
        // dc_pinҪ��cs_pin֮ǰ����
        LL_GPIO_ResetOutputPin(self->DC.group, self->DC.pin);
        LL_GPIO_ResetOutputPin(self->spiI->NSS.group, self->spiI->NSS.pin);
    }
    else
        LL_GPIO_SetOutputPin(self->DC.group, self->DC.pin);
    self->spiI->writeRead8(self->spiI, data);
}

static void St7789vInit(St7789v* self)
{
    // ǰ�����Ƿ��б�Ҫ��
    LL_GPIO_SetOutputPin(self->RST.group, self->RST.pin);
    osDelay(100);
    LL_GPIO_ResetOutputPin(self->RST.group, self->RST.pin);
    osDelay(100);
    LL_GPIO_SetOutputPin(self->RST.group, self->RST.pin);
    osDelay(100);
    
    // 1. ��ʾ����
    _St7789vWrite8(self, CMD, 0x36);
    _St7789vWrite8(self, DATA, self->orientation);

    // 2. ɫ��ģʽ
    _St7789vWrite8(self, CMD, 0x3A);
    _St7789vWrite8(self, DATA, self->colorMode);

    // 3. ǰ�������
    _St7789vWrite8(self, CMD, 0xB2);
    _St7789vWrite8(self, DATA, 0x0C);
    _St7789vWrite8(self, DATA, 0x0C);
    _St7789vWrite8(self, DATA, 0x00);
    _St7789vWrite8(self, DATA, 0x33);
    _St7789vWrite8(self, DATA, 0x33);

    // 4. ��Դ����
    _St7789vWrite8(self, CMD, 0xB7);
    _St7789vWrite8(self, DATA, 0x11);
    _St7789vWrite8(self, CMD, 0xBB);
    _St7789vWrite8(self, DATA, 0x35);
    _St7789vWrite8(self, CMD, 0xD0);
    _St7789vWrite8(self, DATA, 0xA4);
    _St7789vWrite8(self, DATA, 0xA1);

    // 5. ������ʾЧ��
    //_st7789vWrite8(self, CMD, 0xC0);
    //_st7789vWrite8(self, DATA, 0x2C);

    // 6. ����VRH��VDV
    _St7789vWrite8(self, CMD, 0xC2);
    _St7789vWrite8(self, DATA, 0x01);
    _St7789vWrite8(self, DATA, 0xFF);
    _St7789vWrite8(self, CMD, 0xC3);
    _St7789vWrite8(self, DATA, 0x0D);
    _St7789vWrite8(self, CMD, 0xC4);
    _St7789vWrite8(self, DATA, 0x20);

    // 7. ����֡��
    _St7789vWrite8(self, CMD, 0xC6);
    _St7789vWrite8(self, DATA, self->frameRate);

    // δָ֪��0xD6
    // 8. gamma����
    _St7789vWrite8(self, CMD, 0xE0);
    _St7789vWrite8(self, DATA, 0xF0);
    _St7789vWrite8(self, DATA, 0x06);
    _St7789vWrite8(self, DATA, 0x0B);
    _St7789vWrite8(self, DATA, 0x0A);
    _St7789vWrite8(self, DATA, 0x09);
    _St7789vWrite8(self, DATA, 0x26);
    _St7789vWrite8(self, DATA, 0x29);
    _St7789vWrite8(self, DATA, 0x33);
    _St7789vWrite8(self, DATA, 0x41);
    _St7789vWrite8(self, DATA, 0x18);
    _St7789vWrite8(self, DATA, 0x16);
    _St7789vWrite8(self, DATA, 0x15);
    _St7789vWrite8(self, DATA, 0x29);
    _St7789vWrite8(self, DATA, 0x2D);
    _St7789vWrite8(self, CMD, 0xE1);
    _St7789vWrite8(self, DATA, 0xF0);
    _St7789vWrite8(self, DATA, 0x04);
    _St7789vWrite8(self, DATA, 0x08);
    _St7789vWrite8(self, DATA, 0x08);
    _St7789vWrite8(self, DATA, 0x07);
    _St7789vWrite8(self, DATA, 0x03);
    _St7789vWrite8(self, DATA, 0x28);
    _St7789vWrite8(self, DATA, 0x32);
    _St7789vWrite8(self, DATA, 0x40);
    _St7789vWrite8(self, DATA, 0x3B);
    _St7789vWrite8(self, DATA, 0x19);
    _St7789vWrite8(self, DATA, 0x18);
    _St7789vWrite8(self, DATA, 0x2A);
    _St7789vWrite8(self, DATA, 0x2E);

    // 9. �����ſ�
    _St7789vWrite8(self, CMD, 0xE4);
    _St7789vWrite8(self, DATA, 0x25);
    _St7789vWrite8(self, DATA, 0x00);
    _St7789vWrite8(self, DATA, 0x00);

    //// 10. 垂直滚动
    //_st7789vWrite8(self, CMD, 0x33);
    //_st7789vWrite8(self, DATA, 0x00);
    //_st7789vWrite8(self, DATA, 0x00);
    //_st7789vWrite8(self, DATA, 0x01);
    //_st7789vWrite8(self, DATA, 0x40);
    //_st7789vWrite8(self, DATA, 0x00);
    //_st7789vWrite8(self, DATA, 0x00);
    //_st7789vWrite8(self, CMD, 0x37);
    //_st7789vWrite8(self, DATA, 0x00);
    //_st7789vWrite8(self, DATA, 0x14);

    // 11. �ӷ�תģʽ�ָ�
    _St7789vWrite8(self, CMD, 0x21);

    // 12. �˳�˯��ģʽ
    _St7789vWrite8(self, CMD, 0x11);
    osDelay(120);

    // 13. ��ʾ����
    _St7789vWrite8(self, CMD, 0x29);
}

/**
 * @brief st7789vȥ��ʼ��
 * @note  spiʵ����������ͷ�
 *
 * @param self ST7789V����ָ��
 */
static void St7789vDeinit(St7789v* self)
{
    vPortFree(self->interface.buffer);
    vPortFree(self);
}

static void _St7789vSetWindow(St7789v* self, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    assert_param(x0 >= 0 && x0 < self->interface.width);
    assert_param(y0 >= 0 && y0 < self->interface.height);
    assert_param(x1 >= 0 && x1 < self->interface.width);
    assert_param(y1 >= 0 && y1 < self->interface.height);
    x0 += self->offsetX;
    y0 += self->offsetY;
    x1 += self->offsetX;
    y1 += self->offsetY;

	_St7789vWrite8(self, CMD, 0x2A);
	_St7789vWrite8(self, DATA, x0 >> 8);
	_St7789vWrite8(self, DATA, x0 & 0xff);
	_St7789vWrite8(self, DATA, x1 >> 8);
	_St7789vWrite8(self, DATA, x1 & 0xff);

	_St7789vWrite8(self, CMD, 0x2B);
	_St7789vWrite8(self, DATA, y0 >> 8);
	_St7789vWrite8(self, DATA, y0 & 0xff);
	_St7789vWrite8(self, DATA, y1 >> 8);
	_St7789vWrite8(self, DATA, y1 & 0xff);
}

static void St7789vPoint(St7789v* self, uint16_t x, uint16_t y, Color color)
{
	_St7789vSetWindow(self, x, y, x, y);
	_St7789vWrite8(self, CMD, 0x2C);
    switch (self->colorMode)
    {
        case RGB444:
			color.r &= 0xf0;
            color.g &= 0xf0;
            color.b &= 0xf0;
            _St7789vWrite8(self, DATA, color.r | (color.g >> 4));
            _St7789vWrite8(self, DATA, color.b);
			break;
		case RGB565:
            color.r &= 0xf8;
            color.g &= 0xfc;
            color.b &= 0xf8;
            _St7789vWrite8(self, DATA, color.r | (color.g >> 5));
            _St7789vWrite8(self, DATA, (color.g << 3) | color.b);
			break;
		case RGB666:
			color.r &= 0xfc;
			color.g &= 0xfc;
			color.b &= 0xfc;
			_St7789vWrite8(self, DATA, color.r);
			_St7789vWrite8(self, DATA, color.g);
			_St7789vWrite8(self, DATA, color.b);
			break;
    }
}

static void St7789vFillWindow(St7789v* self, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color)
{
    uint32_t length = 0;
    int cnt = 0;
    switch (self->colorMode)
    {
    case RGB444:
        for (cnt = 0; cnt < (x1 - x0 + 1) * (y1 - y0 + 1) && cnt < self->interface.bufferLength / 3; cnt += 2)
        {
            self->interface.buffer[length++] = (color.r & 0xf0) | (color.g >> 4);
            self->interface.buffer[length++] = (color.b & 0xf0) | (color.r >> 4);
            self->interface.buffer[length++] = (color.g & 0xf0) | (color.b >> 4);
        }
        break;
    case RGB565:
        for (cnt = 0; cnt < (x1 - x0 + 1) * (y1 - y0 + 1) && cnt < self->interface.bufferLength / 2; cnt++)
        {
            self->interface.buffer[length++] = (color.r & 0xf8) | (color.g & 0xe0 >> 5);
            self->interface.buffer[length++] = (color.g & 0x1c << 3) | (color.b & 0xf8 >> 3);
        }
        break;
    case RGB666:
        for (cnt = 0; cnt < (x1 - x0 + 1) * (y1 - y0 + 1) && cnt < self->interface.bufferLength / 3; cnt++)
        {
            self->interface.buffer[length++] = color.r & 0xfc;
            self->interface.buffer[length++] = color.g & 0xfc;
            self->interface.buffer[length++] = color.b & 0xfc;
        }
        break;
    }
    _St7789vSetWindow(self, x0, y0, x1, y1);
    _St7789vWrite8(self, CMD, 0x2C);
    LL_GPIO_SetOutputPin(self->DC.group, self->DC.pin);
    int i = 0;
    //for (; i < (x1 - x0 + 1) * (y1 - y0 + 1); i++)
    //{
    //    _st7789vWrite8(self, DATA, self->interface.buffer[0]);
    //    _st7789vWrite8(self, DATA, self->interface.buffer[1]);
    //    _st7789vWrite8(self, DATA, self->interface.buffer[2]);
    //    LL_IWDG_ReloadCounter(IWDG);
    //}
    for (; i < (x1 - x0 + 1) * (y1 - y0 + 1) / cnt; i++)
        self->spiI->dmaWrite(self->spiI, self->interface.buffer, length, 100);
    if ((x1 - x0 + 1) * (y1 - y0 + 1) != i * cnt)
    {
        switch (self->colorMode)
        {
        case RGB444:
            length = ((x1 - x0 + 1) * (y1 - y0 + 1) - (i - 1) * cnt) * 2 / 3;
            break;
        case RGB565:
            length = ((x1 - x0 + 1) * (y1 - y0 + 1) - (i - 1) * cnt) * 2;
            break;
        case RGB666:
            length = ((x1 - x0 + 1) * (y1 - y0 + 1) - (i - 1) * cnt) * 3;
            break;
        }
        self->spiI->dmaWrite(self->spiI, self->interface.buffer, length, 100);
    }
}


/**
 * @brief st7789vImage函数用于在ST7789V显示屏上绘制图像。
 * @deprecated 该函数已经被废弃，不建议使用。不适合小内存mcu。
 *
 * @param self ST7789V结构体指针
 * @param x0 图像起始点的x坐标
 * @param y0 图像起始点的y坐标
 * @param x1 图像结束点的x坐标
 * @param y1 图像结束点的y坐标
 * @param image 图像数据数组指针
 */
static void St7789vImage(St7789v* self, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color* image)
{
    _St7789vSetWindow(self, x0, y0, x1, y1);
	_St7789vWrite8(self, CMD, 0x2C);
    uint32_t length = 0;
	switch (self->colorMode)
	{
		case RGB444:
            for (int i = 0; i < (x1 - x0 + 1) * (y1 - y0 + 1); i+=2)
			{
				self->interface.buffer[length++] = (image[i].r & 0xf0) | (image[i].g >> 4);
                self->interface.buffer[length++] = (image[i].b & 0xf0) | (image[i + 1].r >> 4);
                self->interface.buffer[length++] = (image[i + 1].g & 0xf0) | (image[i + 1].b >> 4);
			}
			break;
		case RGB565:
            for (int i = 0; i < (x1 - x0 + 1) * (y1 - y0 + 1); i++)
            {
                self->interface.buffer[length++] = (image[i].r & 0xf8) | (image[i].g & 0xe0 >> 5);
                self->interface.buffer[length++] = (image[i].g & 0x1c << 3) | (image[i].b & 0xf8 >> 3);
            }
			break;
		case RGB666:
            for (int i = 0; i < (x1 - x0 + 1) * (y1 - y0 + 1); i++)
            {
                self->interface.buffer[length++] = image[i].r & 0xfc;
                self->interface.buffer[length++] = image[i].g & 0xfc;
                self->interface.buffer[length++] = image[i].b & 0xfc;
            }
			break;
    }
    LL_GPIO_SetOutputPin(self->DC.group, self->DC.pin);
    self->spiI->dmaWrite(self->spiI, self->interface.buffer, length, 100);
}

St7789v* St7789vConstruct(
    uint16_t width,
    uint16_t height,
    Spi* spiI,
    Gpio DC,
    Gpio RST,
    ST7789V_ORIENTATION orientation,
    ST7789V_COLORMODE colorMode,
    ST7789V_FRAMERATE frameRate,
    uint16_t offsetX,
    uint16_t offsetY)
{
    St7789v* st7789v = pvPortMalloc(sizeof(St7789v));
    if (NULL == st7789v)
    {
        printf("st7789v malloc failed\n");
        goto e1;
    }
	st7789v->interface.init = St7789vInit;
	st7789v->interface.deinit = St7789vDeinit;
	st7789v->interface.fillWindow = St7789vFillWindow;
    st7789v->interface.image = St7789vImage;
	st7789v->interface.point = St7789vPoint;
	st7789v->interface.width = width;
	st7789v->interface.height = height;
	st7789v->spiI = spiI;
	st7789v->DC = DC;
	st7789v->RST = RST;
	st7789v->orientation = orientation;
	st7789v->colorMode = colorMode;
	st7789v->frameRate = frameRate;
    st7789v->offsetX = offsetX;
    st7789v->offsetY = offsetY;
    st7789v->interface.buffer = pvPortMalloc(sizeof(Color) * width * height);
    if (NULL == st7789v)
    {
        printf("st7789v buffer malloc failed\n");
        goto e2;
    }
    st7789v->interface.init(st7789v);
	return st7789v;

e2:
    vPortFree(st7789v);
e1:
	return NULL;
}



static uint8_t _st7789v_buffer[240 * 3];

St7789v st7789vInstance = {
    .interface = {
        .init = St7789vInit,
        .deinit = St7789vDeinit,
        .fillWindow = St7789vFillWindow,
        .image = St7789vImage,
        .point = St7789vPoint,
        .buffer = _st7789v_buffer,
        .bufferLength = 240 * 3,
        .width = 240,
        .height = 280,
    },
    .spiI = &SPI2Instance,
    .DC = {GPIOA, LL_GPIO_PIN_8},
    .RST = {GPIOB, LL_GPIO_PIN_5},
    .orientation = VERTICAL,
    .colorMode = RGB666,
    .frameRate = _60Hz,
    .offsetX = 0,
    .offsetY = 20,
};
