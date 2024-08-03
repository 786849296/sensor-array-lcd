#include "color.h"

Color linearGradientColors[6] = {
	{ 0x00,0x00,0x8b }, // DARK_BLUE
	{ 0x00,0x00,0xff }, // BLUE
	{ 0x00,0xff,0xff }, // CYAN
	{ 0xff,0xff,0x00 }, // YELLOW
	{ 0xff,0x00,0x00 }, // RED
	{ 0x8b,0x00,0x00 }  // DARK_RED
};
int linearGradientColorsCount = 6;

Color getLinearGradientColor(float offset)
{
	assert_param(0 <= offset && offset <= 1);
	int index = offset * (linearGradientColorsCount - 1);
	float subOffset = offset * (linearGradientColorsCount - 1) - index;
	if (0 == subOffset)
		return linearGradientColors[index];
	Color color = {
		linearGradientColors[index].r + (uint8_t)((linearGradientColors[index + 1].r - linearGradientColors[index].r) * subOffset),
		linearGradientColors[index].g + (uint8_t)((linearGradientColors[index + 1].g - linearGradientColors[index].g) * subOffset),
		linearGradientColors[index].b + (uint8_t)((linearGradientColors[index + 1].b - linearGradientColors[index].b) * subOffset)
	};
	return color;
}

float legendRange = 4095;
