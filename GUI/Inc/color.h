#pragma once

#include <main.h>

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef enum
{
    BLACK = 0x000000,
    RED = 0xff0000,
    DARK_RED = 0x8b0000,
    YELLOW = 0xffff00,
    GREEN = 0x00ff00,
    CYAN = 0x00ffff,
    BLUE = 0x0000ff,
    DARK_BLUE = 0x00008b,
    WHITE = 0xffffff,
}COLOR;

__STATIC_INLINE Color int2Color(int int32)
{
    Color color = {
		(uint8_t)(int32 >> 16),
		(uint8_t)(int32 >> 8),
		(uint8_t)int32
	};
	return color;
}

extern Color linearGradientColors[6];
extern int linearGradientColorsCount;

Color getLinearGradientColor(float offset);

extern float legendRange;

__STATIC_INLINE Color adc2Color(uint16_t adcValue)
{
	float offset = adcValue / legendRange;
    if (offset < 0.01)
        return int2Color(WHITE);
    else if (offset > 1)
		offset = 1;
    return getLinearGradientColor(offset);
}