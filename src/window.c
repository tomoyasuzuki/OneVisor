#include "window.h"
#include "font.h"

Window window;
PixelColor default_backgroundcolor = { 0x00, 0x00, 0x00, 0x00 };
PixelColor default_drawcolor = { 0xff, 0xff, 0xff, 0x00 };

void init_graphic(FrameBuffer* buff) {
    window.baseAddr = buff->baseAddr;
    window.size = buff->size;
    window.width = buff->width;
    window.height = buff->height;
    window.format = buff->pixelFormat;
    window.pixelsPerScanLine = buff->pixelsPerScanLine;
    window.current_x = 0;
    window.current_y = 0;
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

void draw_font(uint32_t x, uint32_t y, char c) {
    char font;

    for (int i = 0; i < 16; i++) {
        font = fonts[c * 16 + i];
        if ((font & 0x80) != 0) { draw_pixel(x + 0, y + i); }
        if ((font & 0x40) != 0) { draw_pixel(x + 1, y + i); }
        if ((font & 0x20) != 0) { draw_pixel(x + 2, y + i); }
        if ((font & 0x10) != 0) { draw_pixel(x + 3, y + i); }
        if ((font & 0x08) != 0) { draw_pixel(x + 4, y + i); }
        if ((font & 0x04) != 0) { draw_pixel(x + 5, y + i); }
        if ((font & 0x02) != 0) { draw_pixel(x + 6, y + i); }
        if ((font & 0x01) != 0) { draw_pixel(x + 7, y + i); }
    }

    if (window.current_x < window.pixelsPerScanLine - 8) {
        window.current_x += 8;
    } else {
        window.current_x = 0;
        window.current_y += 16;
    }
}

void put_c(char c) {
    draw_font(window.current_x, window.current_y, c);
}

void put_s(char *s) {
    int i = 0;
    while(s[i] != '\0') {
        put_c(s[i]);
        i++;
    }
}
