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
 * 输入时都传输RGB888
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
 * @brief 枚举类型，表示ST7789V的帧率。
 *
 * 该枚举类型用于选择LCD的帧率。注意，帧率的值与FPA和BPA的默认值相对应。
 * 最低帧率为39帧（0x1F），最高帧率为119帧（0x00）。
 * 如果需要更多的选项，请查阅文档。
 */
typedef enum
{
    _40Hz = 0x1E, /**< 40帧/秒的帧率。 */
    _50Hz = 0x15, /**< 50帧/秒的帧率。 */
    _60Hz = 0x0F, /**< 60帧/秒的帧率。 */
    _75Hz = 0x09, /**< 75帧/秒的帧率。 */
    _90Hz = 0x05, /**< 90帧/秒的帧率。 */
    _119Hz = 0x00 /**< 119帧/秒的帧率。 */
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