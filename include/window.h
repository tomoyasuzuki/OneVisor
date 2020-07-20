#pragma once
#include <stdint.h>
#include "frameBuffer.h"
#include "color.h"

typedef struct Window {
    uint64_t* baseAddr;
    uint64_t size;
    uint32_t width, height;
    uint32_t pixelsPerScanLine;
    enum PixelFormat format;
} Window;

void init_graphic(Window *w);

void draw_pixel_with_color(uint32_t x, uint32_t y, PixelColor color);

void draw_pixel(uint32_t x, uint32_t y);

void fill_window(PixelColor color);

void blackout_window();