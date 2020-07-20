#include "window.h"

Window window;
PixelColor default_backgroundcolor = { 0x00, 0x00, 0x00, 0x00 };
PixelColor default_drawcolor = { 0xff, 0xff, 0xff, 0x00 };

void init_graphic(Window* w) {
    window.baseAddr = w->baseAddr;
    window.size = w->size;
    window.width = w->width;
    window.height = w->height;
    window.format = w->format;
    window.pixelsPerScanLine = w->pixelsPerScanLine;
}

void draw_pixel_with_color(uint32_t x, uint32_t y, PixelColor color) {
    PixelColor *w = (PixelColor*)window.baseAddr;
    w[x + (y * window.pixelsPerScanLine)] = color;
}

void draw_pixel(uint32_t x, uint32_t y) {
    draw_pixel_with_color(x, y, default_drawcolor);
}

void fill_window(PixelColor color) {
    PixelColor *w = (PixelColor*)window.baseAddr;

    for (uint64_t i = 0; i < window.size; i++) {
        w[i] = color;
    }
}

void blackout_window() {
    fill_window(default_backgroundcolor);
}