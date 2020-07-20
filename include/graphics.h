#pragma once
#include <stdint.h>
#include "frameBuffer.h"
#include "window.h"
#include "color.h"

void drawPixel(Window* window, uint8_t x, uint8_t y, PixelColor color);