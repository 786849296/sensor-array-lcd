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
 *  1�� ����ģʽ
 *	ֻҪ�д��������� IC �ͻ�ÿ�� 10ms ������һ���жϣ����ظ����ж϶�ȡʵʱ���ꡣ��
 *	�����һ��������ظ��ݶ�����������ֵ�����жϴ����а���̧��״̬�롣
 *	2�� ����ģʽ
 *	���� IC �����û������켣ʶ���ָ�������루������˫�����������������һ���������
 *	����������ʱ������һ���жϣ����ظ����ж϶�ȡ���ɵ������롣
 */
typedef enum {
	CONTINUOUS_MODE = 0x60, /**< ����ģʽ */
	GESTURE_MODE = 0x11,		/**< ����ģʽ */
	MIX_MODE = 0x71,			/**< ���ģʽ */
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