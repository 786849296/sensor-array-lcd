#pragma once

#include "main.h"
#include "spi.h"

typedef struct lcdI {
    void (*init)(struct lcdI*);
    void (*deinit)(struct lcdI*);
    void (*point)(struct lcdI*, uint16_t, uint16_t, Color);
    void (*image)(struct lcdI*, uint16_t, uint16_t, uint16_t, uint16_t, Color*);
    void (*fillWindow)(struct lcdI*, uint16_t, uint16_t, uint16_t, uint16_t, Color);
    uint16_t width;
    uint16_t height;
    uint8_t* buffer;
    uint32_t bufferLength;
} LcdI;



/**
 * @brief Enum representing the color mode for ST7789V.
 * ����ʱ������RGB888
 */
typedef enum
{
    RGB444 = 0x03, /**< RGB444 color mode, where each pixel occupies 1.5 bytes. */
    RGB565 = 0x05, /**< RGB565 color mode, where each pixel occupies 2 bytes. */
    RGB666 = 0x06  /**< RGB666 color mode, where each pixel occupies 3 bytes. */
} ST7789V_COLORMODE;

typedef enum
{
    HORIZONTAL = 0x70,
    VERTICAL = 0x00,
} ST7789V_ORIENTATION;


/**
 * @brief ö�����ͣ���ʾST7789V��֡�ʡ�
 *
 * ��ö����������ѡ��LCD��֡�ʡ�ע�⣬֡�ʵ�ֵ��FPA��BPA��Ĭ��ֵ���Ӧ��
 * ���֡��Ϊ39֡��0x1F�������֡��Ϊ119֡��0x00����
 * �����Ҫ�����ѡ�������ĵ���
 */
typedef enum
{
    _40Hz = 0x1E, /**< 40֡/���֡�ʡ� */
    _50Hz = 0x15, /**< 50֡/���֡�ʡ� */
    _60Hz = 0x0F, /**< 60֡/���֡�ʡ� */
    _75Hz = 0x09, /**< 75֡/���֡�ʡ� */
    _90Hz = 0x05, /**< 90֡/���֡�ʡ� */
    _119Hz = 0x00 /**< 119֡/���֡�ʡ� */
} ST7789V_FRAMERATE;

typedef struct st7789v {
    LcdI interface;
    Spi* spiI;
    Gpio DC;
    Gpio RST;
    ST7789V_ORIENTATION orientation;
    ST7789V_COLORMODE colorMode;
    ST7789V_FRAMERATE frameRate;
    uint16_t offsetX;
    uint16_t offsetY;
} St7789v;

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
    uint16_t offsetY);

extern St7789v st7789vInstance;