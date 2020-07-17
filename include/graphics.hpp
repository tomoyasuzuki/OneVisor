#pragma once
#include <stdint.h>
#include "frameBuffer.hpp"
#include "window.hpp"
#include "color.hpp"

void drawPixel(Window* window, uint8_t x, uint8_t y, PixelColor color);