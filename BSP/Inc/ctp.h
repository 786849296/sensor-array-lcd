#pragma once

#include "main.h"

typedef enum {
	NONE = 0,
	SLIDE_UP,
	SLIDE_DOWN,
	SLIDE_LEFT,
	SLIDE_RIGHT,
	SLIDE_CLICK,
	DOUBLE_CLICK,
	PRESS,
}GESTURE;

typedef enum {
	STATE_FIRST = 0x00,
	STATE_PRESS = 0x80,
	STATE_RELEASE = 0x40,
}TOUCH_STATE;

typedef struct ctpI {
	void (*init)(struct ctp*);
	void (*deinit)(struct ctp*);
	void (*getAll)(struct ctp*);
	uint16_t x;
	uint16_t y;
	GESTURE gesture;
	uint8_t fingerNum;
	TOUCH_STATE state;
}CtpI;

/*
 * @brief: 
 *  1、 报点模式
 *	只要有触摸，触摸 IC 就会每隔 10ms 左右拉一次中断，主控更据中断读取实时坐标。上
 *	下左右滑动由主控更据读到连续坐标值进行判断处理。有按下抬起状态码。
 *	2、 手势模式
 *	触摸 IC 更据用户触摸轨迹识别成指定手势码（单击，双击，长按，上下左右滑动），当
 *	手势码生成时，会拉一次中断，主控更据中断读取生成的手势码。
 */
typedef enum {
	CONTINUOUS_MODE = 0x60, /**< 报点模式 */
	GESTURE_MODE = 0x11,		/**< 手势模式 */
	MIX_MODE = 0x71,			/**< 混合模式 */
}CST816T_MODE;

typedef struct cst816t {
	CtpI ctpI;
	I2C_TypeDef* I2Cx;
	Gpio RST;
	Gpio INT;
	uint8_t addr;
	CST816T_MODE mode;
}Cst816t;

Cst816t* Cst816tConstruct(I2C_TypeDef* I2Cx, Gpio RST, Gpio INT, uint8_t addr, CST816T_MODE mode);

extern Cst816t cst816tInstance;