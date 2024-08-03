#pragma once

#include "main.h"
#include "weight.h"

/**
 * @brief 布局接口定义
 * 
 * 该接口也是布局canvas的定义
 */
typedef struct layoutI {
    WeightI interface; 
    WeightI** items; 
    int itemsCount; 
    void (*add)(struct layoutI*, WeightI*);
    void (*remove)(struct layoutI*, WeightI*);
} LayoutI;

typedef struct grid {
    LayoutI interface;
    uint16_t* rowDef; 
    uint16_t* colDef; 
    uint8_t* itemsPos;
    // flow
} Grid;

LayoutI* canvasConstruct(
    WeightI* parent,
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    Color background,
    WeightI** items, 
    int itemsCount);
Grid* gridConstruct(
    WeightI* parent,
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    Color background,
    WeightI** items,
    int itemsCount,
    uint16_t* rowDef, 
    uint16_t* colDef, 
    uint8_t* itemsPos);