#pragma once
#include <stdint.h>
#include "frameBuffer.hpp"

typedef struct Window {
    uint64_t* baseAddr;
    uint64_t size;
    uint32_t width, height;
    enum PixelFormat format;
    uint32_t pixelsPerScanLine;
}Window;